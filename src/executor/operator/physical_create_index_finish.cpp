

module;

import stl;

import third_party;
import logger;
import physical_operator_type;
import physical_operator;
import query_context;
import operator_state;
import load_meta;
import index_base;
import hybridsearch_exception;
import wal_manager;
import hybridsearch_context;
import status;

module physical_create_index_finish;

namespace hybridsearch {
PhysicalCreateIndexFinish::PhysicalCreateIndexFinish(u64 id,
                                                     UniquePtr<PhysicalOperator> left,
                                                     SharedPtr<String> db_name,
                                                     SharedPtr<String> table_name,
                                                     SharedPtr<IndexBase> index_base,
                                                     SharedPtr<Vector<String>> output_names,
                                                     SharedPtr<Vector<SharedPtr<DataType>>> output_types,
                                                     SharedPtr<Vector<LoadMeta>> load_metas)
    : PhysicalOperator(PhysicalOperatorType::kCreateIndexFinish, std::move(left), nullptr, id, load_metas), db_name_(db_name),
      table_name_(table_name), index_base_(index_base), output_names_(output_names), output_types_(output_types) {}

void PhysicalCreateIndexFinish::Init(QueryContext* query_context) {}

bool PhysicalCreateIndexFinish::Execute(QueryContext *query_context, OperatorState *operator_state) {
    StorageMode storage_mode = hybridsearchContext::instance().storage()->GetStorageMode();
    if (storage_mode == StorageMode::kUnInitialized) {
        UnrecoverableError("Uninitialized storage mode");
    }

    if (storage_mode != StorageMode::kWritable) {
        operator_state->status_ = Status::InvalidNodeRole("Attempt to write on non-writable node");
        operator_state->SetComplete();
        return true;
    }

    auto *txn = query_context->GetTxn();
    auto status = txn->CreateIndexFinish(*db_name_, *table_name_, index_base_);
    if (!status.ok()) {
        RecoverableError(status);
    }
    operator_state->SetComplete();
    return true;
}

} // namespace hybridsearch