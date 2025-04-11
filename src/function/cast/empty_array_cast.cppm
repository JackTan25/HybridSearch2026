

module;

export module empty_array_cast;

import stl;
import data_type;
import bound_cast_func;
import column_vector_cast;
import hybridsearch_exception;
import internal_types;
import logical_type;
import third_party;

namespace hybridsearch {

struct EmptyTryCastToFixlen;

export inline BoundCastFunc BindEmptyArrayCast(const DataType &source, const DataType &target) {
    switch (target.type()) {
        case LogicalType::kSparse: {
            return BoundCastFunc(&ColumnVectorCast::TryCastColumnVector<EmptyArrayT, SparseT, EmptyTryCastToFixlen>);
        }
        default: {
            UnrecoverableError("Not implemented");
        }
    }
    return BoundCastFunc(nullptr);
}

struct EmptyTryCastToFixlen {
    template <typename SourceType, typename TargetType>
    static bool Run(SourceType, TargetType &) {
        UnrecoverableError(
            fmt::format("Not support to cast from {} to {}", DataType::TypeToString<SourceType>(), DataType::TypeToString<TargetType>()));
        return false;
    }
};

template <>
bool EmptyTryCastToFixlen::Run(EmptyArrayT, SparseT &target) {
    target.nnz_ = 0;
    return true;
}

} // namespace hybridsearch