

#pragma once

#include <atomic>
#include <mutex>
#include <string>
#include <unordered_map>

namespace hybridsearch {

class GlobalResourceUsage {
public:
    static inline void Init() {
#ifdef hybridsearch_STATS
        if (initialized_) {
            return;
        }
        object_count_ = 0;
        raw_memory_count_ = 0;
        initialized_ = true;
#endif
    }

    static inline void UnInit() {
#ifdef hybridsearch_STATS
        if (initialized_) {
            object_count_ = 0;
            raw_memory_count_ = 0;
            initialized_ = false;
        }
#endif
    }

    static inline void IncrObjectCount(const std::string &key) {
#ifdef hybridsearch_STATS
        std::unique_lock<std::mutex> unique_locker(object_mutex_);
        ++object_map_[key];
        ++object_count_;
#endif
    }

    static void DecrObjectCount(const std::string &key) {
#ifdef hybridsearch_STATS
        std::unique_lock<std::mutex> unique_locker(object_mutex_);
        --object_map_[key];
        --object_count_;
#endif
    }

    static int64_t GetObjectCount() {
#ifdef hybridsearch_STATS
        std::unique_lock<std::mutex> unique_locker(object_mutex_);
        return object_count_;
#else
        return 0;
#endif
    }

    static std::string GetObjectCountInfo() {
#ifdef hybridsearch_STATS
        std::unique_lock<std::mutex> unique_locker(object_mutex_);
        return std::to_string(object_count_);
#else
        return "Not activate";
#endif
    }

    static int64_t GetObjectCount(const std::string &key) {
#ifdef hybridsearch_STATS
        std::unique_lock<std::mutex> unique_locker(object_mutex_);
        return object_map_[key];
#else
        return 0;
#endif
    }

    static std::unordered_map<std::string, int64_t> GetObjectClones() {
#ifdef hybridsearch_STATS
        std::unique_lock<std::mutex> unique_locker(object_mutex_);
        return object_map_;
#else
        return std::unordered_map<std::string, int64_t>();
#endif
    }

    static void IncrRawMemCount(const std::string &key) {
#ifdef hybridsearch_STATS
        std::unique_lock<std::mutex> unique_locker(raw_memory_mutex_);
        ++raw_memory_count_;
        ++raw_memory_map_[key];
#endif
    }

    static void DecrRawMemCount(const std::string &key) {
#ifdef hybridsearch_STATS
        std::unique_lock<std::mutex> unique_locker(raw_memory_mutex_);
        --raw_memory_count_;
        --raw_memory_map_[key];
#endif
    }

    static int64_t GetRawMemoryCount() {
#ifdef hybridsearch_STATS
        std::unique_lock<std::mutex> unique_locker(raw_memory_mutex_);
        return raw_memory_count_;
#else
        return 0;
#endif
    }

    static int64_t GetRawMemoryCount(const std::string &key) {
#ifdef hybridsearch_STATS
        std::unique_lock<std::mutex> unique_locker(raw_memory_mutex_);
        return raw_memory_map_[key];
#else
        return 0;
#endif
    }

    static std::string GetRawMemoryInfo() {
#ifdef hybridsearch_STATS
        std::unique_lock<std::mutex> unique_locker(raw_memory_mutex_);
        return "allocate count: " + std::to_string(raw_memory_map_.size()) + ", total_size: " + std::to_string(raw_memory_count_);
#else
        return "Not activate";
#endif
    }

    static std::unordered_map<std::string, int64_t> GetRawMemoryClone() {
#ifdef hybridsearch_STATS
        std::unique_lock<std::mutex> unique_locker(raw_memory_mutex_);
        return raw_memory_map_;
#else
        return std::unordered_map<std::string, int64_t>();
#endif
    }

private:
    static std::atomic_bool initialized_;

    static std::mutex object_mutex_;
    static int64_t object_count_;
    static std::unordered_map<std::string, int64_t> object_map_;

    static std::mutex raw_memory_mutex_;
    static int64_t raw_memory_count_;
    static std::unordered_map<std::string, int64_t> raw_memory_map_;
};

} // namespace hybridsearch
