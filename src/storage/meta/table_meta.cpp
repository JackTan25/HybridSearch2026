

module;

#include <vector>

module table_meta;

import stl;
import table_entry_type;

import logger;
import default_values;
import third_party;
import txn_state;
import txn_manager;
import buffer_manager;
import catalog_delta_entry;
import third_party;
import status;
import hybridsearch_exception;
import column_def;
import block_index;
import hybridsearch_context;

namespace hybridsearch {

UniquePtr<TableMeta> TableMeta::NewTableMeta(const SharedPtr<String> &db_entry_dir, const SharedPtr<String> &table_name, DBEntry *db_entry) {
    auto table_meta = MakeUnique<TableMeta>(db_entry_dir, table_name, db_entry);

    return table_meta;
}
/**
 * @brief Create a new table entry.
 *        LIST: [👇(insert a new).... table_entry2 , table_entry1 , dummy_entry] insert new table entry from front.
 * @param table_meta
 * @param table_entry_type
 * @param table_collection_name_ptr
 * @param columns
 * @param txn_id
 * @param begin_ts
 * @param txn_mgr
 * @return Status
 */
Tuple<TableEntry *, Status> TableMeta::CreateEntry(std::shared_lock<std::shared_mutex> &&r_lock,
                                                   TableEntryType table_entry_type,
                                                   const SharedPtr<String> &table_name,
                                                   const SharedPtr<String> &table_comment,
                                                   const Vector<SharedPtr<ColumnDef>> &columns,
                                                   TransactionID txn_id,
                                                   TxnTimeStamp begin_ts,
                                                   TxnManager *txn_mgr,
                                                   ConflictType conflict_type) {
    auto init_table_entry = [&](TransactionID txn_id, TxnTimeStamp begin_ts) {
        return TableEntry::NewTableEntry(false, this->db_entry_dir_, table_name, table_comment, columns, table_entry_type, this, txn_id, begin_ts);
    };
    return table_entry_list_.AddEntry(std::move(r_lock), std::move(init_table_entry), txn_id, begin_ts, txn_mgr, conflict_type);
}

Tuple<SharedPtr<TableEntry>, Status> TableMeta::DropEntry(std::shared_lock<std::shared_mutex> &&r_lock,
                                                          TransactionID txn_id,
                                                          TxnTimeStamp begin_ts,
                                                          TxnManager *txn_mgr,
                                                          const String &table_name,
                                                          ConflictType conflict_type) {
    auto init_drop_entry = [&](TransactionID txn_id, TxnTimeStamp begin_ts) {
        Vector<SharedPtr<ColumnDef>> dummy_columns;
        return TableEntry::NewTableEntry(true,
                                         this->db_entry_dir_,
                                         this->table_name_,
                                         MakeShared<String>(),
                                         dummy_columns,
                                         TableEntryType::kTableEntry,
                                         this,
                                         txn_id,
                                         begin_ts);
    };
    return table_entry_list_.DropEntry(std::move(r_lock), std::move(init_drop_entry), txn_id, begin_ts, txn_mgr, conflict_type);
}

Tuple<SharedPtr<TableInfo>, Status> TableMeta::GetTableInfo(std::shared_lock<std::shared_mutex> &&r_lock, Txn *txn) {
    TransactionID txn_id = txn->TxnID();
    TxnTimeStamp begin_ts = txn->BeginTS();
    auto [table_entry, status] = table_entry_list_.GetEntry(std::move(r_lock), txn_id, begin_ts);
    if (!status.ok()) {
        return {nullptr, status};
    }

    return {table_entry->GetTableInfo(txn), status};
}

Status TableMeta::AddEntry(std::shared_lock<std::shared_mutex> &&r_lock,
                           SharedPtr<TableEntry> table_entry,
                           TransactionID txn_id,
                           TxnTimeStamp begin_ts,
                           TxnManager *txn_mgr,
                           bool add_if_found) {
    auto [table_entry_ptr, status] = table_entry_list_.AddEntry(
        std::move(r_lock),
        [&](TransactionID txn_id, TxnTimeStamp begin_ts) {
            table_entry->txn_id_ = txn_id;
            table_entry->begin_ts_ = begin_ts;
            return table_entry;
        },
        txn_id,
        begin_ts,
        txn_mgr,
        ConflictType::kError,
        add_if_found);
    return status;
}

void TableMeta::DeleteEntry(TransactionID txn_id) { auto erase_list = table_entry_list_.DeleteEntry(txn_id); }

void TableMeta::CreateEntryReplay(std::function<SharedPtr<TableEntry>(TransactionID, TxnTimeStamp)> &&init_entry,
                                  TransactionID txn_id,
                                  TxnTimeStamp begin_ts) {
    auto [entry, status] = table_entry_list_.AddEntryReplay(std::move(init_entry), txn_id, begin_ts);
    if (!status.ok()) {
        UnrecoverableError(status.message());
    }
}

Status TableMeta::ApplyTableSnapshot(std::function<SharedPtr<TableEntry>(TransactionID, TxnTimeStamp)> &&restore_entry,
                                     TransactionID txn_id,
                                     TxnTimeStamp begin_ts) {

    auto [entry, status] = table_entry_list_.ApplySnapshot(std::move(restore_entry), txn_id, begin_ts);
    if (!status.ok()) {
        RecoverableError(status);
    }
    return Status::OK();
}

void TableMeta::UpdateEntryReplay(std::function<void(SharedPtr<TableEntry>, TransactionID, TxnTimeStamp)> &&update_entry,
                                  TransactionID txn_id,
                                  TxnTimeStamp begin_ts) {
    auto status = table_entry_list_.UpdateEntryReplay(std::move(update_entry), txn_id, begin_ts);
    if (!status.ok()) {
        UnrecoverableError(status.message());
    }
}

void TableMeta::DropEntryReplay(std::function<SharedPtr<TableEntry>(TransactionID, TxnTimeStamp)> &&init_entry,
                                TransactionID txn_id,
                                TxnTimeStamp begin_ts) {
    auto [dropped_entry, status] = table_entry_list_.DropEntryReplay(std::move(init_entry), txn_id, begin_ts);
    if (!status.ok()) {
        UnrecoverableError(status.message());
    }
}

TableEntry *TableMeta::GetEntryReplay(TransactionID txn_id, TxnTimeStamp begin_ts) {
    auto [entry, status] = table_entry_list_.GetEntryReplay(txn_id, begin_ts);
    if (!status.ok()) {
        UnrecoverableError(status.message());
    }
    return entry;
}

const SharedPtr<String> &TableMeta::db_name_ptr() const { return db_entry_->db_name_ptr(); }

SharedPtr<String> TableMeta::ToString() {
    SharedPtr<String> res = MakeShared<String>(
        fmt::format("TableMeta, db_entry_dir: {}, table name: {}, entry count: {}", *db_entry_dir_, *table_name_, table_entry_list_.size()));
    return res;
}

nlohmann::json TableMeta::Serialize(TxnTimeStamp max_commit_ts) {
    nlohmann::json json_res;

    json_res["db_entry_dir"] = *this->db_entry_dir_;
    json_res["table_name"] = *this->table_name_;

    Vector<BaseEntry *> entry_candidates = table_entry_list_.GetCandidateEntry(max_commit_ts, EntryType::kTable);

    for (const auto &entry : entry_candidates) {
        TableEntry *table_entry = static_cast<TableEntry *>(entry);
        json_res["table_entries"].emplace_back(table_entry->Serialize(max_commit_ts));
    }

    return json_res;
}

/**
 * @brief Deserialize the table meta from json.
 *        The table meta is a list of table entries in reverse order.
 *        Same as CreateNewEntry, the last entry is a dummy entry.
 *        LIST: [👇(a new entry).... table_entry2 , table_entry1 , dummy_entry]
 *        The raw catalog is json, so the dummy entry is not included.
 *        The dummy entry is added during the deserialization.
 * @param table_meta_json
 * @param db_entry
 * @param buffer_mgr
 * @return UniquePtr<TableMeta>
 */
UniquePtr<TableMeta> TableMeta::Deserialize(const nlohmann::json &table_meta_json, DBEntry *db_entry, BufferManager *buffer_mgr) {
    SharedPtr<String> db_entry_dir = MakeShared<String>(table_meta_json["db_entry_dir"]);
    SharedPtr<String> table_name = MakeShared<String>(table_meta_json["table_name"]);
    LOG_TRACE(fmt::format("load table {}", *table_name));
    UniquePtr<TableMeta> table_meta = MakeUnique<TableMeta>(db_entry_dir, table_name, db_entry);
    if (table_meta_json.contains("table_entries")) {
        for (const auto &table_entry_json : table_meta_json["table_entries"]) {
            UniquePtr<TableEntry> table_entry = TableEntry::Deserialize(table_entry_json, table_meta.get(), buffer_mgr);
            table_meta->PushBackEntry(std::move(table_entry));
        }
    }
    table_meta->Sort();

    return table_meta;
}

void TableMeta::Sort() { table_entry_list_.SortEntryListByTS(); }

void TableMeta::PushBackEntry(const SharedPtr<TableEntry> &new_table_entry) { table_entry_list_.PushBackEntry(new_table_entry); }

void TableMeta::Cleanup(CleanupInfoTracer *info_tracer, bool dropped) { table_entry_list_.Cleanup(info_tracer, dropped); }

bool TableMeta::PickCleanup(CleanupScanner *scanner) {
    LOG_DEBUG(fmt::format("Pick cleanup for table: {}, entry_list size: {}", *table_name_, table_entry_list_.size()));
    return table_entry_list_.PickCleanup(scanner);
}

} // namespace hybridsearch
