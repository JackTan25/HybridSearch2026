

module;

#include <type_traits>

module and_func;

import stl;
import catalog;
import status;
import hybridsearch_exception;
import scalar_function;
import scalar_function_set;

import third_party;
import logical_type;
import internal_types;
import data_type;
import logger;

namespace hybridsearch {

struct AndFunction {
    template <typename TA, typename TB, typename TC>
    static inline void Run(TA left, TB right, TC &result) {
        if constexpr (std::is_same_v<std::remove_cv_t<TA>, u8> && std::is_same_v<std::remove_cv_t<TB>, u8> &&
                      std::is_same_v<std::remove_cv_t<TC>, u8>) {
            result = left & right;
        } else if constexpr (std::is_same_v<std::remove_cv_t<TA>, BooleanT> && std::is_same_v<std::remove_cv_t<TB>, BooleanT> &&
                             std::is_same_v<std::remove_cv_t<TC>, BooleanT>) {
            result = left and right;
        } else {
            String error_message = "AND function accepts only u8 and BooleanT.";
            UnrecoverableError(error_message);
        }
    }
};

static void GenerateAndFunction(SharedPtr<ScalarFunctionSet> &function_set_ptr) {
    String func_name = "AND";
    ScalarFunction and_function(func_name,
                                {DataType(LogicalType::kBoolean), DataType(LogicalType::kBoolean)},
                                {DataType(LogicalType::kBoolean)},
                                &ScalarFunction::BinaryFunction<BooleanT, BooleanT, BooleanT, AndFunction>);
    function_set_ptr->AddFunction(and_function);
}

void RegisterAndFunction(const UniquePtr<Catalog> &catalog_ptr) {
    String func_name = "AND";
    SharedPtr<ScalarFunctionSet> function_set_ptr = MakeShared<ScalarFunctionSet>(func_name);

    GenerateAndFunction(function_set_ptr);

    Catalog::AddFunctionSet(catalog_ptr.get(), function_set_ptr);
}

} // namespace hybridsearch