

module;

export module vector_with_lock;
import stl;

namespace hybridsearch {

export template <typename ValueType>
class VectorWithLock {
private:
    std::shared_mutex mutex_;
    Vector<ValueType> vec_;

public:
    VectorWithLock() = default;
    VectorWithLock(SizeT count) : vec_(count) {}
    VectorWithLock(SizeT count, const ValueType &value) : vec_(count, value) {}

    ~VectorWithLock() = default;

    void Resize(SizeT new_size) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        vec_.resize(new_size);
    }

    ValueType Get(SizeT i) {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        return vec_[i];
    }

    ValueType Sum() {
        ValueType sum = 0;
        std::shared_lock<std::shared_mutex> lock(mutex_);
        for (SizeT i = 0; i < vec_.size(); ++i)
            sum += vec_[i];
        return sum;
    }

    void Set(SizeT i, const ValueType &value) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        vec_[i] = value;
    }

    void SetBatch(SizeT begin_idx, const Vector<ValueType> &values) {
        SizeT required_size = begin_idx + values.size();
        std::unique_lock<std::shared_mutex> lock(mutex_);
        if (vec_.size() < required_size)
            vec_.resize(required_size);
        for (SizeT i = 0; i < values.size(); ++i)
            vec_[begin_idx + i] = values[i];
    }

    Vector<ValueType> &UnsafeVec() { return vec_; }

    void Clear() {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        vec_.clear();
    }
};
} // namespace hybridsearch
