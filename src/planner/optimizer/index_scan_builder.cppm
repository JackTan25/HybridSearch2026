

module;

export module index_scan_builder;

import stl;
import logical_node;
import query_context;
import optimizer_rule;

namespace hybridsearch {

export class IndexScanBuilder final : public OptimizerRule {
public:
    ~IndexScanBuilder() final = default;

    void ApplyToPlan(QueryContext *query_context_ptr, SharedPtr<LogicalNode> &logical_plan) override;

    String name() const final { return "Build SecondaryIndexScan"; }
};

} // namespace hybridsearch