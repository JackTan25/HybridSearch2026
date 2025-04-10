

module;

#include <sstream>
#include <vector>

module physical_compact_finish;

import stl;
import operator_state;
import table_entry;
import segment_entry;
import base_table_ref;
import compact_state_data;
import wal_manager;
import wal_entry;
import logger;
import txn;
import internal_types;
import hybridsearch_context;
import hybridsearch_exception;
import status;
import txn_store;
import segment_index_entry;

namespace hybridsearch {

bool PhysicalCompactFinish::Execute(QueryContext *query_context, OperatorState *operator_state) {
    StorageMode storage_mode = hybridsearchContext::instance().storage()->GetStorageMode();
    if (storage_mode == StorageMode::kUnInitialized) {
        UnrecoverableError("Uninitialized storage mode");
    }

    if (storage_mode != StorageMode::kWritable) {
        operator_state->status_ = Status::InvalidNodeRole("Attempt to write on non-writable node");
        operator_state->SetComplete();
        return true;
    }

    auto *compact_finish_operator_state = static_cast<CompactFinishOperatorState *>(operator_state);
    const CompactStateData *compact_state_data = compact_finish_operator_state->compact_state_data_.get();
    if (!ApplyDeletes(query_context, compact_state_data)) {
        LOG_WARN("Failed to apply deletes in compact finish");
        return true;
    }
    SaveSegmentData(query_context, compact_state_data);
    compact_finish_operator_state->SetComplete();
    return true;
}

void PhysicalCompactFinish::SaveSegmentData(QueryContext *query_context, const CompactStateData *compact_state_data) {
    auto *txn = query_context->GetTxn();
    auto [table_entry, status] = txn->GetTableByName(*base_table_ref_->table_info_->db_name_, *base_table_ref_->table_info_->table_name_);
    if(!status.ok()) {
        RecoverableError(status);
    }

    Vector<Pair<SharedPtr<SegmentEntry>, Vector<SegmentEntry *>>> segment_data;

    std::stringstream ss;
    for (const auto &compact_segment_data : compact_state_data->segment_data_list_) {
        auto *new_segment = compact_segment_data.new_segment_.get();
        ss << "Compact ";
        new_segment->FlushNewData();

        for (const auto *old_segment : compact_segment_data.old_segments_) {
            ss << "old segment " << old_segment->segment_id() << ", ";
        }
        ss << "to new segment " << new_segment->segment_id();
        segment_data.emplace_back(compact_segment_data.new_segment_, compact_segment_data.old_segments_);
    }
    LOG_DEBUG(ss.str());

    for (const auto &compact_segment_data : compact_state_data->segment_data_list_) {
        TxnStore *txn_store = txn->txn_store();
        TxnTableStore *txn_table_store = txn_store->GetTxnTableStore(table_entry);
        auto index_map = table_entry->IndexMetaMap();
        for (const auto &[index_name, index_meta] : *index_map) {
            auto [table_index_entry, status] = index_meta->GetEntryNolock(txn->TxnID(), txn->BeginTS());
            if (!status.ok()) {
                continue;
            }
            Vector<SegmentIndexEntry *> segment_index_entries;
            auto &segment_index_map = table_index_entry->index_by_segment();
            for (const auto *old_segment : compact_segment_data.old_segments_) {
                auto iter = segment_index_map.find(old_segment->segment_id());
                if (iter == segment_index_map.end()) {
                    continue;
                }
                auto *segment_index_entry = iter->second.get();
                segment_index_entries.push_back(segment_index_entry);
            }
            txn_table_store->AddSegmentIndexesStore(table_index_entry, std::move(segment_index_entries));
        }
    }

    txn->Compact(table_entry, std::move(segment_data), compact_type_);
}

bool PhysicalCompactFinish::ApplyDeletes(QueryContext *query_context, const CompactStateData *compact_state_data) {
    auto *txn = query_context->GetTxn();
    auto [table_entry, status] = txn->GetTableByName(*base_table_ref_->table_info_->db_name_, *base_table_ref_->table_info_->table_name_);
    if(!status.ok()) {
        RecoverableError(status);
    }

    for (const auto &compact_segment_data : compact_state_data->segment_data_list_) {
        for (auto *old_segment : compact_segment_data.old_segments_) {
            if (!old_segment->SetNoDelete()) {
                return false;
            }
        }
    }
    const Vector<Pair<SegmentID, Vector<SegmentOffset>>> &to_delete = compact_state_data->GetToDelete();
    Vector<RowID> row_ids;
    for (const auto &[segment_id, delete_offsets] : to_delete) {
        for (SegmentOffset offset : delete_offsets) {
            RowID old_row_id(segment_id, offset);
            RowID new_row_id = compact_state_data->remapper_.GetNewRowID(old_row_id);
            row_ids.push_back(new_row_id);
        }
    }
    txn->Delete(*base_table_ref_->table_info_->db_name_, *base_table_ref_->table_info_->table_name_, row_ids, false);
    return true;
}

} // namespace hybridsearch