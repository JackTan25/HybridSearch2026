

module;

export module optimizer;

import stl;
import optimizer_rule;
import logical_node;
import base_statement;
import global_resource_usage;

namespace hybridsearch {

class QueryContext;

export class Optimizer {
public:
    explicit Optimizer(QueryContext *query_context_ptr);

    ~Optimizer() {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::DecrObjectCount("Optimizer");
#endif
    }

    void AddRule(UniquePtr<OptimizerRule> rule);

    void optimize(SharedPtr<LogicalNode> &unoptimized_plan, StatementType statement_type);

public:
    QueryContext *query_context_ptr_{};

    Vector<UniquePtr<OptimizerRule>> rules_{};
};

} // namespace hybridsearch
