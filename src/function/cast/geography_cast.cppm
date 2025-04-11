

module;

export module geography_cast;

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

export struct GeographyTryCastToVarlen;

export template <class SourceType>
inline BoundCastFunc BindGeographyCast(const DataType &source, DataType &target) {
    if (source.type() == target.type()) {
        String error_message = "Can't cast from the same type";
        UnrecoverableError(error_message);
    }
    switch (target.type()) {
        case LogicalType::kVarchar: {
            return BoundCastFunc(&ColumnVectorCast::TryCastColumnVectorToVarlen<SourceType, VarcharT, GeographyTryCastToVarlen>);
        }
        default: {
            String error_message = fmt::format("Can't cast from geography type to {}", target.ToString());
            UnrecoverableError(error_message);
        }
    }
    return BoundCastFunc(nullptr);
}

struct GeographyTryCastToVarlen {
    template <typename SourceType, typename TargetType>
    static inline bool Run(const SourceType &, TargetType &, ColumnVector *) {
        String error_message =
            fmt::format("Not support to cast from {} to {}", DataType::TypeToString<SourceType>(), DataType::TypeToString<TargetType>());
        UnrecoverableError(error_message);
        return false;
    }
};

template <>
inline bool GeographyTryCastToVarlen::Run(const PointT &, VarcharT &, ColumnVector *) {
    String error_message = "Not implement: GeographyTryCastToVarlen::Run";
    UnrecoverableError(error_message);
    return false;
}

template <>
inline bool GeographyTryCastToVarlen::Run(const LineT &, VarcharT &, ColumnVector *) {
    String error_message = "Not implement: GeographyTryCastToVarlen::Run";
    UnrecoverableError(error_message);
    return false;
}

template <>
inline bool GeographyTryCastToVarlen::Run(const LineSegT &, VarcharT &, ColumnVector *) {
    String error_message = "Not implement: GeographyTryCastToVarlen::Run";
    UnrecoverableError(error_message);
    return false;
}

template <>
inline bool GeographyTryCastToVarlen::Run(const BoxT &, VarcharT &, ColumnVector *) {
    String error_message = "Not implement: GeographyTryCastToVarlen::Run";
    UnrecoverableError(error_message);
    return false;
}
#if 0
template <>
inline bool GeographyTryCastToVarlen::Run(const PathT &source, VarcharT &target, const SharedPtr<ColumnVector> &vector_ptr) {
    String error_message = "Not implement: GeographyTryCastToVarlen::Run";
    UnrecoverableError(error_message);
    return false;
}

template <>
inline bool GeographyTryCastToVarlen::Run(const PolygonT &source, VarcharT &target, const SharedPtr<ColumnVector> &vector_ptr) {
    String error_message = "Not implement: GeographyTryCastToVarlen::Run";
    UnrecoverableError(error_message);
    return false;
}
#endif
template <>
inline bool GeographyTryCastToVarlen::Run(const CircleT &, VarcharT &, ColumnVector *) {
    String error_message = "Not implement: GeographyTryCastToVarlen::Run";
    UnrecoverableError(error_message);
    return false;
}

} // namespace hybridsearch
