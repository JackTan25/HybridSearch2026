

module;

import stl;
import config;
import catalog;
import txn_manager;
import buffer_manager;
import wal_manager;
import background_process;
import object_storage_process;
import compaction_process;
import periodic_trigger_thread;
import log_file;
import memindex_tracer;
import persistence_manager;
import virtual_store;
import status;

export module storage;

namespace hybridsearch {

class CleanupInfoTracer;
class ResultCacheManager;

export enum class ReaderInitPhase {
    kInvalid,
    kPhase1,
    kPhase2,
};

export class Storage {
public:
    explicit Storage(Config *config_ptr);

    ~Storage();

    [[nodiscard]] inline Catalog *catalog() noexcept { return catalog_.get(); }

    [[nodiscard]] inline BufferManager *buffer_manager() noexcept { return buffer_mgr_.get(); }

    [[nodiscard]] inline BGMemIndexTracer *memindex_tracer() noexcept { return memory_index_tracer_.get(); }

    [[nodiscard]] inline TxnManager *txn_manager() const noexcept { return txn_mgr_.get(); }

    [[nodiscard]] inline WalManager *wal_manager() const noexcept { return wal_mgr_.get(); }

    [[nodiscard]] inline PersistenceManager *persistence_manager() noexcept { return persistence_manager_.get(); }

    [[nodiscard]] inline BGTaskProcessor *bg_processor() const noexcept { return bg_processor_.get(); }

    [[nodiscard]] inline ObjectStorageProcess *object_storage_processor() const noexcept { return object_storage_processor_.get(); }

    [[nodiscard]] inline PeriodicTriggerThread *periodic_trigger_thread() const noexcept { return periodic_trigger_thread_.get(); }

    [[nodiscard]] inline CompactionProcessor *compaction_processor() const noexcept { return compact_processor_.get(); }

    [[nodiscard]] inline CleanupInfoTracer *cleanup_info_tracer() const noexcept { return cleanup_info_tracer_.get(); }

    [[nodiscard]] ResultCacheManager *result_cache_manager() const noexcept;

    [[nodiscard]] ResultCacheManager *GetResultCacheManagerPtr() const noexcept;

    StorageMode GetStorageMode() const;
    Status SetStorageMode(StorageMode mode);
    Status AdminToReaderBottom(TxnTimeStamp system_start_ts);

    // Used for admin
    Status InitToAdmin();
    Status UnInitFromAdmin();
    Status AdminToReader();
    Status AdminToWriter();

    // Used for follower and learner
    Status ReaderToAdmin();
    Status ReaderToWriter();
    Status UnInitFromReader();

    // Used for leader and standalone
    Status WriterToAdmin();
    Status WriterToReader();
    Status UnInitFromWriter();

    void AttachCatalog(const FullCatalogFileInfo &full_ckp_info, const Vector<DeltaCatalogFileInfo> &delta_ckp_infos);
    void LoadFullCheckpoint(const String &checkpoint_path);
    void AttachDeltaCheckpoint(const String &checkpoint_path);

    Config *config() const { return config_ptr_; }
    ReaderInitPhase reader_init_phase() const { return reader_init_phase_; }

    void CreateDefaultDB();

private:
    Config *config_ptr_{};
    UniquePtr<CleanupInfoTracer> cleanup_info_tracer_{};
    UniquePtr<WalManager> wal_mgr_{};
    UniquePtr<ObjectStorageProcess> object_storage_processor_{};
    UniquePtr<PersistenceManager> persistence_manager_{};
    UniquePtr<ResultCacheManager> result_cache_manager_{};
    UniquePtr<BufferManager> buffer_mgr_{};
    UniquePtr<Catalog> catalog_{};
    UniquePtr<BGMemIndexTracer> memory_index_tracer_{};
    UniquePtr<TxnManager> txn_mgr_{};
    UniquePtr<BGTaskProcessor> bg_processor_{};
    UniquePtr<CompactionProcessor> compact_processor_{};
    UniquePtr<PeriodicTriggerThread> periodic_trigger_thread_{};

    mutable std::mutex mutex_;
    StorageMode current_storage_mode_{StorageMode::kUnInitialized};
    ReaderInitPhase reader_init_phase_{ReaderInitPhase::kInvalid};
};

} // namespace hybridsearch
