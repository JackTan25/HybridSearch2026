

module;

import stl;
import expression_type;

module value_expression;

namespace hybridsearch {

String ValueExpression::ToString() const { return value_.ToString(); }

u64 ValueExpression::Hash() const { return 0; }

bool ValueExpression::Eq(const BaseExpression &other_base) const {
    if (other_base.type() != ExpressionType::kValue) {
        return false;
    }
    const auto &other = static_cast<const ValueExpression &>(other_base);
    return value_ == other.value_;
}

} // namespace hybridsearch
