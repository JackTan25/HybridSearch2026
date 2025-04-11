

module;

export module background_process;

import wal_manager;
import blocking_queue;
import bg_task;
import stl;
import global_resource_usage;

namespace hybridsearch {

class Catalog;
class CleanupPeriodicTrigger;

export class BGTaskProcessor {
public:
    explicit BGTaskProcessor(WalManager *wal_manager, Catalog *catalog) : wal_manager_(wal_manager), catalog_(catalog) {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::IncrObjectCount("BGTaskProcessor");
#endif
    }
    ~BGTaskProcessor() {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::DecrObjectCount("BGTaskProcessor");
#endif
    }
    // cleanup is used before full checkpoint
    void SetCleanupTrigger(SharedPtr<CleanupPeriodicTrigger> cleanup_trigger);

    void Start();
    void Stop();

public:
    void Submit(SharedPtr<BGTask> bg_task);
    u64 RunningTaskCount() const { return task_count_; }
    String RunningTaskText() const {
        std::unique_lock<std::mutex> locker(task_mutex_);
        return task_text_;
    }

    CleanupPeriodicTrigger *cleanup_trigger() const { return cleanup_trigger_.get(); }

private:
    void Process();

private:
    BlockingQueue<SharedPtr<BGTask>> task_queue_{"BGTaskProcessor"};

    Thread processor_thread_{};

    WalManager *wal_manager_{};
    Catalog *catalog_{};
    SharedPtr<CleanupPeriodicTrigger> cleanup_trigger_;

    Atomic<u64> task_count_{};

    mutable std::mutex task_mutex_;
    String task_text_;
};

} // namespace hybridsearch
