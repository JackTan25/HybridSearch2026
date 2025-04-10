
module;

export module selection;

import stl;
import logger;
import hybridsearch_exception;
import global_resource_usage;
import default_values;

namespace hybridsearch {

struct SelectionData {
    explicit SelectionData(SizeT count) : capacity_(count) {
        if (count > std::numeric_limits<u16>::max()) {
            String error_message = "Too large size for selection data.";
            UnrecoverableError(error_message);
        }
        data_ = MakeUnique<u16[]>(count);
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::IncrObjectCount("SelectionData");
#endif
    }

    ~SelectionData() {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::DecrObjectCount("SelectionData");
#endif
    }

    UniquePtr<u16[]> data_{};
    SizeT capacity_{};
};

export class Selection {
public:
    Selection() {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::IncrObjectCount("Selection");
#endif
    }

    ~Selection() {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::DecrObjectCount("Selection");
#endif
    }

    void Initialize(SizeT count = DEFAULT_VECTOR_SIZE) {
        storage_ = MakeShared<SelectionData>(count);
        selection_vector = storage_->data_.get();
    }

    inline void Set(SizeT selection_idx, SizeT row_idx) {
        if (selection_vector == nullptr) {
            String error_message = "Selection container isn't initialized";
            UnrecoverableError(error_message);
        }
        if (selection_idx >= storage_->capacity_) {
            String error_message = "Exceed the selection vector capacity.";
            UnrecoverableError(error_message);
        }
        selection_vector[selection_idx] = row_idx;
    }

    inline void Append(SizeT row_idx) {
        Set(latest_selection_idx_, row_idx);
        ++latest_selection_idx_;
    }

    inline SizeT Get(SizeT idx) const {
        if (selection_vector == nullptr) {
            return idx;
        }
        if (idx >= latest_selection_idx_) {
            String error_message = "Exceed the last row of the selection vector.";
            UnrecoverableError(error_message);
        }
        return selection_vector[idx];
    }

    inline u16 &operator[](SizeT idx) const {
        if (idx >= latest_selection_idx_) {
            String error_message = "Exceed the last row of the selection vector.";
            UnrecoverableError(error_message);
        }
        return selection_vector[idx];
    }

    inline SizeT Capacity() const {
        if (selection_vector == nullptr) {
            String error_message = "Selection container isn't initialized";
            UnrecoverableError(error_message);
        }
        return storage_->capacity_;
    }

    inline SizeT Size() const {
        if (selection_vector == nullptr) {
            String error_message = "Selection container isn't initialized";
            UnrecoverableError(error_message);
        }
        return latest_selection_idx_;
    }

    void Reset() {
        storage_.reset();
        latest_selection_idx_ = 0;
        selection_vector = nullptr;
    }

private:
    SizeT latest_selection_idx_{};
    u16 *selection_vector{};
    SharedPtr<SelectionData> storage_{};
};

} // namespace hybridsearch
