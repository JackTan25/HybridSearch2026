

module;

import stl;
import base_expression;

import function;
import bind_context;
import expression_binder;
import query_context;
import bind_alias_proxy;
import parsed_expr;
import column_expr;

export module where_binder;

namespace hybridsearch {

export class WhereBinder final : public ExpressionBinder {
public:
    explicit WhereBinder(QueryContext *query_context, const SharedPtr<BindAliasProxy> &bind_alias_proxy)
        : ExpressionBinder(query_context), bind_alias_proxy_(bind_alias_proxy) {}

    // Bind expression entry
    SharedPtr<BaseExpression> BuildExpression(const ParsedExpr &expr, BindContext *bind_context_ptr, i64 depth, bool root) final;

    SharedPtr<BaseExpression> BuildColExpr(const ColumnExpr &expr, BindContext *bind_context_ptr, i64 depth, bool root) final;

    void CheckFuncType(FunctionType func_type) const override;

private:
    const SharedPtr<BindAliasProxy> &bind_alias_proxy_;
};

} // namespace hybridsearch
