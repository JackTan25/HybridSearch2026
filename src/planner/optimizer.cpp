

module;

import stl;
import logical_node_type;
import logical_explain;
import column_remapper;
import column_pruner;
import lazy_load;
import index_scan_builder;
import apply_fast_rough_filter;
import explain_logical_plan;
import optimizer_rule;
import bound_delete_statement;
import bound_update_statement;
import logical_node;
import explain_statement;
import logical_node_type;
import base_statement;
import result_cache_getter;
import global_resource_usage;

module optimizer;

namespace hybridsearch {

Optimizer::Optimizer(QueryContext *query_context_ptr) : query_context_ptr_(query_context_ptr) {
    // TODO: need an equivalent expression optimizer
    AddRule(MakeUnique<ApplyFastRoughFilter>()); // put it before SecondaryIndexScanBuilder
    AddRule(MakeUnique<IndexScanBuilder>());     // put it before ColumnPruner, necessary for filter_fulltext and index_scan
    AddRule(MakeUnique<ColumnPruner>());
    AddRule(MakeUnique<LazyLoad>());
    AddRule(MakeUnique<ColumnRemapper>());
    if (query_context_ptr->storage()->result_cache_manager()) {
        AddRule(MakeUnique<ResultCacheGetter>()); // put after column pruner, column remapper
    }

#ifdef hybridsearch_DEBUG
    GlobalResourceUsage::IncrObjectCount("Optimizer");
#endif
}

void Optimizer::AddRule(UniquePtr<OptimizerRule> rule) { rules_.emplace_back(std::move(rule)); }

void Optimizer::optimize(SharedPtr<LogicalNode> &unoptimized_plan, StatementType statement_type) {
    // Expression folding should be done in logical planner before optimizer
    // Non-select plan, the root node won't be project.
    switch (statement_type) {
        case StatementType::kSelect:
        case StatementType::kExplain:
        case StatementType::kDelete:
        case StatementType::kUpdate: {
            ; // continue;
            break;
        }
        default: {
            return;
        }
    }

    // Only work for select
    SizeT rule_count = rules_.size();
    for (SizeT idx = 0; idx < rule_count; ++idx) {
        const auto &rule = rules_[idx];
        rule->ApplyToPlan(query_context_ptr_, unoptimized_plan);
    }

    if (unoptimized_plan->operator_type() == LogicalNodeType::kExplain) {
        LogicalExplain *explain_node = (LogicalExplain *)(unoptimized_plan.get());
        if (explain_node->explain_type() == ExplainType::kOpt) {
            SharedPtr<Vector<SharedPtr<String>>> texts_ptr = MakeShared<Vector<SharedPtr<String>>>();
            ExplainLogicalPlan::Explain(explain_node->left_node().get(), texts_ptr);
            explain_node->SetText(texts_ptr);
        }
    }

    return;
}

} // namespace hybridsearch