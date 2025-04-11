

module;

export module apply_fast_rough_filter;

import stl;
import logical_node;
import query_context;
import optimizer_rule;

namespace hybridsearch {

export class ApplyFastRoughFilter final : public OptimizerRule {
public:
    ~ApplyFastRoughFilter() final = default;

    void ApplyToPlan(QueryContext *, SharedPtr<LogicalNode> &logical_plan) final;

    String name() const final { return "Apply FastRoughFilter"; }
};

} // namespace hybridsearch
