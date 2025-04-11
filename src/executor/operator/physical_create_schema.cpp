

module;

module physical_create_schema;

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

namespace hybridsearch {

void PhysicalCreateSchema::Init(QueryContext* query_context) {}

bool PhysicalCreateSchema::Execute(QueryContext *query_context, OperatorState *operator_state) {
    StorageMode storage_mode = hybridsearchContext::instance().storage()->GetStorageMode();
    if (storage_mode == StorageMode::kUnInitialized) {
        UnrecoverableError("Uninitialized storage mode");
    }

    if (storage_mode != StorageMode::kWritable) {
        operator_state->status_ = Status::InvalidNodeRole("Attempt to write on non-writable node");
        operator_state->SetComplete();
        return true;
    }

    auto txn = query_context->GetTxn();
    Status status = txn->CreateDatabase(schema_name_, conflict_type_, comment_);
    if (!status.ok()) {
        operator_state->status_ = status;
    }
    operator_state->SetComplete();
    return true;
}

} // namespace hybridsearch
