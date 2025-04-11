

module;

import stl;
import txn;
import query_context;
import table_def;
import data_table;

import physical_operator_type;
import operator_state;
import wal_manager;
import hybridsearch_context;
import status;

module physical_create_view;

namespace hybridsearch {

void PhysicalCreateView::Init(QueryContext* query_context) {}

bool PhysicalCreateView::Execute(QueryContext *, OperatorState *operator_state) {
    StorageMode storage_mode = hybridsearchContext::instance().storage()->GetStorageMode();
    if (storage_mode == StorageMode::kUnInitialized) {
        UnrecoverableError("Uninitialized storage mode");
    }

    if (storage_mode != StorageMode::kWritable) {
        operator_state->status_ = Status::InvalidNodeRole("Attempt to write on non-writable node");
        operator_state->SetComplete();
        return true;
    }

    operator_state->SetComplete();
    return true;
}

} // namespace hybridsearch
