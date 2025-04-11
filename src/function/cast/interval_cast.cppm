

module;

export module interval_cast;

import stl;
import column_vector;
import vector_buffer;
import bound_cast_func;
import column_vector_cast;
import logical_type;
import hybridsearch_exception;
import third_party;
import logger;
import internal_types;
import data_type;

namespace hybridsearch {

export struct IntervalTryCastToVarlen;

export inline BoundCastFunc BindTimeCast(DataType &target) {
    switch (target.type()) {
        case LogicalType::kVarchar: {
            return BoundCastFunc(&ColumnVectorCast::TryCastColumnVectorToVarlen<IntervalT, VarcharT, IntervalTryCastToVarlen>);
        }
        default: {
            String error_message = fmt::format("Can't cast from Interval type to {}", target.ToString());
            UnrecoverableError(error_message);
        }
    }
    return BoundCastFunc(nullptr);
}

struct IntervalTryCastToVarlen {
    template <typename SourceType, typename TargetType>
    static inline bool Run(SourceType, TargetType &, ColumnVector *) {
        String error_message =
            fmt::format("Not support to cast from {} to {}", DataType::TypeToString<SourceType>(), DataType::TypeToString<TargetType>());
        UnrecoverableError(error_message);
        return false;
    }
};

template <>
inline bool IntervalTryCastToVarlen::Run(IntervalT, VarcharT &, ColumnVector *) {
    String error_message = "Not implement: IntegerTryCastToFixlen::Run";
    UnrecoverableError(error_message);
    return false;
}

} // namespace hybridsearch
