

module;

module physical_drop_table;

import stl;
import txn;
import query_context;
import table_def;
import data_table;
import result_cache_manager;
import physical_operator_type;
import operator_state;
import status;
import hybridsearch_exception;
import logical_type;
import column_def;
import wal_manager;
import hybridsearch_context;

namespace hybridsearch {

void PhysicalDropTable::Init(QueryContext* query_context) {}

bool PhysicalDropTable::Execute(QueryContext *query_context, OperatorState *operator_state) {
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

    Status status = txn->DropTableCollectionByName(*schema_name_, *table_name_, conflict_type_);
    if (ResultCacheManager *cache_mgr = query_context->storage()->result_cache_manager(); cache_mgr != nullptr) {
        cache_mgr->DropTable(*schema_name_, *table_name_);
    }

    if (!status.ok()) {
        operator_state->status_ = status;
    }

    // Generate the result
    Vector<SharedPtr<ColumnDef>> column_defs = {
        MakeShared<ColumnDef>(0, MakeShared<DataType>(LogicalType::kInteger), "OK", std::set<ConstraintType>())};

    auto result_table_def_ptr = TableDef::Make(MakeShared<String>("default_db"), MakeShared<String>("Tables"), nullptr, column_defs);
    output_ = MakeShared<DataTable>(result_table_def_ptr, TableType::kDataTable);
    operator_state->SetComplete();
    return true;
}

} // namespace hybridsearch
