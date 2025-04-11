

module;

import query_context;
import operator_state;

module physical_dummy_scan;

namespace hybridsearch {

void PhysicalDummyScan::Init(QueryContext* query_context) {}

bool PhysicalDummyScan::Execute(QueryContext *, OperatorState *) { return true; }

} // namespace hybridsearch
