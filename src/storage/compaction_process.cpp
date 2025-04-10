

module;

#include <thread>

module compaction_process;

import stl;
import bg_task;
import catalog;
import txn_manager;
import db_entry;
import table_entry;
import logger;
import hybridsearch_exception;
import third_party;
import blocking_queue;
import bg_query_state;
import query_context;
import hybridsearch_context;
import compact_statement;
import compilation_config;
import defer_op;
import bg_query_state;
import txn_store;
import memindex_tracer;
import segment_entry;
import table_index_entry;
import base_memindex;
import segment_index_entry;
import status;
import default_values;
import wal_manager;
import global_resource_usage;
import txn_state;

namespace hybridsearch {

CompactionProcessor::CompactionProcessor(Catalog *catalog, TxnManager *txn_mgr) : catalog_(catalog), txn_mgr_(txn_mgr) {
#ifdef hybridsearch_DEBUG
    GlobalResourceUsage::IncrObjectCount("CompactionProcessor");
#endif
}

CompactionProcessor::~CompactionProcessor() {
#ifdef hybridsearch_DEBUG
    GlobalResourceUsage::DecrObjectCount("CompactionProcessor");
#endif
}

void CompactionProcessor::Start() {
    LOG_INFO("Compaction processor is started.");
    processor_thread_ = Thread([this] { Process(); });
}

void CompactionProcessor::Stop() {
    LOG_INFO("Compaction processor is stopping.");
    SharedPtr<StopProcessorTask> stop_task = MakeShared<StopProcessorTask>();
    this->Submit(stop_task);
    stop_task->Wait();
    processor_thread_.join();
    LOG_INFO("Compaction processor is stopped.");
}

void CompactionProcessor::Submit(SharedPtr<BGTask> bg_task) {
    task_queue_.Enqueue(std::move(bg_task));
    ++task_count_;
}

void CompactionProcessor::DoCompact() {
    Txn *scan_txn = txn_mgr_->BeginTxn(MakeUnique<String>("ScanForCompact"), TransactionType::kNormal);
    bool success = false;
    DeferFn defer_fn([&] {
        if (!success) {
            txn_mgr_->RollBackTxn(scan_txn);
        }
    });

    Vector<Pair<UniquePtr<BaseStatement>, Txn *>> statements = this->ScanForCompact(scan_txn);
    Vector<Pair<BGQueryContextWrapper, BGQueryState>> wrappers;
    for (const auto &[statement, txn] : statements) {
        BGQueryContextWrapper wrapper(txn);
        BGQueryState state;
        try {
            bool res = wrapper.query_context_->ExecuteBGStatement(statement.get(), state);
            if (res) {
                wrappers.emplace_back(std::move(wrapper), std::move(state));
            }
        } catch (const std::exception &e) {
            LOG_CRITICAL(fmt::format("DoCompact failed: {}", e.what()));
            throw;
        }
    }
    for (auto &[wrapper, query_state] : wrappers) {
        TxnTimeStamp commit_ts_out = 0;
        wrapper.query_context_->JoinBGStatement(query_state, commit_ts_out);
    }
    txn_mgr_->CommitTxn(scan_txn);
    success = true;
}

TxnTimeStamp
CompactionProcessor::ManualDoCompact(const String &schema_name, const String &table_name, bool rollback, Optional<std::function<void()>> mid_func) {
    auto statement = MakeUnique<ManualCompactStatement>(schema_name, table_name);
    Txn *txn = txn_mgr_->BeginTxn(MakeUnique<String>("ManualCompact"), TransactionType::kNormal);
    LOG_INFO(fmt::format("Compact txn id {}.", txn->TxnID()));
    BGQueryContextWrapper wrapper(txn);
    BGQueryState state;
    bool res = wrapper.query_context_->ExecuteBGStatement(statement.get(), state);
    if (mid_func) {
        mid_func.value()();
    }
    TxnTimeStamp out_commit_ts = 0;
    if (res) {
        wrapper.query_context_->JoinBGStatement(state, out_commit_ts, rollback);
    }
    return out_commit_ts;
}

Vector<Pair<UniquePtr<BaseStatement>, Txn *>> CompactionProcessor::ScanForCompact(Txn *scan_txn) {

    Vector<Pair<UniquePtr<BaseStatement>, Txn *>> compaction_tasks;
    TransactionID txn_id = scan_txn->TxnID();
    TxnTimeStamp begin_ts = scan_txn->BeginTS();
    Vector<DBEntry *> db_entries = catalog_->Databases(txn_id, begin_ts);
    for (auto *db_entry : db_entries) {
        Vector<TableEntry *> table_entries = db_entry->TableCollections(txn_id, begin_ts);
        for (auto *table_entry : table_entries) {
            while (true) {
                Txn *txn = txn_mgr_->BeginTxn(MakeUnique<String>("Compact"), TransactionType::kNormal);
                TransactionID txn_id = txn->TxnID();
                auto compact_segments = table_entry->CheckCompaction(txn_id);
                if (compact_segments.empty()) {
                    txn_mgr_->RollBackTxn(txn);
                    break;
                }

                LOG_TRACE("Construct compact task: ");
                for (SegmentEntry *segment_ptr : compact_segments) {
                    LOG_TRACE(fmt::format("To compact segment: {}", segment_ptr->ToString()));
                }

                compaction_tasks.emplace_back(
                    MakeUnique<AutoCompactStatement>(*table_entry->GetDBName(), *table_entry->GetTableName(), std::move(compact_segments)),
                    txn);
            }
        }
    }

    return compaction_tasks;
}

void CompactionProcessor::ScanAndOptimize() {
    Txn *txn = txn_mgr_->BeginTxn(MakeUnique<String>("ScanAndOptimize"), TransactionType::kNormal);
    LOG_INFO(fmt::format("ScanAndOptimize opt begin ts: {}", txn->BeginTS()));
    txn->OptimizeIndexes();
    try {
        txn_mgr_->CommitTxn(txn);
    } catch (const RecoverableException &e) {
        txn_mgr_->RollBackTxn(txn);
    }
}

void CompactionProcessor::DoDump(DumpIndexTask *dump_task) {
    Txn *dump_txn = dump_task->txn_;
    BaseMemIndex *mem_index = dump_task->mem_index_;
    auto *memindex_tracer = hybridsearchContext::instance().storage()->memindex_tracer();
    try {
        TableIndexEntry *table_index_entry = mem_index->table_index_entry();
        auto *table_entry = table_index_entry->table_index_meta()->GetTableEntry();
        TxnTableStore *txn_table_store = dump_txn->GetTxnTableStore(table_entry);
        SizeT dump_size = 0;
        table_index_entry->MemIndexDump(dump_txn, txn_table_store, false /*spill*/, &dump_size);
        LOG_TRACE(fmt::format("Dump size = {}", dump_size));

        txn_mgr_->CommitTxn(dump_txn);

        memindex_tracer->DumpDone(dump_size, mem_index);
    } catch (const RecoverableException &e) {
        txn_mgr_->RollBackTxn(dump_txn);
        memindex_tracer->DumpFail(mem_index);
        LOG_WARN(fmt::format("Dump index task failed: {}, task: {}", e.what(), dump_task->ToString()));
    }
}

void CompactionProcessor::DoDumpByline(DumpIndexBylineTask *dump_task) {
    String msg = fmt::format("Dump index by line, table name: {}, index name: {}", *dump_task->table_name_, *dump_task->index_name_);
    Txn *txn = txn_mgr_->BeginTxn(MakeUnique<String>(msg), TransactionType::kNormal);
    try {
        auto [table_index_entry, status] = txn->GetIndexByName(*dump_task->db_name_, *dump_task->table_name_, *dump_task->index_name_);
        if (!status.ok()) {
            RecoverableError(status);
        }
        auto *dumped_chunk = dump_task->dumped_chunk_.get();
        if (dumped_chunk->deprecate_ts_ != UNCOMMIT_TS) {
            RecoverableError(Status::TxnRollback(txn->TxnID(), fmt::format("Dumped chunk {} is deleted.", dumped_chunk->encode())));
        }

        SharedPtr<SegmentIndexEntry> segment_index_entry = table_index_entry->GetSegment(dump_task->segment_id_, txn);
        if (!segment_index_entry) {
            RecoverableError(Status::TxnRollback(txn->TxnID(), fmt::format("Cannot find segment index entry with id: {}", dump_task->segment_id_)));
        }
        segment_index_entry->AddWalIndexDump(dumped_chunk, txn);

        txn_mgr_->CommitTxn(txn);
    } catch (const RecoverableException &e) {
        txn_mgr_->RollBackTxn(txn);
        LOG_WARN(fmt::format("Rollback {}", msg));
    }
}

void CompactionProcessor::Process() {
    bool running = true;
    while (running) {
        Deque<SharedPtr<BGTask>> tasks;
        task_queue_.DequeueBulk(tasks);

        for (const auto &bg_task : tasks) {
            switch (bg_task->type_) {
                case BGTaskType::kStopProcessor: {
                    running = false;
                    break;
                }
                case BGTaskType::kNotifyCompact: {
                    StorageMode storage_mode = hybridsearchContext::instance().storage()->GetStorageMode();
                    if (storage_mode == StorageMode::kUnInitialized) {
                        UnrecoverableError("Uninitialized storage mode");
                    }
                    if (storage_mode == StorageMode::kWritable) {
                        LOG_DEBUG("Do compact start.");
                        DoCompact();
                        LOG_DEBUG("Do compact end.");
                    }
                    break;
                }
                case BGTaskType::kNotifyOptimize: {
                    StorageMode storage_mode = hybridsearchContext::instance().storage()->GetStorageMode();
                    if (storage_mode == StorageMode::kUnInitialized) {
                        UnrecoverableError("Uninitialized storage mode");
                    }
                    if (storage_mode == StorageMode::kWritable) {
                        LOG_DEBUG("Optimize start.");
                        ScanAndOptimize();
                        LOG_DEBUG("Optimize done.");
                    }
                    break;
                }
                case BGTaskType::kDumpIndex: {
                    StorageMode storage_mode = hybridsearchContext::instance().storage()->GetStorageMode();
                    if (storage_mode == StorageMode::kUnInitialized) {
                        UnrecoverableError("Uninitialized storage mode");
                    }
                    if (storage_mode == StorageMode::kWritable) {
                        auto dump_task = static_cast<DumpIndexTask *>(bg_task.get());
                        LOG_DEBUG(dump_task->ToString());
                        // Trigger transaction to save the mem index
                        DoDump(dump_task);
                        LOG_DEBUG("Dump index done.");
                    }
                    break;
                }
                case BGTaskType::kDumpIndexByline: {
                    StorageMode storage_mode = hybridsearchContext::instance().storage()->GetStorageMode();
                    if (storage_mode == StorageMode::kUnInitialized) {
                        UnrecoverableError("Uninitialized storage mode");
                    }
                    if (storage_mode == StorageMode::kWritable) {
                        if (auto cmd = test_commander_.Check(BGTaskType::kDumpIndexByline)) {
                            if (cmd.value() == "stuck for 3 seconds") {
                                LOG_INFO("Compact process stuck for 3 seconds");
                                std::this_thread::sleep_for(std::chrono::seconds(1));
                            }
                        }
                        auto dump_task = static_cast<DumpIndexBylineTask *>(bg_task.get());
                        LOG_DEBUG(dump_task->ToString());
                        // Trigger transaction to save the mem index
                        DoDumpByline(dump_task);
                        LOG_DEBUG("Dump index byline done.");
                    }
                    break;
                }
                default: {
                    String error_message = fmt::format("Invalid background task: {}", (u8)bg_task->type_);
                    UnrecoverableError(error_message);
                    break;
                }
            }
            bg_task->Complete();
        }
        task_count_ -= tasks.size();
        tasks.clear();
    }
}

} // namespace hybridsearch
