

module;

module snapshot;

import stl;
import txn;
import query_context;
import table_entry;
import status;
import third_party;
import config;
import hybridsearch_exception;
import snapshot_info;

namespace hybridsearch {

Status Snapshot::CreateTableSnapshot(QueryContext *query_context, const String &snapshot_name, const String &table_name) {
    Txn *txn_ptr = query_context->GetTxn();
    const String &db_name = query_context->schema_name();

    SharedPtr<TableSnapshotInfo> table_snapshot;
    Status status;
    std::tie(table_snapshot, status) = txn_ptr->GetTableSnapshot(db_name, table_name);
    if (!status.ok()) {
        RecoverableError(status);
    }
    table_snapshot->snapshot_name_ = snapshot_name;
    String snapshot_dir = query_context->global_config()->SnapshotDir();
    table_snapshot->Serialize(snapshot_dir);

    return Status::OK();
}

Status Snapshot::RestoreTableSnapshot(QueryContext *query_context, const String &snapshot_name) {
    Txn *txn_ptr = query_context->GetTxn();
    String snapshot_dir = query_context->global_config()->SnapshotDir();

    SharedPtr<TableSnapshotInfo> table_snapshot;
    Status status;
    std::tie(table_snapshot, status) = TableSnapshotInfo::Deserialize(snapshot_dir, snapshot_name);
    if(!status.ok()) {
        return status;
    }
    txn_ptr->ApplyTableSnapshot(table_snapshot);
    return Status::OK();
}

} // namespace hybridsearch