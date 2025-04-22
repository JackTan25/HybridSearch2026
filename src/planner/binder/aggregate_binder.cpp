

module;

module aggregate_binder;

import stl;
import base_expression;

import bind_context;
import expression_binder;
import status;
import hybridsearch_exception;
import parsed_expr;
import knn_expr;
import logger;

namespace hybridsearch {

SharedPtr<BaseExpression> AggregateBinder::BuildExpression(const ParsedExpr &expr, BindContext *bind_context_ptr, i64 depth, bool root) {
    SharedPtr<BaseExpression> result = ExpressionBinder::BuildExpression(expr, bind_context_ptr, depth, root);
    return result;
}

SharedPtr<BaseExpression> AggregateBinder::BuildKnnExpr(const KnnExpr &, BindContext *, i64, bool) {
    Status status = Status::SyntaxError("KNN expression isn't supported in in aggregate function");
    RecoverableError(status);
    return nullptr;
}

} // namespace hybridsearch
