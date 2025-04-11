

module;

import query_context;
import operator_state;

module physical_intersect;

namespace hybridsearch {

void PhysicalIntersect::Init(QueryContext* query_context) {}

bool PhysicalIntersect::Execute(QueryContext *, OperatorState *) { return true; }

} // namespace hybridsearch
