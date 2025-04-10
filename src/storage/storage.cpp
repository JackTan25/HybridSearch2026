

module;

#include <filesystem>
#include <functional>
#include <regex>
#include <string>

module storage;

import config;
import stl;
import buffer_manager;
import default_values;
import wal_manager;
import catalog;
import txn_manager;
import builtin_functions;
import third_party;
import logger;

import txn;
import hybridsearch_exception;
import status;
import background_process;
import compaction_process;
import object_storage_process;
import status;
import bg_task;
import periodic_trigger_thread;
import periodic_trigger;
import log_file;

import query_context;
import hybridsearch_context;
import memindex_tracer;
import cleanup_scanner;
import persistence_manager;
import extra_ddl_info;
import virtual_store;
import result_cache_manager;
import global_resource_usage;
import txn_state;

namespace hybridsearch {

Storage::Storage(Config *config_ptr) : config_ptr_(config_ptr) {
#ifdef hybridsearch_DEBUG
    GlobalResourceUsage::IncrObjectCount("Storage");
#endif
}

Storage::~Storage() {
#ifdef hybridsearch_DEBUG
    GlobalResourceUsage::DecrObjectCount("Storage");
#endif
}

Status Storage::InitToAdmin() {
    LOG_INFO(fmt::format("Start to initialize storage from un-init mode to admin"));
    {
        std::unique_lock<std::mutex> lock(mutex_);
        if (current_storage_mode_ != StorageMode::kUnInitialized) {
            UnrecoverableError("Unexpected: attempt to init initialized status storage");
        }

        // Construct wal manager
        if (wal_mgr_ != nullptr) {
            UnrecoverableError("WAL manager was initialized before.");
        }

        wal_mgr_ = MakeUnique<WalManager>(this,
                                          config_ptr_->WALDir(),
                                          config_ptr_->DataDir(),
                                          config_ptr_->WALCompactThreshold(),
                                          config_ptr_->DeltaCheckpointThreshold(),
                                          config_ptr_->FlushMethodAtCommit());

        switch (config_ptr_->StorageType()) {
            case StorageType::kLocal: {
                // Not init remote store
                break;
            }
            case StorageType::kMinio: {
                if (VirtualStore::IsInit()) {
                    VirtualStore::UnInitRemoteStore();
                }
                LOG_INFO(fmt::format("Init remote store url: {}", config_ptr_->ObjectStorageUrl()));
                Status status = VirtualStore::InitRemoteStore(StorageType::kMinio,
                                                              config_ptr_->ObjectStorageUrl(),
                                                              config_ptr_->ObjectStorageHttps(),
                                                              config_ptr_->ObjectStorageAccessKey(),
                                                              config_ptr_->ObjectStorageSecretKey(),
                                                              config_ptr_->ObjectStorageBucket());
                if (!status.ok()) {
                    VirtualStore::UnInitRemoteStore();
                    return status;
                }

                if (object_storage_processor_ != nullptr) {
                    object_storage_processor_->Stop();
                    object_storage_processor_.reset();
                }
                object_storage_processor_ = MakeUnique<ObjectStorageProcess>();
                object_storage_processor_->Start();
                break;
            }
            default: {
                UnrecoverableError(fmt::format("Unsupported storage type: {}.", ToString(config_ptr_->StorageType())));
            }
        }
        // Construct persistence store
        String persistence_dir = config_ptr_->PersistenceDir();
        if (!persistence_dir.empty()) {
            if (persistence_manager_ != nullptr) {
                persistence_manager_.reset();
            }
            i64 persistence_object_size_limit = config_ptr_->PersistenceObjectSizeLimit();
            persistence_manager_ = MakeUnique<PersistenceManager>(persistence_dir, config_ptr_->DataDir(), (SizeT)persistence_object_size_limit);
        }

        current_storage_mode_ = StorageMode::kAdmin;
    }
    LOG_INFO(fmt::format("Finish initializing storage from un-init mode to admin"));
    return Status::OK();
}

Status Storage::UnInitFromAdmin() {
    LOG_INFO(fmt::format("Start to un-initialize storage from admin mode to un-init"));
    {
        std::unique_lock<std::mutex> lock(mutex_);

        if (persistence_manager_ != nullptr) {
            persistence_manager_.reset();
        }

        switch (config_ptr_->StorageType()) {
            case StorageType::kLocal: {
                // Not init remote store
                break;
            }
            case StorageType::kMinio: {
                if (object_storage_processor_ != nullptr) {
                    object_storage_processor_->Stop();
                    object_storage_processor_.reset();
                    VirtualStore::UnInitRemoteStore();
                }
                break;
            }
            default: {
                UnrecoverableError(fmt::format("Unsupported storage type: {}.", ToString(config_ptr_->StorageType())));
            }
        }

        if (wal_mgr_ != nullptr) {
            wal_mgr_->Stop();
            wal_mgr_.reset();
        }

        wal_mgr_.reset();

        if (memory_index_tracer_ != nullptr) {
            memory_index_tracer_.reset();
        }

        current_storage_mode_ = StorageMode::kUnInitialized;
    }
    LOG_INFO(fmt::format("Finishing un-initializing storage from admin mode to un-init"));
    return Status::OK();
}

Status Storage::AdminToReader() {
    LOG_INFO(fmt::format("Start to update storage from admin mode to reader"));
    std::unique_lock<std::mutex> lock(mutex_);

    if (result_cache_manager_ != nullptr) {
        result_cache_manager_.reset();
    }
    SizeT cache_result_num = config_ptr_->CacheResultNum();
    if (result_cache_manager_ == nullptr) {
        result_cache_manager_ = MakeUnique<ResultCacheManager>(cache_result_num);
    }

    // Construct buffer manager
    if (buffer_mgr_ != nullptr) {
        buffer_mgr_->Stop();
        buffer_mgr_.reset();
    }
    buffer_mgr_ = MakeUnique<BufferManager>(config_ptr_->BufferManagerSize(),
                                            MakeShared<String>(config_ptr_->DataDir()),
                                            MakeShared<String>(config_ptr_->TempDir()),
                                            persistence_manager_.get(),
                                            config_ptr_->LRUNum());
    buffer_mgr_->Start();

    LOG_INFO("No checkpoint found in READER mode, waiting for log replication");
    reader_init_phase_ = ReaderInitPhase::kPhase1;
    return Status::OK();
}

Status Storage::AdminToWriter() {

    {
        std::unique_lock<std::mutex> lock(mutex_);
        current_storage_mode_ = StorageMode::kWritable;
    }

    if (config_ptr_->StorageType() == StorageType::kMinio) {
        Status status;
        try {
            status = VirtualStore::CreateBucket();
        } catch (std::exception &e) {
            status = Status::Unknown(e.what());
        }
        if (!status.ok()) {
            std::unique_lock<std::mutex> lock(mutex_);
            current_storage_mode_ = StorageMode::kAdmin;
            return status;
        }
    }

    if (result_cache_manager_ != nullptr) {
        result_cache_manager_.reset();
    }
    SizeT cache_result_num = config_ptr_->CacheResultNum();
    if (result_cache_manager_ == nullptr) {
        result_cache_manager_ = MakeUnique<ResultCacheManager>(cache_result_num);
    }

    // Construct buffer manager
    if (buffer_mgr_ != nullptr) {
        buffer_mgr_->Stop();
        buffer_mgr_.reset();
    }
    buffer_mgr_ = MakeUnique<BufferManager>(config_ptr_->BufferManagerSize(),
                                            MakeShared<String>(config_ptr_->DataDir()),
                                            MakeShared<String>(config_ptr_->TempDir()),
                                            persistence_manager_.get(),
                                            config_ptr_->LRUNum());
    buffer_mgr_->Start();

    // Must init catalog before txn manager.
    // Replay wal file wrap init catalog
    TxnTimeStamp system_start_ts = wal_mgr_->ReplayWalFile(StorageMode::kWritable);
    if (system_start_ts == 0) {
        // Init database, need to create default_db
        LOG_INFO(fmt::format("Init a new catalog"));
        catalog_ = Catalog::NewCatalog();
    }

    i64 compact_interval = config_ptr_->CompactInterval() > 0 ? config_ptr_->CompactInterval() : 0;
    if (compact_interval > 0) {
        LOG_INFO(fmt::format("Init compaction alg"));
        catalog_->InitCompactionAlg(system_start_ts);
    } else {
        LOG_INFO(fmt::format("Skip init compaction alg"));
    }

    BuiltinFunctions builtin_functions(catalog_);
    builtin_functions.Init();
    // Catalog finish init here.
    if (bg_processor_ != nullptr) {
        UnrecoverableError("Background processor was initialized before.");
    }
    bg_processor_ = MakeUnique<BGTaskProcessor>(wal_mgr_.get(), catalog_.get());

    // Construct txn manager
    if (txn_mgr_ != nullptr) {
        UnrecoverableError("Transaction manager was initialized before.");
    }
    txn_mgr_ = MakeUnique<TxnManager>(buffer_mgr_.get(), wal_mgr_.get(), system_start_ts);
    txn_mgr_->Start();

    // start WalManager after TxnManager since it depends on TxnManager.
    wal_mgr_->Start();

    if (system_start_ts == 0) {
        CreateDefaultDB();
    }

    if (memory_index_tracer_ != nullptr) {
        UnrecoverableError("Memory index tracer was initialized before.");
    }
    memory_index_tracer_ = MakeUnique<BGMemIndexTracer>(config_ptr_->MemIndexMemoryQuota(), catalog_.get(), txn_mgr_.get());
    cleanup_info_tracer_ = MakeUnique<CleanupInfoTracer>();

    bg_processor_->Start();

    // Compact processor will do in WRITABLE MODE:
    // 1. Compact segments into a big one
    // 2. Scan which segments should be merged into one
    // 3. Save the dumped mem index in catalog

    if (compact_processor_ != nullptr) {
        UnrecoverableError("compact processor was initialized before.");
    }

    compact_processor_ = MakeUnique<CompactionProcessor>(catalog_.get(), txn_mgr_.get());
    compact_processor_->Start();

    // recover index after start compact process
    catalog_->StartMemoryIndexCommit();
    catalog_->MemIndexRecover(buffer_mgr_.get(), system_start_ts);

    if (periodic_trigger_thread_ != nullptr) {
        UnrecoverableError("periodic trigger was initialized before.");
    }
    periodic_trigger_thread_ = MakeUnique<PeriodicTriggerThread>();

    i64 optimize_interval = config_ptr_->OptimizeIndexInterval() > 0 ? config_ptr_->OptimizeIndexInterval() : 0;
    i64 cleanup_interval = config_ptr_->CleanupInterval() > 0 ? config_ptr_->CleanupInterval() : 0;
    i64 full_checkpoint_interval_sec = config_ptr_->FullCheckpointInterval() > 0 ? config_ptr_->FullCheckpointInterval() : 0;
    i64 delta_checkpoint_interval_sec = config_ptr_->DeltaCheckpointInterval() > 0 ? config_ptr_->DeltaCheckpointInterval() : 0;

    periodic_trigger_thread_->full_checkpoint_trigger_ = MakeShared<CheckpointPeriodicTrigger>(full_checkpoint_interval_sec, wal_mgr_.get(), true);
    periodic_trigger_thread_->delta_checkpoint_trigger_ = MakeShared<CheckpointPeriodicTrigger>(delta_checkpoint_interval_sec, wal_mgr_.get(), false);
    periodic_trigger_thread_->compact_segment_trigger_ = MakeShared<CompactSegmentPeriodicTrigger>(compact_interval, compact_processor_.get());
    periodic_trigger_thread_->optimize_index_trigger_ = MakeShared<OptimizeIndexPeriodicTrigger>(optimize_interval, compact_processor_.get());

    periodic_trigger_thread_->cleanup_trigger_ =
        MakeShared<CleanupPeriodicTrigger>(cleanup_interval, bg_processor_.get(), catalog_.get(), txn_mgr_.get());
    bg_processor_->SetCleanupTrigger(periodic_trigger_thread_->cleanup_trigger_);

    auto txn = txn_mgr_->BeginTxn(MakeUnique<String>("ForceCheckpointTask"), TransactionType::kNormal);
    auto force_ckp_task = MakeShared<ForceCheckpointTask>(txn, true, system_start_ts);
    bg_processor_->Submit(force_ckp_task);
    force_ckp_task->Wait();
    txn->AddOperation(MakeShared<String>("ForceCheckpointTask"));
    txn->SetReaderAllowed(true);
    txn_mgr_->CommitTxn(txn);

    periodic_trigger_thread_->Start();
    return Status::OK();
}

// Used for follower and learner
Status Storage::UnInitFromReader() {
    LOG_INFO(fmt::format("Start to change storage from readable mode to un-init"));
    {
        if (periodic_trigger_thread_ != nullptr) {
            if (reader_init_phase_ != ReaderInitPhase::kPhase2) {
                UnrecoverableError("Error reader init phase");
            }
            periodic_trigger_thread_->Stop();
            periodic_trigger_thread_.reset();
        }

        if (bg_processor_ != nullptr) {
            if (reader_init_phase_ != ReaderInitPhase::kPhase2) {
                UnrecoverableError("Error reader init phase");
            }
            bg_processor_->Stop();
            bg_processor_.reset();
        }

        catalog_.reset();

        if (memory_index_tracer_ != nullptr) {
            memory_index_tracer_.reset();
        }

        if (wal_mgr_ != nullptr) {
            wal_mgr_->Stop();
            wal_mgr_.reset();
        }

        if (txn_mgr_ != nullptr) {
            if (reader_init_phase_ != ReaderInitPhase::kPhase2) {
                UnrecoverableError("Error reader init phase");
            }
            txn_mgr_->Stop();
            txn_mgr_.reset();
        }

        switch (config_ptr_->StorageType()) {
            case StorageType::kLocal: {
                // Not init remote store
                break;
            }
            case StorageType::kMinio: {
                if (object_storage_processor_ != nullptr) {
                    object_storage_processor_->Stop();
                    object_storage_processor_.reset();
                    VirtualStore::UnInitRemoteStore();
                }
                break;
            }
            default: {
                UnrecoverableError(fmt::format("Unsupported storage type: {}.", ToString(config_ptr_->StorageType())));
            }
        }

        if (buffer_mgr_ != nullptr) {
            buffer_mgr_->Stop();
            buffer_mgr_.reset();
        }

        if (result_cache_manager_ != nullptr) {
            result_cache_manager_.reset();
        }

        if (persistence_manager_ != nullptr) {
            persistence_manager_.reset();
        }

        if (cleanup_info_tracer_ != nullptr) {
            cleanup_info_tracer_.reset();
        }

        std::unique_lock<std::mutex> lock(mutex_);
        current_storage_mode_ = StorageMode::kUnInitialized;
    }
    LOG_INFO(fmt::format("Finishing changing storage from readable mode to un-init"));
    return Status::OK();
}

Status Storage::ReaderToAdmin() {
    LOG_INFO(fmt::format("Start to change storage from readable mode to admin"));

    Status status = UnInitFromReader();
    if (!status.ok()) {
        return status;
    }

    status = InitToAdmin();
    if (!status.ok()) {
        return status;
    }
    LOG_INFO(fmt::format("Finishing changing storage from readable mode to admin"));
    return Status::OK();
}

Status Storage::ReaderToWriter() {
    if (compact_processor_ != nullptr) {
        UnrecoverableError("compact processor was initialized before.");
    }

    compact_processor_ = MakeUnique<CompactionProcessor>(catalog_.get(), txn_mgr_.get());
    compact_processor_->Start();

    periodic_trigger_thread_->Stop();
    i64 compact_interval = config_ptr_->CompactInterval() > 0 ? config_ptr_->CompactInterval() : 0;
    i64 optimize_interval = config_ptr_->OptimizeIndexInterval() > 0 ? config_ptr_->OptimizeIndexInterval() : 0;
    //                i64 cleanup_interval = config_ptr_->CleanupInterval() > 0 ? config_ptr_->CleanupInterval() : 0;
    i64 full_checkpoint_interval_sec = config_ptr_->FullCheckpointInterval() > 0 ? config_ptr_->FullCheckpointInterval() : 0;
    i64 delta_checkpoint_interval_sec = config_ptr_->DeltaCheckpointInterval() > 0 ? config_ptr_->DeltaCheckpointInterval() : 0;
    periodic_trigger_thread_->full_checkpoint_trigger_ = MakeShared<CheckpointPeriodicTrigger>(full_checkpoint_interval_sec, wal_mgr_.get(), true);
    periodic_trigger_thread_->delta_checkpoint_trigger_ = MakeShared<CheckpointPeriodicTrigger>(delta_checkpoint_interval_sec, wal_mgr_.get(), false);
    periodic_trigger_thread_->compact_segment_trigger_ = MakeShared<CompactSegmentPeriodicTrigger>(compact_interval, compact_processor_.get());
    periodic_trigger_thread_->optimize_index_trigger_ = MakeShared<OptimizeIndexPeriodicTrigger>(optimize_interval, compact_processor_.get());
    periodic_trigger_thread_->Start();

    std::unique_lock<std::mutex> lock(mutex_);
    current_storage_mode_ = StorageMode::kWritable;
    return Status::OK();
}

Status Storage::WriterToAdmin() {
    if (periodic_trigger_thread_ != nullptr) {
        periodic_trigger_thread_->Stop();
        periodic_trigger_thread_.reset();
    }

    if (compact_processor_ != nullptr) {
        compact_processor_->Stop(); // Different from Readable
        compact_processor_.reset(); // Different from Readable
    }

    if (bg_processor_ != nullptr) {
        bg_processor_->Stop();
        bg_processor_.reset();
    }

    catalog_.reset();

    memory_index_tracer_.reset();

    if (wal_mgr_ != nullptr) {
        wal_mgr_->Stop();
        wal_mgr_.reset();
    }

    if (txn_mgr_ != nullptr) {
        txn_mgr_->Stop();
        txn_mgr_.reset();
    }

    if (buffer_mgr_ != nullptr) {
        buffer_mgr_->Stop();
        buffer_mgr_.reset();
    }

    if (result_cache_manager_ != nullptr) {
        result_cache_manager_.reset();
    }

    if (cleanup_info_tracer_ != nullptr) {
        cleanup_info_tracer_.reset();
    }

    // wal_manager stop won't reset many member. We need to recreate the wal_manager object.
    wal_mgr_ = MakeUnique<WalManager>(this,
                                      config_ptr_->WALDir(),
                                      config_ptr_->DataDir(),
                                      config_ptr_->WALCompactThreshold(),
                                      config_ptr_->DeltaCheckpointThreshold(),
                                      config_ptr_->FlushMethodAtCommit());

    std::unique_lock<std::mutex> lock(mutex_);
    current_storage_mode_ = StorageMode::kAdmin;
    return Status::OK();
}
Status Storage::WriterToReader() {
    if (periodic_trigger_thread_ != nullptr) {
        periodic_trigger_thread_->Stop();
        periodic_trigger_thread_.reset();
    }

    if (compact_processor_ != nullptr) {
        compact_processor_->Stop(); // Different from Readable
        compact_processor_.reset(); // Different from Readable
    }

    i64 cleanup_interval = config_ptr_->CleanupInterval() > 0 ? config_ptr_->CleanupInterval() : 0;

    periodic_trigger_thread_ = MakeUnique<PeriodicTriggerThread>();
    periodic_trigger_thread_->cleanup_trigger_ =
        MakeShared<CleanupPeriodicTrigger>(cleanup_interval, bg_processor_.get(), catalog_.get(), txn_mgr_.get());
    bg_processor_->SetCleanupTrigger(periodic_trigger_thread_->cleanup_trigger_);
    periodic_trigger_thread_->Start();

    std::unique_lock<std::mutex> lock(mutex_);
    current_storage_mode_ = StorageMode::kReadable;
    return Status::OK();
}
Status Storage::UnInitFromWriter() {

    if (periodic_trigger_thread_ != nullptr) {
        periodic_trigger_thread_->Stop();
        periodic_trigger_thread_.reset();
    }

    if (compact_processor_ != nullptr) {
        compact_processor_->Stop(); // Different from Readable
        compact_processor_.reset(); // Different from Readable
    }

    if (bg_processor_ != nullptr) {
        bg_processor_->Stop();
        bg_processor_.reset();
    }

    catalog_.reset();

    memory_index_tracer_.reset();

    if (wal_mgr_ != nullptr) {
        wal_mgr_->Stop();
        wal_mgr_.reset();
    }

    switch (config_ptr_->StorageType()) {
        case StorageType::kLocal: {
            // Not init remote store
            break;
        }
        case StorageType::kMinio: {
            if (object_storage_processor_ != nullptr) {
                object_storage_processor_->Stop();
                object_storage_processor_.reset();
                VirtualStore::UnInitRemoteStore();
            }
            break;
        }
        default: {
            UnrecoverableError(fmt::format("Unsupported storage type: {}.", ToString(config_ptr_->StorageType())));
        }
    }

    if (txn_mgr_ != nullptr) {
        txn_mgr_->Stop();
        txn_mgr_.reset();
    }

    if (buffer_mgr_ != nullptr) {
        buffer_mgr_->Stop();
        buffer_mgr_.reset();
    }

    if (result_cache_manager_ != nullptr) {
        result_cache_manager_.reset();
    }

    if (persistence_manager_ != nullptr) {
        persistence_manager_.reset();
    }

    if (cleanup_info_tracer_ != nullptr) {
        cleanup_info_tracer_.reset();
    }

    std::unique_lock<std::mutex> lock(mutex_);
    current_storage_mode_ = StorageMode::kUnInitialized;
    return Status::OK();
}

ResultCacheManager *Storage::result_cache_manager() const noexcept {
    if (config_ptr_->ResultCache() != "on") {
        return nullptr;
    }
    return result_cache_manager_.get();
}

ResultCacheManager *Storage::GetResultCacheManagerPtr() const noexcept { return result_cache_manager_.get(); }

StorageMode Storage::GetStorageMode() const {
    std::unique_lock<std::mutex> lock(mutex_);
    return current_storage_mode_;
}

Status Storage::SetStorageMode(StorageMode target_mode) {
    StorageMode current_mode = GetStorageMode();
    if (current_mode == target_mode) {
        LOG_WARN(fmt::format("Set unchanged mode"));
        return Status::OK();
    }
    switch (current_mode) {
        case StorageMode::kUnInitialized: {
            if (target_mode != StorageMode::kAdmin) {
                UnrecoverableError("Attempt to set storage mode from UnInit to UnInit");
            }
            return InitToAdmin();
        }
        case StorageMode::kAdmin: {
            if (target_mode == StorageMode::kAdmin) {
                UnrecoverableError("Attempt to set storage mode from Admin to Admin");
            }

            switch (target_mode) {
                case StorageMode::kUnInitialized: {
                    return UnInitFromAdmin();
                }
                case StorageMode::kAdmin: {
                    UnrecoverableError("Attempt to set storage mode from Admin to Admin");
                    break;
                }
                case StorageMode::kReadable: {
                    return AdminToReader();
                }
                case StorageMode::kWritable: {
                    return AdminToWriter();
                }
            }
            break;
        }
        case StorageMode::kReadable: {

            switch (target_mode) {
                case StorageMode::kUnInitialized: {
                    return UnInitFromReader();
                }
                case StorageMode::kAdmin: {
                    return ReaderToAdmin();
                }
                case StorageMode::kReadable: {
                    UnrecoverableError("Attempt to set storage mode from Readable to Readable");
                }
                case StorageMode::kWritable: {
                    return ReaderToWriter();
                }
            }
            break;
        }
        case StorageMode::kWritable: {
            switch (target_mode) {
                case StorageMode::kUnInitialized: {
                    return UnInitFromWriter();
                }
                case StorageMode::kAdmin: {
                    return WriterToAdmin();
                }
                case StorageMode::kReadable: {
                    return WriterToReader();
                }
                case StorageMode::kWritable: {
                    UnrecoverableError("Attempt to set storage mode from Writable to Writable");
                }
            }
            break;
        }
    }
    return Status::OK();
}

Status Storage::AdminToReaderBottom(TxnTimeStamp system_start_ts) {

    std::unique_lock<std::mutex> lock(mutex_);
    if (reader_init_phase_ != ReaderInitPhase::kPhase1) {
        UnrecoverableError(fmt::format("Expect current storage mode is READER with Phase1"));
    }

    BuiltinFunctions builtin_functions(catalog_);
    builtin_functions.Init();
    // Catalog finish init here.
    if (bg_processor_ != nullptr) {
        UnrecoverableError("Background processor was initialized before.");
    }
    bg_processor_ = MakeUnique<BGTaskProcessor>(wal_mgr_.get(), catalog_.get());

    // Construct txn manager
    if (txn_mgr_ != nullptr) {
        UnrecoverableError("Transaction manager was initialized before.");
    }
    txn_mgr_ = MakeUnique<TxnManager>(buffer_mgr_.get(), wal_mgr_.get(), system_start_ts);
    txn_mgr_->Start();

    // start WalManager after TxnManager since it depends on TxnManager.
    wal_mgr_->Start();

    if (memory_index_tracer_ != nullptr) {
        UnrecoverableError("Memory index tracer was initialized before.");
    }
    memory_index_tracer_ = MakeUnique<BGMemIndexTracer>(config_ptr_->MemIndexMemoryQuota(), catalog_.get(), txn_mgr_.get());
    cleanup_info_tracer_ = MakeUnique<CleanupInfoTracer>();

    catalog_->StartMemoryIndexCommit();
    catalog_->MemIndexRecover(buffer_mgr_.get(), system_start_ts);

    bg_processor_->Start();

    if (periodic_trigger_thread_ != nullptr) {
        UnrecoverableError("periodic trigger was initialized before.");
    }
    periodic_trigger_thread_ = MakeUnique<PeriodicTriggerThread>();

    i64 cleanup_interval = config_ptr_->CleanupInterval() > 0 ? config_ptr_->CleanupInterval() : 0;
    periodic_trigger_thread_->cleanup_trigger_ =
        MakeShared<CleanupPeriodicTrigger>(cleanup_interval, bg_processor_.get(), catalog_.get(), txn_mgr_.get());
    bg_processor_->SetCleanupTrigger(periodic_trigger_thread_->cleanup_trigger_);

    periodic_trigger_thread_->Start();
    reader_init_phase_ = ReaderInitPhase::kPhase2;
    current_storage_mode_ = StorageMode::kReadable;

    return Status::OK();
}

void Storage::AttachCatalog(const FullCatalogFileInfo &full_ckp_info, const Vector<DeltaCatalogFileInfo> &delta_ckp_infos) {
    catalog_ = Catalog::LoadFromFiles(full_ckp_info, delta_ckp_infos, buffer_mgr_.get());
}

void Storage::LoadFullCheckpoint(const String &checkpoint_path) {
    if (catalog_.get() != nullptr) {
        UnrecoverableError("Catalog was already initialized before.");
    }
    catalog_ = Catalog::LoadFullCheckpoint(checkpoint_path);
}
void Storage::AttachDeltaCheckpoint(const String &checkpoint_path) { catalog_->AttachDeltaCheckpoint(checkpoint_path); }

void Storage::CreateDefaultDB() {
    Txn *new_txn = txn_mgr_->BeginTxn(MakeUnique<String>("create db1"), TransactionType::kNormal);
    new_txn->SetReaderAllowed(true);
    // Txn1: Create db1, OK
    Status status = new_txn->CreateDatabase(MakeShared<String>("default_db"), ConflictType::kError, MakeShared<String>("Initial startup created"));
    if (!status.ok()) {
        UnrecoverableError("Can't initial 'default_db'");
    }
    txn_mgr_->CommitTxn(new_txn);
}

} // namespace hybridsearch
