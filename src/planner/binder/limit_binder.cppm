

module;

export module limit_binder;

import stl;

import base_expression;
import bind_context;
import expression_binder;
import query_context;
import parsed_expr;
import function_expr;
import column_expr;
import knn_expr;

namespace hybridsearch {

export class LimitBinder final : public ExpressionBinder {
public:
    explicit LimitBinder(QueryContext *query_context) : ExpressionBinder(query_context) {}

    // Bind expression entry
    SharedPtr<BaseExpression> BuildExpression(const ParsedExpr &expr, BindContext *bind_context_ptr, i64 depth, bool root) final;

    SharedPtr<BaseExpression> BuildFuncExpr(const FunctionExpr &expr, BindContext *bind_context_ptr, i64 depth, bool root) final;

    SharedPtr<BaseExpression> BuildColExpr(const ColumnExpr &expr, BindContext *bind_context_ptr, i64 depth, bool root) final;

    SharedPtr<BaseExpression> BuildKnnExpr(const KnnExpr &expr, BindContext *bind_context_ptr, i64 depth, bool root) final;

private:
};

} // namespace hybridsearch
