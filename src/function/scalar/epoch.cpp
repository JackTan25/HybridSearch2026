
module;
module epoch;
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

struct EpochFunction {
    template <typename TA, typename TB>
    static inline bool Run(TA left, TB &result) {
        Status status = Status::NotSupport("Not implemented");
        RecoverableError(status);
        return false;
    }

};

template <>
inline bool EpochFunction::Run(DateT left, BigIntT &result) {
    result = left.GetEpochTime();
    return true;
}

template <>
inline bool EpochFunction::Run(DateTimeT left, BigIntT &result) {
    result = left.GetEpochTime();
    return true;
}

template <>
inline bool EpochFunction::Run(TimestampT left, BigIntT &result) {
    result = left.GetEpochTime();
    return true;
}

void RegisterEpochFunction(const UniquePtr<Catalog> &catalog_ptr) {
    String func_name = "epoch";

    SharedPtr<ScalarFunctionSet> function_set_ptr = MakeShared<ScalarFunctionSet>(func_name);

    ScalarFunction epoch_date_function(func_name,
                                  {DataType(LogicalType::kDate)},
                                  {DataType(LogicalType::kBigInt)},
                                  &ScalarFunction::UnaryFunctionWithFailure<DateT, BigIntT, EpochFunction>);
    function_set_ptr->AddFunction(epoch_date_function);

    ScalarFunction epoch_datetime_function(func_name,
                                  {DataType(LogicalType::kDateTime)},
                                  {DataType(LogicalType::kBigInt)},
                                  &ScalarFunction::UnaryFunctionWithFailure<DateTimeT, BigIntT, EpochFunction>);
    function_set_ptr->AddFunction(epoch_datetime_function);

    ScalarFunction epoch_timestamp_function(func_name,
                                  {DataType(LogicalType::kTimestamp)},
                                  {DataType(LogicalType::kBigInt)},
                                  &ScalarFunction::UnaryFunctionWithFailure<TimestampT, BigIntT, EpochFunction>);
    function_set_ptr->AddFunction(epoch_timestamp_function);

    Catalog::AddFunctionSet(catalog_ptr.get(), function_set_ptr);
}

} // namespace hybridsearch