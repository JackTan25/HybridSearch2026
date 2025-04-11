

module;

export module result_cache_getter;

import stl;
import logical_node;
import query_context;
import optimizer_rule;

namespace hybridsearch {

export class ResultCacheGetter final : public OptimizerRule {
public:
    void ApplyToPlan(QueryContext *query_context_ptr, SharedPtr<LogicalNode> &logical_plan) override;

    String name() const override { return "Result Cache Getter"; }

private:
    void VisitNode(SharedPtr<LogicalNode> &op);
};

} // namespace hybridsearch
