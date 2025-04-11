

module;

import stl;

import physical_operator_type;
import physical_operator;
import query_context;
import operator_state;
import load_meta;

import create_index_data;
import base_table_ref;
import status;
import hybridsearch_exception;
import buffer_handle;
import index_hnsw;
import index_base;
import buffer_manager;
import txn_store;
import third_party;
import logger;
import wal_manager;
import hybridsearch_context;

module physical_create_index_do;

namespace hybridsearch {
PhysicalCreateIndexDo::PhysicalCreateIndexDo(u64 id,
                                             UniquePtr<PhysicalOperator> left,
                                             SharedPtr<BaseTableRef> base_table_ref,
                                             SharedPtr<String> index_name,
                                             SharedPtr<Vector<String>> output_names,
                                             SharedPtr<Vector<SharedPtr<DataType>>> output_types,
                                             SharedPtr<Vector<LoadMeta>> load_metas)
    : PhysicalOperator(PhysicalOperatorType::kCreateIndexDo, std::move(left), nullptr, id, load_metas), base_table_ref_(base_table_ref),
      index_name_(index_name), output_names_(output_names), output_types_(output_types) {}

void PhysicalCreateIndexDo::Init(QueryContext* query_context) {}

// FIXME: fetch and add a block one time
bool PhysicalCreateIndexDo::Execute(QueryContext *query_context, OperatorState *operator_state) {
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
    auto *create_index_do_state = static_cast<CreateIndexDoOperatorState *>(operator_state);
    auto &create_index_idxes = create_index_do_state->create_index_shared_data_->create_index_idxes_;

    auto status = txn->CreateIndexDo(base_table_ref_.get(), *index_name_, create_index_idxes);
    if (!status.ok()) {
        operator_state->status_ = status;
        return false;
    }
    operator_state->SetComplete();

    return true;
}

}; // namespace hybridsearch