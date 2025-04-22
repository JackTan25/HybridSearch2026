

module;

module physical_unnest_aggregate;

import stl;
import txn;
import query_context;
import table_def;
import data_table;

import physical_operator_type;
import operator_state;
import expression_state;
import expression_selector;
import data_block;
import logger;
import third_party;

import hybridsearch_exception;

namespace hybridsearch {

void PhysicalUnnestAggregate::Init(QueryContext* query_context) {}

bool PhysicalUnnestAggregate::Execute(QueryContext *, OperatorState *operator_state) {
    OperatorState *prev_op_state = operator_state->prev_op_state_;
    if (prev_op_state->Complete()) {
        operator_state->SetComplete();
    }
    return true;
}

} // namespace hybridsearch
