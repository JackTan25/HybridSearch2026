

module;

import query_context;
import operator_state;

module physical_sort_merge_join;

namespace hybridsearch {

void PhysicalSortMergeJoin::Init(QueryContext* query_context) {}

bool PhysicalSortMergeJoin::Execute(QueryContext *, OperatorState *) { return true; }

} // namespace hybridsearch
