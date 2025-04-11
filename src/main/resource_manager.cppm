

module;

export module resource_manager;

import singleton;
import stl;
import global_resource_usage;

namespace hybridsearch {

export class ResourceManager : public Singleton<ResourceManager> {
public:
    explicit ResourceManager(u64 total_cpu_count, u64 total_memory)
        : total_cpu_count_(total_cpu_count), total_memory_(total_memory), hardware_concurrency_(Thread::hardware_concurrency()) {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::IncrObjectCount("ResourceManager");
#endif
    }

    ~ResourceManager() {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::DecrObjectCount("ResourceManager");
#endif
    }

    inline u64 GetCpuResource(u64 cpu_count) {
        total_cpu_count_ -= cpu_count;
        return cpu_count;
    }

    inline u64 GetCpuResource() { return GetCpuResource(hardware_concurrency_); }
    // inline u64 GetCpuResource() { return GetCpuResource(4); }

    inline u64 GetMemoryResource(u64 memory_size) {
        total_memory_ -= memory_size;
        return memory_size;
    }

    inline u64 GetMemoryResource() {
        // Get 16MB each time
        return GetMemoryResource(16 * 1024 * 1024);
    }

private:
    atomic_u64 total_cpu_count_;
    atomic_u64 total_memory_;
    u64 hardware_concurrency_;
};

} // namespace hybridsearch
