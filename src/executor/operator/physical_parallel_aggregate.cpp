

module;

import query_context;
import operator_state;

module physical_parallel_aggregate;

namespace hybridsearch {

void PhysicalParallelAggregate::Init(QueryContext* query_context) {}

bool PhysicalParallelAggregate::Execute(QueryContext *, OperatorState *) { return true; }

} // namespace hybridsearch
