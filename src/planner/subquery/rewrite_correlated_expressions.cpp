

module;

module rewrite_correlated_expression;

import stl;
import logical_node;
import base_expression;
import column_expression;
import status;
import hybridsearch_exception;
import subquery_expression;
import logger;

namespace hybridsearch {

void RewriteCorrelatedExpressions::VisitNode(LogicalNode &op) { VisitNodeExpression(op); }

SharedPtr<BaseExpression> RewriteCorrelatedExpressions::VisitReplace(const SharedPtr<ColumnExpression> &expression) {
    if (expression->depth() == 0) {
        return expression;
    }

    if (expression->depth() > 1) {
        Status status = Status::SyntaxError("Correlated depth > 1 is not supported now.");
        RecoverableError(status);
    }

    auto entry = bind_context_ptr_->correlated_column_map_.find(expression->binding());
    if (entry == bind_context_ptr_->correlated_column_map_.end()) {
        // This column expression wasn't stored in correlated column map before
        String error_message = "Correlated expression isn't found.";
        UnrecoverableError(error_message);
    }

    expression->SetBinding(base_binding_.table_idx, base_binding_.column_idx + entry->second);
    expression->SetDepth(0);

    return expression;
}

SharedPtr<BaseExpression> RewriteCorrelatedExpressions::VisitReplace(const SharedPtr<SubqueryExpression> &expression) {
    if (expression->correlated_columns.empty()) {
        // Not correlated
        return nullptr;
    }

    Status status = Status::SyntaxError("Not support rewrite nested correlated subquery in the subquery plan");
    RecoverableError(status);
    return nullptr;
}

} // namespace hybridsearch
