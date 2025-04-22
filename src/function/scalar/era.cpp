
module;
module era;
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

struct EraFunction {
    template <typename TA, typename TB>
    static inline bool Run(TA left, TB &result) {
        Status status = Status::NotSupport("Not implemented");
        RecoverableError(status);
        return false;
    }

};

template <>
inline bool EraFunction::Run(DateT left, BigIntT &result) {
    result = DateT::GetDatePart(left, TimeUnit::kYear) > 0 ? 1 : 0;
    return true;
}

template <>
inline bool EraFunction::Run(DateTimeT left, BigIntT &result) {
    result = DateTimeT::GetDateTimePart(left, TimeUnit::kYear) > 0 ? 1 : 0;
    return true;
}

template <>
inline bool EraFunction::Run(TimestampT left, BigIntT &result) {
    result = TimestampT::GetDateTimePart(left, TimeUnit::kYear) > 0 ? 1 : 0;
    return true;
}

void RegisterEraFunction(const UniquePtr<Catalog> &catalog_ptr) {
    String func_name = "era";

    SharedPtr<ScalarFunctionSet> function_set_ptr = MakeShared<ScalarFunctionSet>(func_name);

    ScalarFunction era_date_function(func_name,
                                  {DataType(LogicalType::kDate)},
                                  {DataType(LogicalType::kBigInt)},
                                  &ScalarFunction::UnaryFunctionWithFailure<DateT, BigIntT, EraFunction>);
    function_set_ptr->AddFunction(era_date_function);

    ScalarFunction era_datetime_function(func_name,
                                  {DataType(LogicalType::kDateTime)},
                                  {DataType(LogicalType::kBigInt)},
                                  &ScalarFunction::UnaryFunctionWithFailure<DateTimeT, BigIntT, EraFunction>);
    function_set_ptr->AddFunction(era_datetime_function);

    ScalarFunction era_timestamp_function(func_name,
                                  {DataType(LogicalType::kTimestamp)},
                                  {DataType(LogicalType::kBigInt)},
                                  &ScalarFunction::UnaryFunctionWithFailure<TimestampT, BigIntT, EraFunction>);
    function_set_ptr->AddFunction(era_timestamp_function);

    Catalog::AddFunctionSet(catalog_ptr.get(), function_set_ptr);
}

} // namespace hybridsearch