

module;

module insert_binder;

import stl;
import base_expression;

import bind_context;
import column_expression;
import function;
import status;
import hybridsearch_exception;
import third_party;
import function_set;
import bind_alias_proxy;
import logger;

namespace hybridsearch {

SharedPtr<BaseExpression> InsertBinder::BuildExpression(const ParsedExpr &expr, BindContext *bind_context_ptr, i64 depth, bool root) {
    SharedPtr<BaseExpression> result = ExpressionBinder::BuildExpression(expr, bind_context_ptr, depth, root);
    return result;
}

SharedPtr<BaseExpression> InsertBinder::BuildKnnExpr(const KnnExpr &, BindContext *, i64, bool) {
    Status status = Status::SyntaxError("KNN expression isn't supported in insert clause");
    RecoverableError(status);
    return nullptr;
}

// SharedPtr<BaseExpression>
// InsertBinder::BuildColRefExpr(const hsql::Expr &expr, const SharedPtr<BindContext>& bind_context_ptr) {
//     PlannerError("HavingBinder::BuildColRefExpr");
// }

} // namespace hybridsearch