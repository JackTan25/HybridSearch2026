

module;

import blocking_queue;
import stl;
import object_storage_task;
import global_resource_usage;

export module object_storage_process;

namespace hybridsearch {

export class ObjectStorageProcess {
public:
    explicit ObjectStorageProcess() {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::IncrObjectCount("ObjectStorageProcess");
#endif
    }

    ~ObjectStorageProcess() {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::DecrObjectCount("ObjectStorageProcess");
#endif
    }

    void Start();
    void Stop();
    u64 RunningTaskCount() const { return task_count_; }

public:
    void Submit(SharedPtr<BaseObjectStorageTask> object_storage_task);

private:
    void Process();

private:
    BlockingQueue<SharedPtr<BaseObjectStorageTask>> task_queue_{"ObjectStorageProcess"};

    Thread processor_thread_{};

    Atomic<u64> task_count_{};

    mutable std::mutex task_mutex_;
    String task_text_;
};

} // namespace hybridsearch
