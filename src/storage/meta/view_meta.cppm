

module;

export module view_meta;

import stl;
import base_entry;
import status;
import column_def;

namespace hybridsearch {

class TxnManager;

struct DBEntry;

export struct ViewMeta {
public:
    explicit ViewMeta(SharedPtr<String> name, DBEntry *db_entry) : view_name_(std::move(name)), db_entry_(db_entry) {}

public:
    static Status CreateNewEntry(ViewMeta *table_meta,
                                 const SharedPtr<String> &view_name,
                                 const Vector<SharedPtr<ColumnDef>> &columns,
                                 TransactionID txn_id,
                                 TxnTimeStamp begin_ts,
                                 TxnManager *txn_mgr);

    static SharedPtr<String> ToString(ViewMeta *table_meta);

private:
    Status DropNewEntry(TransactionID txn_id, TxnTimeStamp begin_ts, TxnManager *txn_mgr, const String &table_name);

    void DeleteNewEntry(TransactionID txn_id, TxnManager *txn_mgr);

    Status GetEntry(TransactionID txn_id, TxnTimeStamp begin_ts);

    inline void *GetDBEntry() { return this->db_entry_; }

private:
    std::shared_mutex rw_locker_{};
    SharedPtr<String> view_name_{};

    DBEntry *db_entry_{};

    // Ordered by commit_ts from latest to oldest.
    List<UniquePtr<BaseEntry>> entry_list_{};
};

} // namespace hybridsearch
