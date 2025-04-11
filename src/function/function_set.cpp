

module;

#include <sstream>

module function_set;

import stl;
import catalog;
import base_expression;

namespace hybridsearch {

SharedPtr<FunctionSet> FunctionSet::GetFunctionSet(Catalog *catalog, const FunctionExpr &expr) {
    String function_name = expr.func_name_;

    // SharedPtr<Catalog>& catalog
    SharedPtr<FunctionSet> function_set_ptr = Catalog::GetFunctionSetByName(catalog, function_name);

    return function_set_ptr;
}

String FunctionSet::ToString(const String &name, const Vector<SharedPtr<BaseExpression>> &arguments) {
    std::stringstream ss;

    ss << name;
    SizeT argument_count = arguments.size();
    if (argument_count == 0) {
        ss << "()";

    } else {
        ss << "(";
        for (SizeT i = 0; i < argument_count - 1; ++i) {
            ss << arguments[i]->Type().ToString() << ", ";
        }
        ss << arguments.back()->Type().ToString();
        ss << ")";
    }
    return ss.str();
}

} // namespace hybridsearch
