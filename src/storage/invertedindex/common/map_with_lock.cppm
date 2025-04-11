

module;

export module map_with_lock;
import stl;

namespace hybridsearch {

export template <typename KeyType, typename ValueType>
class MapWithLock {
private:
    std::shared_mutex mutex_;
    Map<KeyType, ValueType> map_;

public:
    MapWithLock() = default;

    ~MapWithLock() = default;

    bool Get(const KeyType &key, ValueType &value) {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        auto it = map_.find(key);
        if (it == map_.end()) {
            return false;
        }
        value = it->second;
        return true;
    }

    // Get or add a value to the map.
    // Returns true if found.
    // Returns false if not found, and add the key-value pair into the map.
    bool GetOrAdd(const KeyType &key, ValueType &value, const ValueType &new_value) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        auto result = map_.emplace(key, new_value);
        if (result.second) {
            // added
            value = new_value;
            return false;
        } else {
            // found
            value = result.first->second;
            return true;
        }
    }

    void Clear() {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        map_.clear();
    }

    void Range(const KeyType &key_min, const KeyType &key_max, Vector<Pair<KeyType, ValueType>> &items) {
        items.clear();
        std::shared_lock<std::shared_mutex> lock(mutex_);
        auto it1 = map_.lower_bound(key_min);
        auto it2 = map_.upper_bound(key_max);
        while (it1 != it2) {
            items.push_back(MakePair(it1->first, it1->second));
            ++it1;
        }
    }

    // WARN: Caller shall ensure there's no concurrent write access
    Map<KeyType, ValueType>::iterator UnsafeBegin() { return map_.begin(); }

    // WARN: Caller shall ensure there's no concurrent write access
    Map<KeyType, ValueType>::iterator UnsafeEnd() { return map_.end(); }
};
} // namespace hybridsearch
