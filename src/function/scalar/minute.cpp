
module;
module minute;
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

struct MinuteFunction {
    template <typename TA, typename TB>
    static inline bool Run(TA left, TB &result) {
        Status status = Status::NotSupport("Not implemented");
        RecoverableError(status);
        return false;
    }

};

template <>
inline bool MinuteFunction::Run(DateTimeT left, BigIntT &result) {
    result = DateTimeT::GetDateTimePart(left, TimeUnit::kMinute);
    return true;
}

template <>
inline bool MinuteFunction::Run(TimeT left, BigIntT &result) {
    result = TimeT::GetTimePart(left, TimeUnit::kMinute);
    return true;
}

template <>
inline bool MinuteFunction::Run(TimestampT left, BigIntT &result) {
    result = TimestampT::GetDateTimePart(left, TimeUnit::kMinute);
    return true;
}

void RegisterMinuteFunction(const UniquePtr<Catalog> &catalog_ptr) {
    String func_name = "minute";

    SharedPtr<ScalarFunctionSet> function_set_ptr = MakeShared<ScalarFunctionSet>(func_name);

    ScalarFunction minute_datetime_function(func_name,
                                  {DataType(LogicalType::kDateTime)},
                                  {DataType(LogicalType::kBigInt)},
                                  &ScalarFunction::UnaryFunctionWithFailure<DateTimeT, BigIntT, MinuteFunction>);
    function_set_ptr->AddFunction(minute_datetime_function);

    ScalarFunction minute_time_function(func_name,
                                  {DataType(LogicalType::kTime)},
                                  {DataType(LogicalType::kBigInt)},
                                  &ScalarFunction::UnaryFunctionWithFailure<TimeT, BigIntT, MinuteFunction>);
    function_set_ptr->AddFunction(minute_time_function);

    ScalarFunction minute_timestamp_function(func_name,
                                  {DataType(LogicalType::kTimestamp)},
                                  {DataType(LogicalType::kBigInt)},
                                  &ScalarFunction::UnaryFunctionWithFailure<TimestampT, BigIntT, MinuteFunction>);
    function_set_ptr->AddFunction(minute_timestamp_function);

    Catalog::AddFunctionSet(catalog_ptr.get(), function_set_ptr);
}

} // namespace hybridsearch