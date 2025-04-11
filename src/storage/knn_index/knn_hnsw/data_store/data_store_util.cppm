

module;

export module data_store_util;

import stl;

namespace hybridsearch {

export template <typename T, bool OwnMem>
class ArrayPtr {
public:
    ArrayPtr() = default;
    ArrayPtr(UniquePtr<T[]> ptr) : ptr_(std::move(ptr)) {}

    T &operator[](SizeT idx) { return ptr_[idx]; }
    const T &operator[](SizeT idx) const { return ptr_[idx]; }

    T *get() const { return ptr_.get(); }

    UniquePtr<T[]> exchange(UniquePtr<T[]> ptr) { return std::exchange(ptr_, std::move(ptr)); }

private:
    UniquePtr<T[]> ptr_;
};

export template <typename T>
class ArrayPtr<T, false> {
public:
    ArrayPtr() = default;
    ArrayPtr(const T *ptr) : ptr_(ptr) {}

    const T &operator[](SizeT idx) const { return ptr_[idx]; }

    const T *get() const { return ptr_; }

private:
    const T *ptr_ = nullptr;
};

export template <bool OwnMem>
class PPtr {
public:
    PPtr() = default;
    void set(char *ptr) { ptr_ = ptr; }
    char *get() const { return ptr_; }

private:
    char *ptr_;
};

export template <>
class PPtr<false> {
public:
    PPtr() = default;
    void set(const char *ptr) { ptr_ = ptr; }
    const char *get() const { return ptr_; }

private:
    const char *ptr_ = nullptr;
};

} // namespace hybridsearch
