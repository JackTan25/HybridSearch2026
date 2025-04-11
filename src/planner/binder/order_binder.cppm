

module;

import stl;

import base_expression;
import bind_context;
import expression_binder;
import query_context;
import parsed_expr;

export module order_binder;

namespace hybridsearch {

export class OrderBinder : public ExpressionBinder {
public:
    static void PushExtraExprToSelectList(ParsedExpr *expr, const SharedPtr<BindContext> &bind_context_ptr);

public:
    explicit OrderBinder(QueryContext *query_context) : ExpressionBinder(query_context) {}

    // Bind expression entry
    SharedPtr<BaseExpression> BuildExpression(const ParsedExpr &expr, BindContext *bind_context_ptr, i64 depth, bool root) override;

private:
};

} // namespace hybridsearch
