

module;

module physical_drop_view;

import stl;
import txn;
import query_context;
import table_def;
import data_table;

import wal_manager;
import hybridsearch_context;
import status;
import operator_state;

namespace hybridsearch {

void PhysicalDropView::Init(QueryContext* query_context) {}

bool PhysicalDropView::Execute(QueryContext *, OperatorState *operator_state) {
    StorageMode storage_mode = hybridsearchContext::instance().storage()->GetStorageMode();
    if (storage_mode == StorageMode::kUnInitialized) {
        UnrecoverableError("Uninitialized storage mode");
    }

    if (storage_mode != StorageMode::kWritable) {
        operator_state->status_ = Status::InvalidNodeRole("Attempt to write on non-writable node");
        operator_state->SetComplete();
    }
    return true;
}

} // namespace hybridsearch
