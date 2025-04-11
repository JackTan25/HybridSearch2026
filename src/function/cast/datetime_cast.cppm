

module;

export module datetime_cast;

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
import logger;

namespace hybridsearch {

export struct DateTimeTryCastToFixlen;
export struct DateTimeTryCastToVarlen;

export inline BoundCastFunc BindDateTimeCast(DataType &target) {
    switch (target.type()) {
        case LogicalType::kDate: {
            return BoundCastFunc(&ColumnVectorCast::TryCastColumnVector<DateTimeT, DateT, DateTimeTryCastToFixlen>);
        }
        case LogicalType::kTime: {
            return BoundCastFunc(&ColumnVectorCast::TryCastColumnVector<DateTimeT, TimeT, DateTimeTryCastToFixlen>);
        }
        case LogicalType::kTimestamp: {
            return BoundCastFunc(&ColumnVectorCast::TryCastColumnVector<DateTimeT, TimestampT, DateTimeTryCastToFixlen>);
        }
        case LogicalType::kVarchar: {
            return BoundCastFunc(&ColumnVectorCast::TryCastColumnVectorToVarlen<DateTimeT, VarcharT, DateTimeTryCastToVarlen>);
        }
        default: {
            String error_message = fmt::format("Can't cast from DateTime type to {}", target.ToString());
            UnrecoverableError(error_message);
        }
    }
    return BoundCastFunc(nullptr);
}

struct DateTimeTryCastToFixlen {
    template <typename SourceType, typename TargetType>
    static inline bool Run(SourceType, TargetType &) {
        String error_message =
            fmt::format("Not support to cast from {} to {}", DataType::TypeToString<SourceType>(), DataType::TypeToString<TargetType>());
        UnrecoverableError(error_message);

        return false;
    }
};

struct DateTimeTryCastToVarlen {
    template <typename SourceType, typename TargetType>
    static inline bool Run(SourceType, TargetType &, ColumnVector *) {
        String error_message =
            fmt::format("Not support to cast from {} to {}", DataType::TypeToString<SourceType>(), DataType::TypeToString<TargetType>());
        UnrecoverableError(error_message);
        return false;
    }
};

template <>
inline bool DateTimeTryCastToFixlen::Run(DateTimeT, DateT &) {
    String error_message = "Not implemented";
    UnrecoverableError(error_message);
    return false;
}

template <>
inline bool DateTimeTryCastToFixlen::Run(DateTimeT, TimeT &) {
    String error_message = "Not implemented";
    UnrecoverableError(error_message);
    return false;
}

template <>
inline bool DateTimeTryCastToFixlen::Run(DateTimeT, TimestampT &) {
    String error_message = "Not implemented";
    UnrecoverableError(error_message);
    return false;
}

template <>
inline bool DateTimeTryCastToVarlen::Run(DateTimeT, VarcharT &, ColumnVector *) {
    String error_message = "Not implemented";
    UnrecoverableError(error_message);
    return false;
}

} // namespace hybridsearch
