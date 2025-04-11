
module;
#include <chrono>
module current_date;
import stl;
import config;
import catalog;
import status;
import logical_type;
import hybridsearch_context;
import hybridsearch_exception;
import scalar_function;
import scalar_function_set;
import third_party;
import internal_types;
import data_type;
import column_vector;
import singleton;

namespace hybridsearch {
using namespace std::chrono;
struct CurrentDateFunction {
    template <typename TB>
    static inline void Run(TB &result) {
        Status status = Status::NotSupport("Not implemented");
        RecoverableError(status);
        return;
    }
};

template <>
inline void CurrentDateFunction::Run(DateT &result) {
    hybridsearchContext& hybridsearchContext = hybridsearchContext::instance();
    Config* config = hybridsearchContext.config();
    auto now = system_clock::now() + config->hour_offset_;
    auto sys_days = std::chrono::floor<std::chrono::days>(now);
    result.value = sys_days.time_since_epoch().count();
}

void RegisterCurrentDateFunction(const UniquePtr<Catalog> &catalog_ptr) {
    String func_name = "current_date";

    SharedPtr<ScalarFunctionSet> function_set_ptr = MakeShared<ScalarFunctionSet>(func_name);

    ScalarFunction current_date_function(func_name, {}, DataType(LogicalType::kDate), &ScalarFunction::NullaryFunction<DateT, CurrentDateFunction>);
    function_set_ptr->AddFunction(current_date_function);

    Catalog::AddFunctionSet(catalog_ptr.get(), function_set_ptr);
}

} // namespace hybridsearch