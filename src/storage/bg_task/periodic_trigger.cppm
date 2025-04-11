

module;

export module periodic_trigger;

import stl;
import background_process;
import compaction_process;
import catalog;
import txn_manager;
import wal_manager;
import global_resource_usage;

namespace hybridsearch {

class CleanupTask;

export class PeriodicTrigger {
public:
    explicit PeriodicTrigger(i64 interval) : interval_(interval), last_check_(std::chrono::system_clock::now()) {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::IncrObjectCount("PeriodicTrigger");
#endif
    }

    virtual ~PeriodicTrigger() {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::DecrObjectCount("PeriodicTrigger");
#endif
    }

    bool Check();

    void UpdateInternal(i64 new_interval) { interval_ = new_interval; }

    virtual void Trigger() = 0;

    void Reset() { last_check_ = std::chrono::system_clock::now(); }

protected:
    Atomic<i64> interval_{};
    std::chrono::system_clock::time_point last_check_{};
    Atomic<i64> duration_{0};
};

export class CleanupPeriodicTrigger final : public PeriodicTrigger {
public:
    CleanupPeriodicTrigger(i64 interval, BGTaskProcessor *bg_processor, Catalog *catalog, TxnManager *txn_mgr)
        : PeriodicTrigger(interval), bg_processor_(bg_processor), catalog_(catalog), txn_mgr_(txn_mgr) {}

    SharedPtr<CleanupTask> CreateCleanupTask(TxnTimeStamp visible_ts = 0);

    virtual void Trigger() override;

private:
    BGTaskProcessor *const bg_processor_{};
    Catalog *const catalog_{};
    TxnManager *const txn_mgr_{};

    std::mutex mtx_;
    TxnTimeStamp last_visible_ts_{0};
};

export class CheckpointPeriodicTrigger final : public PeriodicTrigger {
public:
    explicit CheckpointPeriodicTrigger(i64 interval, WalManager *wal_mgr, bool full_checkpoint)
        : PeriodicTrigger(interval), wal_mgr_(wal_mgr), is_full_checkpoint_(full_checkpoint) {}

    virtual void Trigger() override;

private:
    WalManager *const wal_mgr_{};
    bool is_full_checkpoint_{};
};

export class CompactSegmentPeriodicTrigger final : public PeriodicTrigger {
public:
    explicit CompactSegmentPeriodicTrigger(i64 interval, CompactionProcessor *compact_processor)
        : PeriodicTrigger(interval), compact_processor_(compact_processor) {}

    virtual void Trigger() override;

private:
    CompactionProcessor *const compact_processor_{};
};

export class OptimizeIndexPeriodicTrigger final : public PeriodicTrigger {
public:
    explicit OptimizeIndexPeriodicTrigger(i64 interval, CompactionProcessor *compact_processor)
        : PeriodicTrigger(interval), compact_processor_(compact_processor) {}

    virtual void Trigger() override;

private:
    CompactionProcessor *const compact_processor_{};
};

} // namespace hybridsearch
