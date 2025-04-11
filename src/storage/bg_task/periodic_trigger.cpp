

module;

module periodic_trigger;

import stl;
import logger;
import hybridsearch_exception;
import background_process;
import bg_task;
import catalog;
import txn_manager;
import third_party;

namespace hybridsearch {

bool PeriodicTrigger::Check() {
    if (interval_ == 0) {
        return false;
    }
    const auto now = std::chrono::system_clock::now();
    i64 duration = (now - last_check_).count() / 1000000;
    duration_.store(duration);
    if (duration < interval_) {
        return false;
    }
    last_check_ = now;
    return true;
}

SharedPtr<CleanupTask> CleanupPeriodicTrigger::CreateCleanupTask(TxnTimeStamp visible_ts) {
    LOG_DEBUG(fmt::format("Trigger cleanup task, after {} seconds", duration_.load()));
    std::lock_guard lck(mtx_);
    if (visible_ts == 0) {
        visible_ts = txn_mgr_->GetCleanupScanTS() + 1;
    }
    if (visible_ts == last_visible_ts_) {
        LOG_TRACE(fmt::format("Skip cleanup. visible timestamp: {}", visible_ts));
        return nullptr;
    }
    if (visible_ts < last_visible_ts_) {
        UnrecoverableError("The visible timestamp is not monotonic.");
        return nullptr;
    }
    last_visible_ts_ = visible_ts;
    LOG_DEBUG(fmt::format("Cleanup visible timestamp: {}", visible_ts));

    auto buffer_mgr = txn_mgr_->GetBufferMgr();
    return MakeShared<CleanupTask>(catalog_, visible_ts, buffer_mgr);
}

void CleanupPeriodicTrigger::Trigger() {
    auto cleanup_task = CreateCleanupTask();
    if (cleanup_task.get() == nullptr) {
        return;
    }
    bg_processor_->Submit(std::move(cleanup_task));
}

void CheckpointPeriodicTrigger::Trigger() {
    LOG_DEBUG(fmt::format("Trigger {} periodic checkpoint, after {} seconds", is_full_checkpoint_ ? "FULL" : "DELTA", duration_.load()));
    auto checkpoint_task = MakeShared<CheckpointTask>(is_full_checkpoint_);
    // LOG_DEBUG(fmt::format("Trigger {} periodic checkpoint.", is_full_checkpoint_ ? "FULL" : "DELTA"));
    if (!wal_mgr_->TrySubmitCheckpointTask(std::move(checkpoint_task))) {
        LOG_TRACE(
            fmt::format("Skip {} checkpoint(time) because there is already a checkpoint task running.", is_full_checkpoint_ ? "FULL" : "DELTA"));
    }
}

void CompactSegmentPeriodicTrigger::Trigger() {
    LOG_DEBUG(fmt::format("Trigger compact segment task, after {} seconds", duration_.load()));
    auto compact_task = MakeShared<NotifyCompactTask>();
    compact_processor_->Submit(std::move(compact_task));
}

void OptimizeIndexPeriodicTrigger::Trigger() {
    LOG_DEBUG(fmt::format("Trigger optimize index task, after {} seconds", duration_.load()));
    auto optimize_task = MakeShared<NotifyOptimizeTask>();
    compact_processor_->Submit(std::move(optimize_task));
}

} // namespace hybridsearch