

module;

#include <sstream>
import stl;
import function;

import query_context;

module table_function;
namespace hybridsearch {

TableFunction::TableFunction(String name, Vector<LogicalType> argument_types, TableFunctionType function)
    : Function(std::move(name), FunctionType::kTable), main_function_(std::move(function)), parameter_types_(std::move(argument_types)) {}

String TableFunction::ToString() const {
    std::stringstream ss;
    ss << "Table Function: " << name_;
    return ss.str();
}

} // namespace hybridsearch
