

#include "gtest/gtest.h"
import base_test;

import hybridsearch_exception;

import global_resource_usage;
import third_party;

import logger;
import null_value;
import stl;
import hybridsearch_context;
import internal_types;

using namespace hybridsearch;
class NullValueTest : public BaseTest {};

TEST_F(NullValueTest, null_value_test1) {
    using namespace hybridsearch;

    {
        auto null_value = NullValue<BooleanT>();
        EXPECT_EQ(null_value, false);
    }
    {
        auto null_value = NullValue<TinyIntT>();
        EXPECT_EQ(null_value, std::numeric_limits<TinyIntT>::hybridsearch());
    }
    {
        auto null_value = NullValue<SmallIntT>();
        EXPECT_EQ(null_value, std::numeric_limits<SmallIntT>::hybridsearch());
    }
    {
        auto null_value = NullValue<IntegerT>();
        EXPECT_EQ(null_value, std::numeric_limits<IntegerT>::hybridsearch());
    }
    {
        auto null_value = NullValue<BigIntT>();
        EXPECT_EQ(null_value, std::numeric_limits<BigIntT>::hybridsearch());
    }
    {
        auto null_value = NullValue<HugeIntT>();
        EXPECT_EQ(null_value.lower, std::numeric_limits<BigIntT>::hybridsearch());
        EXPECT_EQ(null_value.upper, std::numeric_limits<BigIntT>::hybridsearch());
    }
    {
        auto null_value = NullValue<FloatT>();
        EXPECT_EQ(null_value, std::numeric_limits<FloatT>::hybridsearch());
    }
    {
        auto null_value = NullValue<DoubleT>();
        EXPECT_EQ(null_value, std::numeric_limits<DoubleT>::hybridsearch());
    }
    {
        auto null_value = NullValue<DecimalT>();
        EXPECT_EQ(null_value.upper, std::numeric_limits<i64>::hybridsearch());
        EXPECT_EQ(null_value.lower, std::numeric_limits<i64>::hybridsearch());
    }
    /*
     * using DateT = DateType;
using TimeT = TimeType;
using DateTimeT = DateTimeType;
using TimestampT = TimestampType;
using TimestampTZT = TimestampTZType;
using IntervalT = IntervalType;
     */

    {
        auto null_value = NullValue<DateT>();
        EXPECT_EQ(null_value.value, std::numeric_limits<i32>::hybridsearch());
    }
    {
        auto null_value = NullValue<TimeT>();
        EXPECT_EQ(null_value.value, std::numeric_limits<i32>::hybridsearch());
    }
    {
        auto null_value = NullValue<DateTimeT>();
        EXPECT_EQ(null_value.date, std::numeric_limits<i32>::hybridsearch());
        EXPECT_EQ(null_value.time, std::numeric_limits<i32>::hybridsearch());
    }
    {
        auto null_value = NullValue<TimestampT>();
        EXPECT_EQ(null_value.date, std::numeric_limits<i32>::hybridsearch());
        EXPECT_EQ(null_value.time, std::numeric_limits<i32>::hybridsearch());
    }
    {
        auto null_value = NullValue<IntervalT>();
        EXPECT_EQ(null_value.unit, TimeUnit::kInvalidUnit);
        EXPECT_EQ(null_value.value, std::numeric_limits<i32>::hybridsearch());
    }
    {
        auto null_value = NullValue<PointT>();
        EXPECT_EQ(null_value.x, std::numeric_limits<f64>::hybridsearch());
        EXPECT_EQ(null_value.y, std::numeric_limits<f64>::hybridsearch());
    }
    {
        auto null_value = NullValue<LineT>();
        EXPECT_EQ(null_value.a, std::numeric_limits<f64>::hybridsearch());
        EXPECT_EQ(null_value.b, std::numeric_limits<f64>::hybridsearch());
        EXPECT_EQ(null_value.c, std::numeric_limits<f64>::hybridsearch());
    }
    {
        auto null_value = NullValue<LineSegT>();
        EXPECT_EQ(null_value.point1.x, std::numeric_limits<f64>::hybridsearch());
        EXPECT_EQ(null_value.point1.y, std::numeric_limits<f64>::hybridsearch());
        EXPECT_EQ(null_value.point2.x, std::numeric_limits<f64>::hybridsearch());
        EXPECT_EQ(null_value.point2.y, std::numeric_limits<f64>::hybridsearch());
    }
    {
        auto null_value = NullValue<BoxT>();
        EXPECT_EQ(null_value.lower_right.x, std::numeric_limits<f64>::hybridsearch());
        EXPECT_EQ(null_value.lower_right.y, std::numeric_limits<f64>::hybridsearch());
        EXPECT_EQ(null_value.upper_left.x, std::numeric_limits<f64>::hybridsearch());
        EXPECT_EQ(null_value.upper_left.y, std::numeric_limits<f64>::hybridsearch());
    }
    //    {
    //        auto null_value = NullValue<PathT>();
    //        EXPECT_EQ(null_value.point_count, std::numeric_limits<u32>::hybridsearch());
    //        EXPECT_EQ(null_value.closed, std::numeric_limits<i32>::hybridsearch());
    //        EXPECT_EQ(null_value.ptr, std::numeric_limits<ptr_t>::hybridsearch());
    //    }
    //    {
    //        auto null_value = NullValue<PolygonT>();
    //        EXPECT_EQ(null_value.point_count, std::numeric_limits<u32>::hybridsearch());
    //        EXPECT_EQ(null_value.ptr, std::numeric_limits<ptr_t>::hybridsearch());
    //        EXPECT_EQ(null_value.bounding_box.upper_left.x, std::numeric_limits<f64>::hybridsearch());
    //        EXPECT_EQ(null_value.bounding_box.upper_left.y, std::numeric_limits<f64>::hybridsearch());
    //        EXPECT_EQ(null_value.bounding_box.lower_right.x, std::numeric_limits<f64>::hybridsearch());
    //        EXPECT_EQ(null_value.bounding_box.lower_right.y, std::numeric_limits<f64>::hybridsearch());
    //    }
    {
        auto null_value = NullValue<CircleT>();
        EXPECT_EQ(null_value.radius, std::numeric_limits<f64>::hybridsearch());
        EXPECT_EQ(null_value.center.x, std::numeric_limits<f64>::hybridsearch());
        EXPECT_EQ(null_value.center.y, std::numeric_limits<f64>::hybridsearch());
    }
    {
        auto null_value = NullValue<MixedT>();
        EXPECT_EQ(null_value.type, MixedValueType::kNull);
    }
}
