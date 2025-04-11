

module;

export module fragment_data;

import stl;
import data_block;
import status;

namespace hybridsearch {

export enum class FragmentDataType {
    kData,
    kNone,
    kError,
    kInvalid,
};

export struct FragmentDataBase {
    FragmentDataType type_{FragmentDataType::kInvalid};
    u64 fragment_id_{std::numeric_limits<u64>::max()};

    FragmentDataBase(FragmentDataType type, u64 fragment_id) : type_(type), fragment_id_(fragment_id) {}
};

export struct FragmentError : public FragmentDataBase {
    //    UniquePtr<String> error_message_{};
    Status status_{};

    FragmentError(u64 fragment_id, Status status) : FragmentDataBase(FragmentDataType::kError, fragment_id), status_(std::move(status)) {}
};

export struct FragmentData : public FragmentDataBase {
    UniquePtr<DataBlock> data_block_{};
    i64 task_id_{-1};
    Optional<SizeT> data_idx_{};
    SizeT data_count_{std::numeric_limits<u64>::max()};
    bool is_last_{false};
    bool total_hits_count_flag_{false};
    SizeT total_hits_count_{};

    FragmentData(u64 fragment_id,
                 UniquePtr<DataBlock> data_block,
                 i64 task_id,
                 SizeT data_idx,
                 SizeT data_count,
                 bool is_last,
                 bool total_hits_count_flag,
                 SizeT total_hits_count)
        : FragmentDataBase(FragmentDataType::kData, fragment_id), data_block_(std::move(data_block)), task_id_(task_id), data_idx_(data_idx),
          data_count_(data_count), is_last_(is_last), total_hits_count_flag_(total_hits_count_flag), total_hits_count_(total_hits_count) {}
};

export struct FragmentNone : public FragmentDataBase {
    FragmentNone(u64 fragment_id) : FragmentDataBase(FragmentDataType::kNone, fragment_id) {}
};

} // namespace hybridsearch
