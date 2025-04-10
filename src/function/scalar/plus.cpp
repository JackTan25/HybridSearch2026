

module;

module plus;

import stl;
import catalog;
import logical_type;
import hybridsearch_exception;
import scalar_function;
import scalar_function_set;

import third_party;
import internal_types;
import data_type;
import logger;

namespace hybridsearch {

struct PlusFunction {
    template <typename TA, typename TB>
    static inline bool Run(TA, TB &) {
        String error_message = "Not implement: PlusFunction::Run";
        UnrecoverableError(error_message);
        return false;
    }
};

template <>
inline bool PlusFunction::Run(MixedT value, MixedT &result) {
    if (value.type == MixedValueType::kInteger || value.type == MixedValueType::kFloat) {
        result = value;
        return true;
    } else {
        result.type = MixedValueType::kNull;
        return false;
    }
}

SharedPtr<ScalarFunctionSet> GetPlusFunctionSet() {
    String func_name = "+";

    SharedPtr<ScalarFunctionSet> function_set_ptr = MakeShared<ScalarFunctionSet>(func_name);

    // Plus operator
    ScalarFunction plus_int8(func_name, {DataType(LogicalType::kTinyInt)}, DataType(LogicalType::kTinyInt), &ScalarFunction::NoOpFunction);
    function_set_ptr->AddFunction(plus_int8);

    ScalarFunction plus_int16(func_name, {DataType(LogicalType::kSmallInt)}, DataType(LogicalType::kSmallInt), &ScalarFunction::NoOpFunction);
    function_set_ptr->AddFunction(plus_int16);

    ScalarFunction plus_int32(func_name, {DataType(LogicalType::kInteger)}, DataType(LogicalType::kInteger), &ScalarFunction::NoOpFunction);
    function_set_ptr->AddFunction(plus_int32);

    ScalarFunction plus_int64(func_name, {DataType(LogicalType::kBigInt)}, DataType(LogicalType::kBigInt), &ScalarFunction::NoOpFunction);
    function_set_ptr->AddFunction(plus_int64);

    ScalarFunction plus_int128(func_name, {DataType(LogicalType::kHugeInt)}, DataType(LogicalType::kHugeInt), &ScalarFunction::NoOpFunction);
    function_set_ptr->AddFunction(plus_int128);

    ScalarFunction plus_float16(func_name, {DataType(LogicalType::kFloat16)}, DataType(LogicalType::kFloat16), &ScalarFunction::NoOpFunction);
    function_set_ptr->AddFunction(plus_float16);

    ScalarFunction plus_bfloat16(func_name, {DataType(LogicalType::kBFloat16)}, DataType(LogicalType::kBFloat16), &ScalarFunction::NoOpFunction);
    function_set_ptr->AddFunction(plus_bfloat16);

    ScalarFunction plus_float(func_name, {DataType(LogicalType::kFloat)}, DataType(LogicalType::kFloat), &ScalarFunction::NoOpFunction);
    function_set_ptr->AddFunction(plus_float);

    ScalarFunction plus_double(func_name, {DataType(LogicalType::kDouble)}, DataType(LogicalType::kDouble), &ScalarFunction::NoOpFunction);
    function_set_ptr->AddFunction(plus_double);

    ScalarFunction plus_decimal(func_name, {DataType(LogicalType::kDecimal)}, DataType(LogicalType::kDecimal), &ScalarFunction::NoOpFunction);
    function_set_ptr->AddFunction(plus_decimal);

    ScalarFunction plus_mixed(func_name,
                              {DataType(LogicalType::kMixed)},
                              DataType(LogicalType::kMixed),
                              &ScalarFunction::UnaryFunctionWithFailure<MixedT, MixedT, PlusFunction>);
    function_set_ptr->AddFunction(plus_mixed);

    return function_set_ptr;
}

void RegisterPlusFunction(const UniquePtr<Catalog> &catalog_ptr) {
    auto function_set_ptr = GetPlusFunctionSet();
    Catalog::AddFunctionSet(catalog_ptr.get(), std::move(function_set_ptr));
}

void AppendRegisterPlusFunction(const UniquePtr<Catalog> &catalog_ptr) {
    auto function_set_ptr = GetPlusFunctionSet();
    Catalog::AppendToScalarFunctionSet(catalog_ptr.get(), std::move(function_set_ptr));
}

} // namespace hybridsearch
