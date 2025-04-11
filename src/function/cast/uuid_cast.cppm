

module;

export module uuid_cast;

import stl;
import column_vector;
import vector_buffer;
import bound_cast_func;
import data_type;
import column_vector_cast;
import logical_type;
import hybridsearch_exception;
import third_party;
import internal_types;
import status;
import logger;

namespace hybridsearch {

export struct UuidTryCastToVarlen;

export inline BoundCastFunc BindUuidCast(DataType &target) {
    switch (target.type()) {
        case LogicalType::kVarchar: {
            return BoundCastFunc(&ColumnVectorCast::TryCastColumnVectorToVarlen<UuidT, VarcharT, UuidTryCastToVarlen>);
        }
        default: {
            String error_message = fmt::format("Can't cast from Uuid type to {}", target.ToString());
            UnrecoverableError(error_message);
        }
    }
    return BoundCastFunc(nullptr);
}

struct UuidTryCastToVarlen {
    template <typename SourceType, typename TargetType>
    static inline bool Run(const SourceType &, TargetType &, ColumnVector *) {
        UnrecoverableError(
            fmt::format("Not support to cast from {} to {}", DataType::TypeToString<SourceType>(), DataType::TypeToString<TargetType>()));
        return false;
    }
};

template <>
inline bool UuidTryCastToVarlen::Run(const UuidT &, VarcharT &, ColumnVector *) {
    Status status = Status::NotSupport("Not implemented");
    RecoverableError(status);
    //    target.length_ = UuidT::LENGTH;
    //    std::memcpy(target.prefix, source.body, VarcharT::PREFIX_LENGTH);
    //    Assert<UnrecoverableException>(vector_ptr->buffer_->buffer_type_ == VectorBufferType::kHeap,
    //                          "Varchar column vector should use MemoryVectorBuffer.");
    //
    //    ptr_t ptr = vector_ptr->buffer_->fix_heap_mgr_->Allocate(target.length);
    //    std::memcpy(ptr, source.body, target.length);
    //    target.ptr = ptr;

    return true;
}

} // namespace hybridsearch
