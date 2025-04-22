

#include "global_resource_usage.h"

namespace hybridsearch {

std::atomic_bool GlobalResourceUsage::initialized_ = false;

int64_t GlobalResourceUsage::object_count_ = 0;
std::unordered_map<std::string, int64_t> GlobalResourceUsage::object_map_;

int64_t GlobalResourceUsage::raw_memory_count_ = 0;
std::unordered_map<std::string, int64_t> GlobalResourceUsage::raw_memory_map_;

std::mutex GlobalResourceUsage::object_mutex_{};

std::mutex GlobalResourceUsage::raw_memory_mutex_{};

} // namespace hybridsearch
