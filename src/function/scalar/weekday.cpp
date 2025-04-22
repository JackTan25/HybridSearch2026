
module;
#include <chrono>
module weekday;
import stl;
import catalog;
import status;
import logical_type;
import hybridsearch_exception;
import scalar_function;
import scalar_function_set;
import third_party;
import internal_types;
import data_type;
import column_vector;

namespace hybridsearch {

using namespace std::chrono;

struct WeekdayFunction {
    template <typename TA, typename TB>
    static inline bool Run(TA left, TB &result) {
        Status status = Status::NotSupport("Not implemented");
        RecoverableError(status);
        return false;
    }

};

template <>
inline bool WeekdayFunction::Run(DateT left, BigIntT &result) {
    year_month_day ymd;
    DateT::OuterDate2YMD(left, ymd);
    result = static_cast<i64>(weekday{ymd}.c_encoding());
    return true;
}

template <>
inline bool WeekdayFunction::Run(DateTimeT left, BigIntT &result) {
    year_month_day ymd;
    DateTimeT::OuterDateTime2YMD(left.date, ymd);
    result = static_cast<i64>(weekday{ymd}.c_encoding());
    return true;
}

template <>
inline bool WeekdayFunction::Run(TimestampT left, BigIntT &result) {
    year_month_day ymd;
    TimestampT::OuterDateTime2YMD(left.date, ymd);
    result = static_cast<i64>(weekday{ymd}.c_encoding());
    return true;
}

void RegisterWeekdayFunction(const UniquePtr<Catalog> &catalog_ptr) {
    String func_name = "weekday";

    SharedPtr<ScalarFunctionSet> function_set_ptr = MakeShared<ScalarFunctionSet>(func_name);

    ScalarFunction weekday_date_function(func_name,
                                  {DataType(LogicalType::kDate)},
                                  {DataType(LogicalType::kBigInt)},
                                  &ScalarFunction::UnaryFunctionWithFailure<DateT, BigIntT, WeekdayFunction>);
    function_set_ptr->AddFunction(weekday_date_function);

    ScalarFunction weekday_datetime_function(func_name,
                                  {DataType(LogicalType::kDateTime)},
                                  {DataType(LogicalType::kBigInt)},
                                  &ScalarFunction::UnaryFunctionWithFailure<DateTimeT, BigIntT, WeekdayFunction>);
    function_set_ptr->AddFunction(weekday_datetime_function);

    ScalarFunction weekday_timestamp_function(func_name,
                                  {DataType(LogicalType::kTimestamp)},
                                  {DataType(LogicalType::kBigInt)},
                                  &ScalarFunction::UnaryFunctionWithFailure<TimestampT, BigIntT, WeekdayFunction>);
    function_set_ptr->AddFunction(weekday_timestamp_function);

    Catalog::AddFunctionSet(catalog_ptr.get(), function_set_ptr);
}

} // namespace hybridsearch