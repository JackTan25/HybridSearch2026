

module;

export module emvb_shared_vec;
import stl;

namespace hybridsearch {

// append only
// reallocation will not affect the existing shared pointers for reading old data
export template <typename T>
class EMVBSharedVec {
    mutable std::shared_mutex rw_mutex_; // mutex for read-write operations
    SharedPtr<T[]> data_;
    u32 capacity_ = 0;
    u32 size_ = 0;

    void ReserveUnderLock(u32 new_capacity);
    void ReserveUnderLockForNewSize(u32 new_size) {
        if (new_size <= capacity_) {
            return;
        }
        const u32 new_capacity = new_size + (new_size / 2);
        ReserveUnderLock(new_capacity);
    }

public:
    EMVBSharedVec &operator=(EMVBSharedVec &&other) noexcept {
        if (this != &other) {
            std::scoped_lock lock(rw_mutex_, other.rw_mutex_);
            data_ = std::move(other.data_);
            capacity_ = other.capacity_;
            size_ = other.size_;
            other.data_.reset();
            other.capacity_ = 0;
            other.size_ = 0;
        }
        return *this;
    }

    [[nodiscard]] Pair<SharedPtr<T[]>, u32> GetData() const {
        std::shared_lock lock(rw_mutex_);
        return {data_, size_};
    }
    void Reserve(u32 new_capacity) {
        std::unique_lock lock(rw_mutex_);
        ReserveUnderLock(new_capacity);
    }
    void PushBack(auto &&val) {
        std::unique_lock lock(rw_mutex_);
        ReserveUnderLockForNewSize(size_ + 1);
        data_[size_++] = std::forward<decltype(val)>(val);
    }
    void PushBackIfDifferentFromLast(auto &&val) {
        std::unique_lock lock(rw_mutex_);
        if (size_ > 0 and data_[size_ - 1] == val) {
            return;
        }
        ReserveUnderLockForNewSize(size_ + 1);
        data_[size_++] = std::forward<decltype(val)>(val);
    }
    void PushBack(auto IterBegin, auto IterEnd) {
        std::unique_lock lock(rw_mutex_);
        const u32 append_size = std::distance(IterBegin, IterEnd);
        const u32 new_size = size_ + append_size;
        ReserveUnderLockForNewSize(new_size);
        std::copy_n(IterBegin, append_size, data_.get() + size_);
        size_ = new_size;
    }
};

} // namespace hybridsearch
