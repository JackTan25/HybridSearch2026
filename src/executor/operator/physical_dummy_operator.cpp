

module;

import query_context;
import operator_state;

module physical_dummy_operator;

namespace hybridsearch {

void PhysicalDummyOperator::Init(QueryContext* query_context) {}

bool PhysicalDummyOperator::Execute(QueryContext *, OperatorState *) { return true; }

} // namespace hybridsearch
