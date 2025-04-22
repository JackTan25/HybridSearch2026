

module;

import query_context;
import operator_state;

module physical_merge_hash;

namespace hybridsearch {

void PhysicalMergeHash::Init(QueryContext* query_context) {}

bool PhysicalMergeHash::Execute(QueryContext *, OperatorState *) { return true; }

} // namespace hybridsearch
