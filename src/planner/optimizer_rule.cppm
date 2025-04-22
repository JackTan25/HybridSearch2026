

module;

import logical_node;
import stl;

export module optimizer_rule;

namespace hybridsearch {

class QueryContext;
export class OptimizerRule {
public:
    virtual void ApplyToPlan(QueryContext *query_context_ptr, SharedPtr<LogicalNode> &logical_plan) = 0;

    virtual String name() const = 0;

    virtual ~OptimizerRule() = default;
};

} // namespace hybridsearch
