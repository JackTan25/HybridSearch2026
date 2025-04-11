

module;

module bg_task;

import base_memindex;
import chunk_index_entry;
import cleanup_scanner;
import hybridsearch_context;
import storage;

namespace hybridsearch {

void CleanupTask::Execute() {
    auto *storage = hybridsearchContext::instance().storage();
    CleanupScanner scanner(catalog_, visible_ts_, buffer_mgr_);
    scanner.Scan();

    auto *tracer = storage->cleanup_info_tracer();
    tracer->ResetInfo(visible_ts_);
    std::move(scanner).Cleanup(tracer);
}

DumpIndexTask::DumpIndexTask(BaseMemIndex *mem_index, Txn *txn) : BGTask(BGTaskType::kDumpIndex, true), mem_index_(mem_index), txn_(txn) {}

DumpIndexBylineTask::DumpIndexBylineTask(SharedPtr<String> db_name,
                                         SharedPtr<String> table_name,
                                         SharedPtr<String> index_name,
                                         SegmentID segment_id,
                                         SharedPtr<ChunkIndexEntry> dumped_chunk)
    : BGTask(BGTaskType::kDumpIndexByline, true), db_name_(db_name), table_name_(table_name), index_name_(index_name), segment_id_(segment_id),
      dumped_chunk_(dumped_chunk) {}

TestCommandTask::TestCommandTask(String command_content) : BGTask(BGTaskType::kTestCommand, true), command_content_(std::move(command_content)) {}

} // namespace hybridsearch