

module;

#include <sstream>

module aggregate_function;

import base_expression;
import hybridsearch_exception;
import status;
import logger;

namespace hybridsearch {

void AggregateFunction::CastArgumentTypes(BaseExpression &) {
    // Check and add a cast function to cast the input arguments expression type to target type
    Status status = Status::NotSupport("Not implemented: need to cast the argument types");
    RecoverableError(status);
}

std::string AggregateFunction::ToString() const {

    std::stringstream ss;
    ss << name_;
    ss << "(";
    ss << argument_type_.ToString();
    ss << ")->" << return_type_.ToString();

    return ss.str();
}

} // namespace hybridsearch