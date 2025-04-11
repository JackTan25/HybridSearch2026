

module;

export module task_scheduler;

import config;
import stl;
import fragment_task;
import blocking_queue;
import base_statement;

namespace hybridsearch {

class QueryContext;
class PlanFragment;

using FragmentTaskBlockQueue = BlockingQueue<FragmentTask *>;

struct Worker {
    Worker(u64 cpu_id, UniquePtr<FragmentTaskBlockQueue> queue, UniquePtr<Thread> thread);
    u64 cpu_id_{0};
    UniquePtr<FragmentTaskBlockQueue> queue_{};
    UniquePtr<Thread> thread_{};
};

export class TaskScheduler {
public:
    explicit TaskScheduler(Config *config_ptr);

    ~TaskScheduler();

    void Init(Config *config_ptr);

    void UnInit();

    // Schedule start fragments
    void Schedule(PlanFragment *plan_fragment_root, const BaseStatement *base_statement);

    // `plan_fragment` can be scheduled because all of its dependencies are met.
    void ScheduleFragment(PlanFragment *plan_fragment);

    void DumpPlanFragment(PlanFragment *plan_fragment);

private:
    u64 FindLeastWorkloadWorker();

    void ScheduleTask(FragmentTask *task, u64 worker_id);

    void RunTask(FragmentTask *task);

    void WorkerLoop(FragmentTaskBlockQueue *task_queue, i64 worker_id);

private:
    bool initialized_{false};

    Vector<Worker> worker_array_{};
    Deque<Atomic<u64>> worker_workloads_{};

    u64 worker_count_{0};
};

} // namespace hybridsearch
