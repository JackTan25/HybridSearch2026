

module;

import stl;

import base_expression;
import bind_context;
import expression_binder;
import parsed_expr;
import knn_expr;

export module aggregate_binder;

namespace hybridsearch {

class AggregateBinder : public ExpressionBinder {
public:
    //    explicit AggregateBinder(PlanBuilder& plan_builder) : ExpressionBinder(plan_builder) {}

    // Bind expression entry
    SharedPtr<BaseExpression> BuildExpression(const ParsedExpr &expr, BindContext *bind_context_ptr, i64 depth, bool root) override;

    //    // Bind column reference expression also include correlated column reference.
    //    SharedPtr<BaseExpression>
    //    BuildColRefExpr(const hsql::Expr &expr, const SharedPtr<BindContext>& bind_context_ptr) override;

    SharedPtr<BaseExpression> BuildKnnExpr(const KnnExpr &expr, BindContext *bind_context_ptr, i64 depth, bool root) override;

private:
};

} // namespace hybridsearch
