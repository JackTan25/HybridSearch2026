

module;

export module periodic_trigger_thread;

import stl;
import periodic_trigger;

namespace hybridsearch {

export class PeriodicTriggerThread {
public:
    PeriodicTriggerThread() : running_(false) {}

    ~PeriodicTriggerThread();

    void Start();

    void Stop();

    void Run();

public:
    SharedPtr<CleanupPeriodicTrigger> cleanup_trigger_;
    SharedPtr<CheckpointPeriodicTrigger> full_checkpoint_trigger_;
    SharedPtr<CheckpointPeriodicTrigger> delta_checkpoint_trigger_;
    SharedPtr<CompactSegmentPeriodicTrigger> compact_segment_trigger_;
    SharedPtr<OptimizeIndexPeriodicTrigger> optimize_index_trigger_;

private:
    Thread thread_{};
    std::mutex mtx_{};
    std::condition_variable cv_{};
    bool running_{false};
};

} // namespace hybridsearch