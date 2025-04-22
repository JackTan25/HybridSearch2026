

module;

import query_context;
import operator_state;

module physical_prepared_plan;

namespace hybridsearch {

void PhysicalPreparedPlan::Init(QueryContext* query_context) {}

bool PhysicalPreparedPlan::Execute(QueryContext *, OperatorState *) { return true; }

} // namespace hybridsearch
