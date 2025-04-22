
module;
module day_of_year;
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

struct DayOfYearFunction {
    template <typename TA, typename TB>
    static inline bool Run(TA left, TB &result) {
        Status status = Status::NotSupport("Not implemented");
        RecoverableError(status);
        return false;
    }

};

template <>
inline bool DayOfYearFunction::Run(DateT left, BigIntT &result) {
    year_month_day ymd;
    DateT::OuterDate2YMD(left, ymd);
    sys_days sd = sys_days(ymd);
    year_month_day start{ymd.year(), month(1), day(1)};
    sys_days start_sd = sys_days(start);
    auto days_diff = sd - start_sd;
    result = days_diff.count() + 1;
    return true;
}

template <>
inline bool DayOfYearFunction::Run(DateTimeT left, BigIntT &result) {
    year_month_day ymd;
    DateTimeT::OuterDateTime2YMD(left.date, ymd);
    sys_days sd = sys_days(ymd);
    year_month_day start{ymd.year(), month(1), day(1)};
    sys_days start_sd = sys_days(start);
    auto days_diff = sd - start_sd;
    result = days_diff.count() + 1;
    return true;
}

template <>
inline bool DayOfYearFunction::Run(TimestampT left, BigIntT &result) {
    year_month_day ymd;
    TimestampT::OuterDateTime2YMD(left.date, ymd);
    sys_days sd = sys_days(ymd);
    year_month_day start{ymd.year(), month(1), day(1)};
    sys_days start_sd = sys_days(start);
    auto days_diff = sd - start_sd;
    result = days_diff.count() + 1;
    return true;
}

void RegisterDayOfYearFunction(const UniquePtr<Catalog> &catalog_ptr) {
    String func_name = "dayofyear";

    SharedPtr<ScalarFunctionSet> function_set_ptr = MakeShared<ScalarFunctionSet>(func_name);

    ScalarFunction day_of_year_date_function(func_name,
                                  {DataType(LogicalType::kDate)},
                                  {DataType(LogicalType::kBigInt)},
                                  &ScalarFunction::UnaryFunctionWithFailure<DateT, BigIntT, DayOfYearFunction>);
    function_set_ptr->AddFunction(day_of_year_date_function);

    ScalarFunction day_of_year_datetime_function(func_name,
                                  {DataType(LogicalType::kDateTime)},
                                  {DataType(LogicalType::kBigInt)},
                                  &ScalarFunction::UnaryFunctionWithFailure<DateTimeT, BigIntT, DayOfYearFunction>);
    function_set_ptr->AddFunction(day_of_year_datetime_function);

    ScalarFunction day_of_year_timestamp_function(func_name,
                                  {DataType(LogicalType::kTimestamp)},
                                  {DataType(LogicalType::kBigInt)},
                                  &ScalarFunction::UnaryFunctionWithFailure<TimestampT, BigIntT, DayOfYearFunction>);
    function_set_ptr->AddFunction(day_of_year_timestamp_function);

    Catalog::AddFunctionSet(catalog_ptr.get(), function_set_ptr);
}

} // namespace hybridsearch