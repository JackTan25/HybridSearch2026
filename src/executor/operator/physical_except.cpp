

module;

import query_context;
import operator_state;

module physical_except;

namespace hybridsearch {

void PhysicalExcept::Init(QueryContext* query_context) {}

bool PhysicalExcept::Execute(QueryContext *, OperatorState *) { return true; }

} // namespace hybridsearch
