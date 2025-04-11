

module;

module corrlated_expr_detector;

import stl;
import logical_node;
import base_expression;
import column_expression;
import subquery_expression;
import status;
import hybridsearch_exception;
import logger;

namespace hybridsearch {

void CorrelatedExpressionsDetector::VisitNode(LogicalNode &op) { VisitNodeExpression(op); }

SharedPtr<BaseExpression> CorrelatedExpressionsDetector::VisitReplace(const SharedPtr<ColumnExpression> &expression) {

    if (expression->depth() == 0) {
        return expression;
    }

    if (expression->depth() > 1) {
        Status status = Status::SyntaxError("Column expression with depth > 1 is detected");
        RecoverableError(status);
    }

    is_correlated_ = true;
    return expression;
}

SharedPtr<BaseExpression> CorrelatedExpressionsDetector::VisitReplace(const SharedPtr<SubqueryExpression> &expression) {
    if (expression->correlated_columns.empty()) {
        // Uncorrelated subquery
        return nullptr;
    }

    Status status = Status::SyntaxError("Not support nested correlated subquery in the subquery plan");
    RecoverableError(status);
    return nullptr;
}

} // namespace hybridsearch
