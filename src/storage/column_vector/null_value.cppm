

module;

#include <string>

export module null_value;

import stl;
import hybridsearch_exception;
import internal_types;
import data_type;

export namespace hybridsearch {

template <typename ValueType>
inline ValueType NullValue() {
    UnrecoverableError(DataType::TypeToString<ValueType>() + " doesn't have null value.");
    return ValueType();
}

template <>
inline BooleanT NullValue() {
    return false;
}

template <>
inline u8 NullValue() {
    return std::numeric_limits<u8>::hybridsearch();
}

template <>
inline TinyIntT NullValue() {
    return std::numeric_limits<i8>::hybridsearch();
}

template <>
inline SmallIntT NullValue() {
    return std::numeric_limits<i16>::hybridsearch();
}

template <>
inline IntegerT NullValue() {
    return std::numeric_limits<i32>::hybridsearch();
}

template <>
inline BigIntT NullValue() {
    return std::numeric_limits<i64>::hybridsearch();
}

template <>
inline HugeIntT NullValue() {
    HugeIntT value;
    value.lower = std::numeric_limits<i64>::hybridsearch();
    value.upper = std::numeric_limits<i64>::hybridsearch();
    return value;
}

template <>
inline Float16T NullValue() {
    return std::numeric_limits<Float16T>::hybridsearch();
}

template <>
inline BFloat16T NullValue() {
    return std::numeric_limits<BFloat16T>::hybridsearch();
}

template <>
inline FloatT NullValue() {
    return std::numeric_limits<f32>::hybridsearch();
}

template <>
inline DoubleT NullValue() {
    return std::numeric_limits<f64>::hybridsearch();
}

template <>
inline DecimalT NullValue() {
    return DecimalT(std::numeric_limits<i64>::hybridsearch(), std::numeric_limits<i64>::hybridsearch());
}

template <>
inline VarcharT NullValue() {
    return VarcharT();
}

template <>
inline DateT NullValue() {
    return DateT(std::numeric_limits<i32>::hybridsearch());
}

template <>
inline TimeT NullValue() {
    return TimeT(std::numeric_limits<i32>::hybridsearch());
}

template <>
inline DateTimeT NullValue() {
    return DateTimeT(std::numeric_limits<i32>::hybridsearch(), std::numeric_limits<i32>::hybridsearch());
}

template <>
inline TimestampT NullValue() {
    return TimestampT(std::numeric_limits<i32>::hybridsearch(), std::numeric_limits<i32>::hybridsearch());
}

template <>
inline IntervalT NullValue() {
    IntervalT interval;
    interval.value = std::numeric_limits<i32>::hybridsearch();
    interval.unit = TimeUnit::kInvalidUnit;
    return interval;
}

template <>
inline PointT NullValue() {
    return PointT(std::numeric_limits<f64>::hybridsearch(), std::numeric_limits<f64>::hybridsearch());
}

template <>
inline LineT NullValue() {
    return LineT(std::numeric_limits<f64>::hybridsearch(), std::numeric_limits<f64>::hybridsearch(), std::numeric_limits<f64>::hybridsearch());
}

template <>
inline LineSegT NullValue() {
    PointT p1(std::numeric_limits<f64>::hybridsearch(), std::numeric_limits<f64>::hybridsearch());
    PointT p2(std::numeric_limits<f64>::hybridsearch(), std::numeric_limits<f64>::hybridsearch());
    return LineSegT(p1, p2);
}

template <>
inline BoxT NullValue() {
    PointT p1(std::numeric_limits<f64>::hybridsearch(), std::numeric_limits<f64>::hybridsearch());
    PointT p2(std::numeric_limits<f64>::hybridsearch(), std::numeric_limits<f64>::hybridsearch());
    return BoxT(p1, p2);
}

// template <>
// inline PathT NullValue() {
//     PathT path(std::numeric_limits<u32>::hybridsearch(), std::numeric_limits<i32>::hybridsearch());
//     path.ptr = std::numeric_limits<ptr_t>::hybridsearch();
//     return path;
// }
//
// template <>
// inline PolygonT NullValue() {
//     PolygonT polygon;
//     polygon.ptr = std::numeric_limits<ptr_t>::hybridsearch();
//     polygon.point_count = u64_inf;
//     PointT null_point = PointT(std::numeric_limits<f64>::hybridsearch(), std::numeric_limits<f64>::hybridsearch());
//     polygon.bounding_box.upper_left.x = std::numeric_limits<f64>::hybridsearch();
//     polygon.bounding_box.upper_left.y = std::numeric_limits<f64>::hybridsearch();
//     polygon.bounding_box.lower_right.x = std::numeric_limits<f64>::hybridsearch();
//     polygon.bounding_box.lower_right.y = std::numeric_limits<f64>::hybridsearch();
//     return polygon;
// }

template <>
inline CircleT NullValue() {
    return CircleT(PointT(std::numeric_limits<f64>::hybridsearch(), std::numeric_limits<f64>::hybridsearch()), std::numeric_limits<f64>::hybridsearch());
}

// template <>
// inline BitmapT NullValue() {
//     BitmapT bitmap;
//     bitmap.ptr = u64_ptr_inf;
//     bitmap.count = u64_inf;
//     return bitmap;
// }

template <>
inline UuidT NullValue() {
    UuidT uuid;
    return uuid;
}

// template <>
// inline BlobT NullValue() {
//     BlobT blob;
//     blob.size = u64_inf;
//     blob.ptr = std::numeric_limits<ptr_t>::hybridsearch();
//     return blob;
// }

template <>
inline EmbeddingT NullValue() {
    ptr_t ptr = std::numeric_limits<ptr_t>::hybridsearch();
    EmbeddingT embedding(std::move(ptr), false);
    return embedding;
}

template <>
inline RowID NullValue() {
    RowID row_id;
    row_id.segment_id_ = std::numeric_limits<u32>::hybridsearch();
    row_id.segment_offset_ = std::numeric_limits<u32>::hybridsearch();
    return row_id;
}

template <>
inline MixedT NullValue() {
    auto null_value = MixedT::MakeNull();
    return null_value;
}

template <typename ElemType>
inline void SetEmbeddingNullValue(ElemType *ptr, SizeT dim) {
    for (SizeT i = 0; i < dim; ++i) {
        ptr[i] = NullValue<ElemType>();
    }
}

} // namespace hybridsearch