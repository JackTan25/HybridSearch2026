

module;

import stl;
import column_binding;
import logical_node;
import base_expression;
import subquery_expression;
import column_expression;

export module subquery_unnest;

namespace hybridsearch {

class QueryContext;
class BindContext;

export class SubqueryUnnest {
public:
    static void UnnestSubqueries(SharedPtr<BaseExpression> &expr_ptr,
                                 SharedPtr<LogicalNode> &root,
                                 QueryContext *query_context,
                                 const SharedPtr<BindContext> &bind_context);

    static SharedPtr<BaseExpression> UnnestSubquery(SharedPtr<BaseExpression> &expr_ptr,
                                                    SharedPtr<LogicalNode> &root,
                                                    QueryContext *query_context,
                                                    const SharedPtr<BindContext> &bind_context);

    static SharedPtr<BaseExpression> UnnestUncorrelated(SubqueryExpression *expr_ptr,
                                                        SharedPtr<LogicalNode> &root,
                                                        SharedPtr<LogicalNode> &subquery_plan,
                                                        QueryContext *query_context,
                                                        const SharedPtr<BindContext> &bind_context);

    static SharedPtr<BaseExpression> UnnestCorrelated(SubqueryExpression *expr_ptr,
                                                      SharedPtr<LogicalNode> &root,
                                                      SharedPtr<LogicalNode> &subquery_plan,
                                                      QueryContext *query_context,
                                                      const SharedPtr<BindContext> &bind_context);

private:
    static void GenerateJoinConditions(QueryContext *query_context,
                                       Vector<SharedPtr<BaseExpression>> &conditions,
                                       const Vector<SharedPtr<ColumnExpression>> &correlated_columns,
                                       const Vector<ColumnBinding> &subplan_column_bindings,
                                       SizeT correlated_base_index);
};

} // namespace hybridsearch
