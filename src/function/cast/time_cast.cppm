

module;

export module time_cast;

import stl;
import column_vector_cast;
import logical_type;
import hybridsearch_exception;
import bound_cast_func;
import column_vector;
import third_party;
import internal_types;
import data_type;
import logger;

namespace hybridsearch {

export struct TimeTryCastToVarlen;

export inline BoundCastFunc BindTimeCast(DataType &target) {
    switch (target.type()) {
        case LogicalType::kVarchar: {
            return BoundCastFunc(&ColumnVectorCast::TryCastColumnVectorToVarlen<TimeT, VarcharT, TimeTryCastToVarlen>);
        }
        default: {
            String error_message = fmt::format("Can't cast from Time type to  {}", target.ToString());
            UnrecoverableError(error_message);
        }
    }
    return BoundCastFunc(nullptr);
}

struct TimeTryCastToVarlen {
    template <typename SourceType, typename TargetType>
    static inline bool Run(SourceType, TargetType &, ColumnVector *) {
        String error_message =
            fmt::format("Not support to cast from {} to {}", DataType::TypeToString<SourceType>(), DataType::TypeToString<TargetType>());
        UnrecoverableError(error_message);
        return false;
    }
};

template <>
inline bool TimeTryCastToVarlen::Run(TimeT, VarcharT &, ColumnVector *) {
    String error_message = "Not implement: IntegerTryCastToFixlen::Run";
    UnrecoverableError(error_message);
    return false;
}

} // namespace hybridsearch
