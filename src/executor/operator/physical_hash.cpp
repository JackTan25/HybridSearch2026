

module;

import query_context;
import operator_state;

module physical_hash;

namespace hybridsearch {

void PhysicalHash::Init(QueryContext* query_context) {}

bool PhysicalHash::Execute(QueryContext *, OperatorState *) { return true; }

} // namespace hybridsearch
