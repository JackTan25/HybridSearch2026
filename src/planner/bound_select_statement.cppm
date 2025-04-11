

module;

import bound_statement;
import logical_knn_scan;
import base_expression;
import stl;
import bind_context;
import logical_node;
import table_ref;

import query_context;
import search_expression;
import knn_expression;
import select_statement;
import highlighter;

export module bound_select_statement;

namespace hybridsearch {

export struct BoundSelectStatement final : public BoundStatement {
public:
    static inline UniquePtr<BoundSelectStatement> Make(SharedPtr<BindContext> bind_context) {
        return MakeUnique<BoundSelectStatement>(std::move(bind_context));
    }

public:
    inline explicit BoundSelectStatement(SharedPtr<BindContext> bind_context) : bind_context_(std::move(bind_context)) {}

    SharedPtr<LogicalNode> BuildPlan(QueryContext *query_context) final;

    SharedPtr<LogicalKnnScan> BuildInitialKnnScan(SharedPtr<TableRef> &table_ref,
                                                  SharedPtr<KnnExpression> knn_expr,
                                                  QueryContext *query_context,
                                                  const SharedPtr<BindContext> &bind_context);

    SharedPtr<LogicalNode> BuildFrom(SharedPtr<TableRef> &table_ref, QueryContext *query_context, const SharedPtr<BindContext> &bind_context);

    SharedPtr<LogicalNode> BuildBaseTable(SharedPtr<TableRef> &table_ref, QueryContext *query_context, const SharedPtr<BindContext> &bind_context);

    SharedPtr<LogicalNode>
    BuildSubqueryTable(SharedPtr<TableRef> &table_ref, QueryContext *query_context, const SharedPtr<BindContext> &bind_context);

    SharedPtr<LogicalNode>
    BuildCrossProductTable(SharedPtr<TableRef> &table_ref, QueryContext *query_context, const SharedPtr<BindContext> &bind_context);

    SharedPtr<LogicalNode> BuildJoinTable(SharedPtr<TableRef> &table_ref, QueryContext *query_context, const SharedPtr<BindContext> &bind_context);

    SharedPtr<LogicalNode> BuildDummyTable(SharedPtr<TableRef> &table_ref, QueryContext *query_context, const SharedPtr<BindContext> &bind_context);

    SharedPtr<LogicalNode> BuildFilter(SharedPtr<LogicalNode> &root,
                                       Vector<SharedPtr<BaseExpression>> &conditions,
                                       QueryContext *query_context,
                                       const SharedPtr<BindContext> &bind_context);

    SharedPtr<LogicalNode> BuildUnnest(SharedPtr<LogicalNode> &root,
                                       Vector<SharedPtr<BaseExpression>> &expressions,
                                       QueryContext *query_context,
                                       const SharedPtr<BindContext> &bind_context);

    void BuildSubquery(SharedPtr<LogicalNode> &root,
                       SharedPtr<BaseExpression> &condition,
                       QueryContext *query_context,
                       const SharedPtr<BindContext> &bind_context);

    SharedPtr<BaseExpression> UnnestSubquery(SharedPtr<LogicalNode> &root,
                                             SharedPtr<BaseExpression> &condition,
                                             QueryContext *query_context,
                                             const SharedPtr<BindContext> &bind_context);

public:
    SharedPtr<BindContext> bind_context_{};

    // From clause
    SharedPtr<TableRef> table_ref_ptr_{};

    // Search expression
    SharedPtr<SearchExpression> search_expr_{};

    // Where conditions
    Vector<SharedPtr<BaseExpression>> where_conditions_{};

    // Aggregate expression
    Vector<SharedPtr<BaseExpression>> aggregate_expressions_{};

    // Group by expression list
    Vector<SharedPtr<BaseExpression>> group_by_expressions_{};

    // Having expression list
    Vector<SharedPtr<BaseExpression>> having_expressions_{};

    // Project expression list
    Vector<SharedPtr<BaseExpression>> projection_expressions_{};
    bool total_hits_count_flag_{false};

    // Highlight info
    Map<SizeT, SharedPtr<HighlightInfo>> highlight_columns_{};

    // Unnest columns
    Vector<SharedPtr<BaseExpression>> unnest_expressions_{};

    // Order by expression list
    Vector<SharedPtr<BaseExpression>> order_by_expressions_{};
    Vector<OrderType> order_by_types_{};

    // limit expression
    SharedPtr<BaseExpression> limit_expression_{};
    SharedPtr<BaseExpression> offset_expression_{};

    // Pruned output expression list
    Vector<SharedPtr<BaseExpression>> pruned_expression_{};

    bool distinct_ = false;

    //    SharedPtr<LogicalNode>
    //    BuildPlan() override {
    //        return this->logical_plan_;
    //    }

    u64 groupby_index_{0};
    u64 aggregate_index_{0};
    u64 projection_index_{0};
    u64 result_index_{0};
    u64 knn_index_{0};

    // For build subquery
    bool building_subquery_{false};
};

} // namespace hybridsearch
