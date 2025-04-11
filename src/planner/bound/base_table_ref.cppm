

module;

export module base_table_ref;

import stl;
import table_ref;
import txn;
import table_function;
import block_index;
import internal_types;
import hybridsearch_exception;
import table_reference;
import data_type;
import meta_info;
import status;

namespace hybridsearch {

export class BaseTableRef : public TableRef {
public:
    explicit BaseTableRef(SharedPtr<TableInfo> table_info,
                          Vector<SizeT> column_ids,
                          SharedPtr<BlockIndex> block_index,
                          const String &alias,
                          u64 table_index,
                          SharedPtr<Vector<String>> column_names,
                          SharedPtr<Vector<SharedPtr<DataType>>> column_types)
        : TableRef(TableRefType::kTable, alias), table_info_(std::move(table_info)), column_ids_(std::move(column_ids)),
          block_index_(std::move(block_index)), column_names_(std::move(column_names)), column_types_(std::move(column_types)),
          table_index_(table_index) {}

    // only use some fields
    explicit BaseTableRef(SharedPtr<TableInfo> table_info, SharedPtr<BlockIndex> block_index, SharedPtr<IndexIndex> index_index = nullptr)
        : TableRef(TableRefType::kTable, ""), table_info_(std::move(table_info)), block_index_(block_index), index_index_(index_index) {}

    static SharedPtr<BaseTableRef> FakeTableRef(Txn *txn, const String &db_name, const String &table_name) {

        SharedPtr<TableInfo> table_info;
        Status status;
        std::tie(table_info, status) = txn->GetTableInfo(db_name, table_name);
        if (!status.ok()) {
            RecoverableError(status);
            return nullptr;
        }
        SharedPtr<BlockIndex> block_index = txn->GetBlockIndexFromTable(db_name, table_name);
        return MakeShared<BaseTableRef>(table_info, std::move(block_index));
    }

    void RetainColumnByIndices(const Vector<SizeT> &indices) {
        replace_field(column_ids_, indices);
        replace_field(*column_names_, indices);
        replace_field(*column_types_, indices);
    }

    SharedPtr<String> db_name() const { return table_info_->db_name_; }

    SharedPtr<String> table_name() const { return table_info_->table_name_; }

    TxnTimeStamp max_commit_ts() const { return table_info_->max_commit_ts_; }

    SharedPtr<TableInfo> table_info_{};
    Vector<SizeT> column_ids_{};
    SharedPtr<BlockIndex> block_index_{};
    SharedPtr<IndexIndex> index_index_{};

    SharedPtr<Vector<String>> column_names_{};
    SharedPtr<Vector<SharedPtr<DataType>>> column_types_{};
    u64 table_index_;

private:
    template <typename T>
    inline static void replace_field(Vector<T> &field, const Vector<SizeT> &indices) {
        Vector<T> items;
        items.reserve(indices.size());
        for (SizeT i = 0; i < indices.size(); ++i) {
            items.emplace_back(std::move(field[indices[i]]));
        }
        field = std::move(items);
    }
};

} // namespace hybridsearch