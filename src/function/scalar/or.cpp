

module;

#include <type_traits>

module or_func;

import stl;
import catalog;
import logical_type;
import hybridsearch_exception;
import scalar_function;
import scalar_function_set;

// import third_party;
import status;
import internal_types;
import data_type;
import logger;

namespace hybridsearch {

struct OrFunction {
    template <typename TA, typename TB, typename TC>
    static inline void Run(TA left, TB right, TC &result) {
        if constexpr (std::is_same_v<std::remove_cv_t<TA>, u8> && std::is_same_v<std::remove_cv_t<TB>, u8> &&
                      std::is_same_v<std::remove_cv_t<TC>, u8>) {
            result = left | right;
        } else if constexpr (std::is_same_v<std::remove_cv_t<TA>, BooleanT> && std::is_same_v<std::remove_cv_t<TB>, BooleanT> &&
                             std::is_same_v<std::remove_cv_t<TC>, BooleanT>) {
            result = left or right;
        } else {
            String error_message = "OR function accepts only u8 and BooleanT.";
            UnrecoverableError(error_message);
        }
    }
};

static void GenerateOrFunction(SharedPtr<ScalarFunctionSet> &function_set_ptr) {
    String func_name = "OR";
    ScalarFunction or_function(func_name,
                               {DataType(LogicalType::kBoolean), DataType(LogicalType::kBoolean)},
                               {DataType(LogicalType::kBoolean)},
                               &ScalarFunction::BinaryFunction<BooleanT, BooleanT, BooleanT, OrFunction>);
    function_set_ptr->AddFunction(or_function);
}

void RegisterOrFunction(const UniquePtr<Catalog> &catalog_ptr) {
    String func_name = "OR";

    SharedPtr<ScalarFunctionSet> function_set_ptr = MakeShared<ScalarFunctionSet>(func_name);

    GenerateOrFunction(function_set_ptr);

    Catalog::AddFunctionSet(catalog_ptr.get(), function_set_ptr);
}

} // namespace hybridsearch