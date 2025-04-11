

module;

#include <vector>

module physical_compact_index_prepare;

import stl;
import operator_state;
import query_context;
import table_index_entry;
import block_index;
import txn;
import status;
import base_table_ref;
import wal_manager;
import hybridsearch_context;
import hybridsearch_exception;

namespace hybridsearch {

bool PhysicalCompactIndexPrepare::Execute(QueryContext *query_context, OperatorState *operator_state) {
    StorageMode storage_mode = hybridsearchContext::instance().storage()->GetStorageMode();
    if (storage_mode == StorageMode::kUnInitialized) {
        UnrecoverableError("Uninitialized storage mode");
    }

    if (storage_mode != StorageMode::kWritable) {
        operator_state->status_ = Status::InvalidNodeRole("Attempt to write on non-writable node");
        operator_state->SetComplete();
        return true;
    }

    auto *compact_index_prepare_operator_state = static_cast<CompactIndexPrepareOperatorState *>(operator_state);
    auto *compact_state_data = compact_index_prepare_operator_state->compact_state_data_.get();
    BaseTableRef *new_table_ref = compact_state_data->GetNewTableRef();
    if (new_table_ref->block_index_->IsEmpty()) { // compact yield no new segment
        operator_state->SetComplete();
        return true;
    }

    auto *index_index = base_table_ref_->index_index_.get();

    SizeT create_index_idx = compact_index_prepare_operator_state->create_index_idx_;
    if (create_index_idx == index_index->index_snapshots_vec_.size()) {
        operator_state->SetComplete();
        return true;
    }
    auto *table_index_entry = index_index->index_snapshots_vec_[create_index_idx]->table_index_entry_;

    const auto *create_index_shared_data = compact_index_prepare_operator_state->create_index_shared_data_.get();
    if (create_index_shared_data != nullptr) {
        (*create_index_shared_data)[create_index_idx]->Init(new_table_ref->block_index_.get());
    }

    Txn *txn = query_context->GetTxn();
    auto [segment_index_entries, status] = txn->CreateIndexPrepare(table_index_entry, new_table_ref, prepare_, false);
    if (!status.ok()) {
        operator_state->status_ = status;
        return true;
    }
    for (auto *segment_index_entry : segment_index_entries) {
        compact_state_data->AddNewIndexSegment(table_index_entry, segment_index_entry);
    }

    compact_index_prepare_operator_state->create_index_idx_ = ++create_index_idx;
    if (create_index_idx == index_index->index_snapshots_vec_.size()) {
        compact_index_prepare_operator_state->SetComplete();
    }
    return true;
}

} // namespace hybridsearch