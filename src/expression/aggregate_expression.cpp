

module;

#include <sstream>
import stl;
import expression_type;
import aggregate_function;

module aggregate_expression;

namespace hybridsearch {

AggregateExpression::AggregateExpression(AggregateFunction aggregate_function, Vector<SharedPtr<BaseExpression>> arguments)
    : BaseExpression(ExpressionType::kAggregate, std::move(arguments)), aggregate_function_(std::move(aggregate_function)) {}

bool AggregateExpression::IsCountStar() const { return false; }

String AggregateExpression::ToString() const {
    std::stringstream ss;
    ss << aggregate_function_.name() << "(";
    if (arguments_.empty()) {
        ss << ")";
    } else {
        SizeT arguments_count = arguments_.size();
        for (SizeT i = 0; i < arguments_count - 1; ++i) {
            ss << arguments_[i]->Name() << ", ";
        }
        ss << arguments_.back()->Name() << ")";
    }

    return ss.str();
}

DataType AggregateExpression::Type() const { return aggregate_function_.return_type(); }

} // namespace hybridsearch
