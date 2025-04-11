

module;

#include <sstream>

module case_expression;

import stl;
import expression_type;
import hybridsearch_exception;
import logger;

namespace hybridsearch {

CaseExpression::CaseExpression() : BaseExpression(ExpressionType::kCase, {}) {}

void CaseExpression::AddCaseCheck(const SharedPtr<BaseExpression> &when_expr, const SharedPtr<BaseExpression> &then_expr) {
    case_check_.emplace_back(when_expr, then_expr);
}

void CaseExpression::AddElseExpr(const SharedPtr<BaseExpression> &else_expr) {
    if (else_expr_.get() != nullptr) {
        String error_message = "else expression already been assigned before.";
        UnrecoverableError(error_message);
    }
    else_expr_ = else_expr;
}

String CaseExpression::ToString() const {
    std::stringstream ss;

    for (auto &check : case_check_) {
        ss << "When: " << check.when_expr_->Name() << " Then: " << check.then_expr_->Name() << std::endl;
    }
    ss << "Else: " << else_expr_->Name() << std::endl;

    return ss.str();
}

DataType CaseExpression::Type() const { return return_type_; }

} // namespace hybridsearch
