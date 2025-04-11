

module;

import stl;
import base_expression;
import conjunction_expression;

export module expression_transformer;

namespace hybridsearch {

enum class VisitControlType { kInvalid, kVisit, kNotVisit };

// Transform expr_a AND expr_b AND expr_c into expressions array: [expr_a, expr_b, expr_c].
export Vector<SharedPtr<BaseExpression>> SplitExpressionByDelimiter(const SharedPtr<BaseExpression> &expression, ConjunctionType conjunction_type);

export SharedPtr<BaseExpression> ComposeExpressionWithDelimiter(const Vector<SharedPtr<BaseExpression>> &expressions,
                                                                ConjunctionType conjunction_type);

// Traverse the expression and it's child
void VisitExpression(const SharedPtr<BaseExpression> &expression, const std::function<VisitControlType(SharedPtr<BaseExpression> &child)> &visitor);

export void VisitExpression(const SharedPtr<BaseExpression> &expression, const std::function<void(SharedPtr<BaseExpression> &child)> &visitor);

} // namespace hybridsearch
