

module;

export module date_cast;

import stl;
import column_vector;
import vector_buffer;
import bound_cast_func;
import column_vector_cast;
import logical_type;
import hybridsearch_exception;
import third_party;
import internal_types;
import data_type;
import logger;

namespace hybridsearch {

export struct DateTryCastToFixlen;
export struct DateTryCastToVarlen;

export inline BoundCastFunc BindDateCast(DataType &target) {
    switch (target.type()) {
        case LogicalType::kDateTime: {
            return BoundCastFunc(&ColumnVectorCast::TryCastColumnVector<DateT, DateTimeT, DateTryCastToFixlen>);
        }
        case LogicalType::kTimestamp: {
            return BoundCastFunc(&ColumnVectorCast::TryCastColumnVector<DateT, TimestampT, DateTryCastToFixlen>);
        }
        case LogicalType::kVarchar: {
            return BoundCastFunc(&ColumnVectorCast::TryCastColumnVectorToVarlen<DateT, VarcharT, DateTryCastToVarlen>);
        }
        default: {
            String error_message = fmt::format("Can't cast from Date type to {}", target.ToString());
            UnrecoverableError(error_message);
        }
    }
    return BoundCastFunc(nullptr);
}

struct DateTryCastToFixlen {
    template <typename SourceType, typename TargetType>
    static inline bool Run(SourceType, TargetType &) {
        String error_message =
            fmt::format("Not support to cast from {} to {}", DataType::TypeToString<SourceType>(), DataType::TypeToString<TargetType>());
        UnrecoverableError(error_message);
        return false;
    }
};

struct DateTryCastToVarlen {
    template <typename SourceType, typename TargetType>
    static inline bool Run(SourceType, TargetType &, ColumnVector *) {
        String error_message =
            fmt::format("Not support to cast from {} to {}", DataType::TypeToString<SourceType>(), DataType::TypeToString<TargetType>());
        UnrecoverableError(error_message);
        return false;
    }
};

template <>
inline bool DateTryCastToFixlen::Run(DateT, DateTimeT &) {
    String error_message = "Not implemented";
    UnrecoverableError(error_message);
    return false;
}

template <>
inline bool DateTryCastToFixlen::Run(DateT, TimestampT &) {
    String error_message = "Not implemented";
    UnrecoverableError(error_message);
    return false;
}

template <>
inline bool DateTryCastToVarlen::Run(DateT, VarcharT &, ColumnVector *) {
    String error_message = "Not implemented";
    UnrecoverableError(error_message);
    return false;
}

} // namespace hybridsearch
