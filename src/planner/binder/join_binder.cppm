

module;

import stl;

import base_expression;
import bind_context;
import expression_binder;
import query_context;
import parsed_expr;
import knn_expr;

export module join_binder;

namespace hybridsearch {

export class JoinBinder final : public ExpressionBinder {
public:
    explicit JoinBinder(QueryContext *query_context) : ExpressionBinder(query_context) {}

    // Bind expression entry
    SharedPtr<BaseExpression> BuildExpression(const ParsedExpr &expr, BindContext *bind_context_ptr, i64 depth, bool root) final;

    SharedPtr<BaseExpression> BuildKnnExpr(const KnnExpr &expr, BindContext *bind_context_ptr, i64 depth, bool root) override;

    //    // Bind column reference expression also include correlated column reference.
    //    SharedPtr<BaseExpression>
    //    BuildColRefExpr(const hsql::Expr &expr, const SharedPtr<BindContext>& bind_context_ptr) override;
private:
};

} // namespace hybridsearch
