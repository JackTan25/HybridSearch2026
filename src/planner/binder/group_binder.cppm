

module;

import stl;

import base_expression;
import subquery_expression;
import bind_context;
import expression_binder;
import query_context;
import function;
import bind_alias_proxy;
import parsed_expr;
import column_expr;
import constant_expr;
import function_expr;
import knn_expr;
import subquery_expr;

export module group_binder;

namespace hybridsearch {

export class GroupBinder : public ExpressionBinder {
public:
    i64 group_by_expr_index{-1};

    HashSet<i64> bound_select_index_;

public:
    explicit GroupBinder(QueryContext *query_context, const SharedPtr<BindAliasProxy> &bind_alias_proxy)
        : ExpressionBinder(query_context), bind_alias_proxy_(bind_alias_proxy) {}

    // Bind expression entry
    SharedPtr<BaseExpression> BuildExpression(const ParsedExpr &expr, BindContext *bind_context_ptr, i64 depth, bool root) override;

    SharedPtr<BaseExpression> BindColumnReference(const ColumnExpr &expr, BindContext *bind_context_ptr);

    SharedPtr<BaseExpression> BindConstantExpression(const ConstantExpr &expr, BindContext *bind_context_ptr);

    SharedPtr<BaseExpression> BuildColExpr(const ColumnExpr &expr, BindContext *bind_context_ptr, i64 depth, bool root) override;

    SharedPtr<BaseExpression> BuildFuncExpr(const FunctionExpr &expr, BindContext *bind_context_ptr, i64 depth, bool root) override;

    void CheckFuncType(FunctionType func_type) const override;

    SharedPtr<SubqueryExpression>
    BuildSubquery(const SubqueryExpr &select, BindContext *bind_context_ptr, SubqueryType subquery_type, i64 depth, bool root) final;

    SharedPtr<BaseExpression> BuildKnnExpr(const KnnExpr &expr, BindContext *bind_context_ptr, i64 depth, bool root) override;

private:
    const SharedPtr<BindAliasProxy> &bind_alias_proxy_;
};

} // namespace hybridsearch
