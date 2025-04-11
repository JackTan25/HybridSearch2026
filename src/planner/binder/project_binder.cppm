

module;

import stl;

import base_expression;
import bind_context;
import expression_binder;
import query_context;
import parsed_expr;
import function_expr;
import column_expr;
import bound_select_statement;

export module project_binder;

namespace hybridsearch {

export class ProjectBinder final : public ExpressionBinder {
public:
    explicit ProjectBinder(QueryContext *query_context) : ExpressionBinder(query_context) {}
    ProjectBinder(QueryContext *query_context, BoundSelectStatement *bound_select_statement)
        : ExpressionBinder(query_context), bound_select_statement_(bound_select_statement) {}

    // Bind expression entry
    SharedPtr<BaseExpression> BuildExpression(const ParsedExpr &expr, BindContext *bind_context_ptr, i64 depth, bool root) final;

    SharedPtr<BaseExpression> BuildFuncExpr(const FunctionExpr &expr, BindContext *bind_context_ptr, i64 depth, bool root) final;

    SharedPtr<BaseExpression> BuildColExpr(const ColumnExpr &expr, BindContext *bind_context_ptr, i64 depth, bool root) final;

    [[nodiscard]] const String &BoundColumn() const { return bound_column_name_; }

    SharedPtr<BaseExpression> BuildUnnestExpr(const FunctionExpr &expr, BindContext *bind_context_ptr, i64 depth, bool root) final;

private:
    bool binding_agg_func_ = false;
    String bound_column_name_;

    BoundSelectStatement *bound_select_statement_ = nullptr;
};

} // namespace hybridsearch
