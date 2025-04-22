

module;

module emvb_shared_vec;
import stl;

namespace hybridsearch {

// It seems that 'MakeShared' cannot be put into the cppm file.
template <typename T>
void EMVBSharedVec<T>::ReserveUnderLock(u32 new_capacity) {
    if (new_capacity <= capacity_) {
        return;
    }
    auto new_data = MakeShared<T[]>(new_capacity);
    std::copy_n(data_.get(), size_, new_data.get());
    data_ = std::move(new_data);
    capacity_ = new_capacity;
}

template class EMVBSharedVec<u32>;

} // namespace hybridsearch
