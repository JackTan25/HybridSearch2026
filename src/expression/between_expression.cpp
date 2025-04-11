

module;

#include <sstream>

module between_expression;

import stl;
import expression_type;

import logical_type;

namespace hybridsearch {

BetweenExpression::BetweenExpression(const SharedPtr<BaseExpression> &value,
                                     const SharedPtr<BaseExpression> &upper_bound,
                                     const SharedPtr<BaseExpression> &lower_bound,
                                     bool upper_inclusive,
                                     bool lower_inclusive)
    : BaseExpression(ExpressionType::kBetween, {lower_bound, value, upper_bound}), lower_inclusive_(lower_inclusive),
      upper_inclusive_(upper_inclusive) {}

String BetweenExpression::ToString() const {
    std::stringstream ss;

    ss << arguments_[1]->Name() << " Between " << arguments_[0]->Name() << " And " << arguments_[2]->Name();

    return ss.str();
}

DataType BetweenExpression::Type() const { return DataType(LogicalType::kBoolean); }

} // namespace hybridsearch
