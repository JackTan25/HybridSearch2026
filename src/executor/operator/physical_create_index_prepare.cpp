

module;

#include <tuple>
#include <vector>
#include <thread>
module physical_create_index_prepare;

import stl;
import third_party;
import physical_operator_type;
import physical_operator;
import query_context;
import operator_state;
import load_meta;
import logger;
import status;
import hybridsearch_exception;
import index_base;
import index_file_worker;
import buffer_manager;
import buffer_handle;
import index_hnsw;
import default_values;
import txn_store;
import base_table_ref;
import extra_ddl_info;
import wal_manager;
import hybridsearch_context;
import table_entry;
import table_index_entry;

namespace hybridsearch {
PhysicalCreateIndexPrepare::PhysicalCreateIndexPrepare(u64 id,
                                                       SharedPtr<BaseTableRef> base_table_ref,
                                                       SharedPtr<IndexBase> index_definition,
                                                       ConflictType conflict_type,
                                                       SharedPtr<Vector<String>> output_names,
                                                       SharedPtr<Vector<SharedPtr<DataType>>> output_types,
                                                       SharedPtr<Vector<LoadMeta>> load_metas,
                                                       bool prepare)
    : PhysicalOperator(PhysicalOperatorType::kCreateIndexPrepare, nullptr, nullptr, id, load_metas), base_table_ref_(base_table_ref),
      index_def_ptr_(index_definition), conflict_type_(conflict_type), output_names_(output_names), output_types_(output_types), prepare_(prepare) {}

void PhysicalCreateIndexPrepare::Init(QueryContext *query_context) {}

bool PhysicalCreateIndexPrepare::Execute(QueryContext *query_context, OperatorState *operator_state) {
    StorageMode storage_mode = hybridsearchContext::instance().storage()->GetStorageMode();
    if (storage_mode == StorageMode::kUnInitialized) {
        UnrecoverableError("Uninitialized storage mode");
    }
    LOG_INFO(fmt::format("Create Index FullText Index"));
    if (storage_mode != StorageMode::kWritable) {
        operator_state->status_ = Status::InvalidNodeRole("Attempt to write on non-writable node");
        operator_state->SetComplete();
        return true;
    }

    auto *txn = query_context->GetTxn();
    auto *table_info = base_table_ref_->table_info_.get();

    auto [table_entry, get_table_status] = txn->GetTableByName(*table_info->db_name_, *table_info->table_name_);
    if (!get_table_status.ok()) {
        operator_state->status_ = get_table_status;
        RecoverableError(get_table_status);
    }
    monitor_status = 0;
    std::thread(monitorMemoryUsage,std::ref(monitor_status),"/home/ubuntu/hybridsearch/experiments/peak_memory_index_file").detach();
    auto [table_index_entry, create_index_status] = txn->CreateIndexDef(table_entry, index_def_ptr_, conflict_type_);
    if (!create_index_status.ok()) {
        operator_state->status_ = create_index_status;
    } else {
        auto [segment_index_entries, prepare_status] = txn->CreateIndexPrepare(table_index_entry, base_table_ref_.get(), prepare_);
        if (!prepare_status.ok()) {
            operator_state->status_ = prepare_status;
            return true;
        }
        for (auto *segment_index_entry : segment_index_entries) {
            base_table_ref_->index_index_->Insert(table_index_entry, segment_index_entry);
        }
        if (!prepare_) {
            auto finish_status = txn->CreateIndexFinish(table_entry, table_index_entry);
            if (!finish_status.ok()) {
                operator_state->status_ = finish_status;
                return true;
            }
        }
    }
    monitor_status = 1;
    operator_state->SetComplete();
    return true;
}
} // namespace hybridsearch
