
module;
#include <chrono>
module current_timestamp;
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
import query_context;

namespace hybridsearch {
using namespace std::chrono;
struct CurrentTimestampFunction {
    template <typename TB>
    static inline void Run(TB &result) {
        Status status = Status::NotSupport("Not implemented");
        RecoverableError(status);
    }
};

template <>
inline void CurrentTimestampFunction::Run(TimestampT &result) {
    hybridsearchContext& hybridsearchContext = hybridsearchContext::instance();
    Config* config = hybridsearchContext.config();
    auto now = system_clock::now() + config->hour_offset_;
    auto sys_days = std::chrono::floor<std::chrono::days>(now);
    auto sys_secs = std::chrono::floor<std::chrono::seconds>(now);
    auto seconds_per_day = 24 * 60 * 60;
    result.time.value = static_cast<i32>(sys_secs.time_since_epoch().count() - (seconds_per_day * sys_days.time_since_epoch().count()));
    result.date.value = static_cast<i32>(sys_days.time_since_epoch().count());
}

void RegisterCurrentTimestampFunction(const UniquePtr<Catalog> &catalog_ptr) {
    String func_name = "current_timestamp";

    SharedPtr<ScalarFunctionSet> function_set_ptr = MakeShared<ScalarFunctionSet>(func_name);

    ScalarFunction current_timestamp_function(func_name,
                                  {},
                                  DataType(LogicalType::kTimestamp),
                                  &ScalarFunction::NullaryFunction<TimestampT, CurrentTimestampFunction>);
    function_set_ptr->AddFunction(current_timestamp_function);

    Catalog::AddFunctionSet(catalog_ptr.get(), function_set_ptr);
}

} // namespace hybridsearch