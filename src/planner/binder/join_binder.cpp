

module;

module join_binder;

import stl;
import base_expression;

import bind_context;
import status;
import hybridsearch_exception;
import parsed_expr;
import knn_expr;
import logger;

namespace hybridsearch {

SharedPtr<BaseExpression> JoinBinder::BuildExpression(const ParsedExpr &expr, BindContext *bind_context_ptr, i64 depth, bool root) {
    SharedPtr<BaseExpression> result;
    switch (expr.type_) {
        case ParsedExprType::kSubquery: {
            Status status = Status::SyntaxError("Subquery isn't allowed in JOIN condition.");
            RecoverableError(status);
        }
        default: {
            result = ExpressionBinder::BuildExpression(expr, bind_context_ptr, depth, root);
        }
    }
    return result;
}

SharedPtr<BaseExpression> JoinBinder::BuildKnnExpr(const KnnExpr &, BindContext *, i64, bool) {

    Status status = Status::SyntaxError("KNN expression isn't supported in join clause.");
    RecoverableError(status);
    return nullptr;
}

} // namespace hybridsearch
