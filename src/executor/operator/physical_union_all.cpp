

module;

import query_context;
import operator_state;

module physical_union_all;

namespace hybridsearch {

void PhysicalUnionAll::Init(QueryContext* query_context) {}

bool PhysicalUnionAll::Execute(QueryContext *, OperatorState *) { return true; }

} // namespace hybridsearch
