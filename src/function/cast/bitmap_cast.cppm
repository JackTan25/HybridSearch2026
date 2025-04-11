

module;

import stl;

import column_vector_cast;

import hybridsearch_exception;
import bound_cast_func;
import column_vector;
import third_party;

export module bitmap_cast;
#if 0
namespace hybridsearch {

export struct BitmapTryCastToVarlen;

export inline BoundCastFunc BindBitmapCast(DataType &target) {
    switch (target.type()) {
        case LogicalType::kVarchar: {
            return BoundCastFunc(&ColumnVectorCast::TryCastColumnVectorToVarlen<BitmapT, VarcharT, BitmapTryCastToVarlen>);
        }
        default: {
            String error_message = fmt::format("Can't cast from Time type to {}", target.ToString());
            UnrecoverableError(error_message);
        }
    }
    return BoundCastFunc(nullptr);
}

struct BitmapTryCastToVarlen {
    template <typename SourceType, typename TargetType>
    static inline bool Run(const SourceType &source, TargetType &target, const SharedPtr<ColumnVector> &vector_ptr) {
        String error_message = "Not support to cast from " + DataType::TypeToString<SourceType>() + " to " + DataType::TypeToString<TargetType>();
        UnrecoverableError(error_message);
        return false;
    }
};

template <>
inline bool BitmapTryCastToVarlen::Run(const BitmapT &source, VarcharT &target, const SharedPtr<ColumnVector> &vector_ptr) {
    RecoverableError(Status::NotSupport("Not implemented"));
    return false;
}

} // namespace hybridsearch
#endif