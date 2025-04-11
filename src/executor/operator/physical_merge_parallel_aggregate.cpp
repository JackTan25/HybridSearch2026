

module;

import query_context;
import operator_state;

module physical_merge_parallel_aggregate;

namespace hybridsearch {

void PhysicalMergeParallelAggregate::Init(QueryContext* query_context) {}

bool PhysicalMergeParallelAggregate::Execute(QueryContext *, OperatorState *) { return true; }

} // namespace hybridsearch
