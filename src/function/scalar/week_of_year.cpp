
module;
#include <chrono>
module week_of_year;
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

struct WeekOfYearFunction {
    template <typename TA, typename TB>
    static inline bool Run(TA left, TB &result) {
        Status status = Status::NotSupport("Not implemented");
        RecoverableError(status);
        return false;
    }

};

template <>
inline bool WeekOfYearFunction::Run(DateT left, BigIntT &result) {
    year_month_day ymd;
    DateT::OuterDate2YMD(left, ymd);
    year_month_day ymd_first{ymd.year(), month(1), day(1)};
    sys_days target_day = sys_days{ymd};
    sys_days first_day = sys_days{ymd_first};
    days days_diff = target_day - first_day;
    result = (days_diff.count() / 7) + 1;

    return true;
}

template <>
inline bool WeekOfYearFunction::Run(DateTimeT left, BigIntT &result) {
    year_month_day ymd;
    DateTimeT::OuterDateTime2YMD(left.date, ymd);
    year_month_day ymd_first{ymd.year(), month(1), day(1)};
    sys_days target_day = sys_days{ymd};
    sys_days first_day = sys_days{ymd_first};
    days days_diff = target_day - first_day;
    result = (days_diff.count() / 7) + 1;
    return true;
}

template <>
inline bool WeekOfYearFunction::Run(TimestampT left, BigIntT &result) {
    year_month_day ymd;
    TimestampT::OuterDateTime2YMD(left.date, ymd);
    year_month_day ymd_first{ymd.year(), month(1), day(1)};
    sys_days target_day = sys_days{ymd};
    sys_days first_day = sys_days{ymd_first};
    days days_diff = target_day - first_day;
    result = (days_diff.count() / 7) + 1;
    return true;
}

void RegisterWeekOfYearFunction(const UniquePtr<Catalog> &catalog_ptr) {
    String func_name = "weekofyear";

    SharedPtr<ScalarFunctionSet> function_set_ptr = MakeShared<ScalarFunctionSet>(func_name);

    ScalarFunction week_of_year_date_function(func_name,
                                  {DataType(LogicalType::kDate)},
                                  {DataType(LogicalType::kBigInt)},
                                  &ScalarFunction::UnaryFunctionWithFailure<DateT, BigIntT, WeekOfYearFunction>);
    function_set_ptr->AddFunction(week_of_year_date_function);

    ScalarFunction week_of_year_datetime_function(func_name,
                                  {DataType(LogicalType::kDateTime)},
                                  {DataType(LogicalType::kBigInt)},
                                  &ScalarFunction::UnaryFunctionWithFailure<DateTimeT, BigIntT, WeekOfYearFunction>);
    function_set_ptr->AddFunction(week_of_year_datetime_function);

    ScalarFunction week_of_year_timestamp_function(func_name,
                                  {DataType(LogicalType::kTimestamp)},
                                  {DataType(LogicalType::kBigInt)},
                                  &ScalarFunction::UnaryFunctionWithFailure<TimestampT, BigIntT, WeekOfYearFunction>);
    function_set_ptr->AddFunction(week_of_year_timestamp_function);

    Catalog::AddFunctionSet(catalog_ptr.get(), function_set_ptr);
}

} // namespace hybridsearch