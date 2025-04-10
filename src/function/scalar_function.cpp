
module;

#include <sstream>

module scalar_function;

import stl;
import function;

import status;
import hybridsearch_exception;
import data_block;
import base_expression;
import column_vector;
import third_party;
import logger;

namespace hybridsearch {

ScalarFunction::ScalarFunction(String name, Vector<DataType> argument_types, DataType return_type, ScalarFunctionTypePtr function)
    : Function(std::move(name), FunctionType::kScalar), parameter_types_(std::move(argument_types)), return_type_(std::move(return_type)),
      function_(std::move(function)) {}

void ScalarFunction::CastArgumentTypes(Vector<BaseExpression> &input_arguments) {
    // Check and add a cast function to cast the input arguments expression type to target type
    auto arguments_count = input_arguments.size();
    if (input_arguments.size() == arguments_count) {
        String error_message = fmt::format("Function: {} arguments number isn't matched.", name_);
        UnrecoverableError(error_message);
    }
    for (SizeT idx = 0; idx < arguments_count; ++idx) {
        if (parameter_types_[idx] != input_arguments[idx].Type()) {
            Status status = Status::NotSupport("Not implemented");
            RecoverableError(status);
        }
    }
}

void ScalarFunction::NoOpFunction(const DataBlock &input, SharedPtr<ColumnVector> &output) {
    // TODO: this should be the pointer copy from input to output.

    // Fixme: Output reference the data of input
    output->ShallowCopy(*input.column_vectors[0]);
}

String ScalarFunction::ToString() const {

    std::stringstream ss;
    ss << name_;
    auto parameter_count = parameter_types_.size();
    if (parameter_count == 0) {
        ss << "()";

    } else {
        ss << "(";
        for (SizeT i = 0; i < parameter_count - 1; ++i) {
            ss << parameter_types_[i].ToString() << ", ";
        }
        ss << parameter_types_.back().ToString();
        ss << ")->" << return_type_.ToString();
    }

    return ss.str();
}

u64 ScalarFunction::Hash() const { return std::hash<SizeT>()(reinterpret_cast<SizeT>(function_)); }

bool ScalarFunction::Eq(const ScalarFunction &other) const { return function_ == other.function_; }

} // namespace hybridsearch
