
module;

export module table_def;

import stl;

import index_base;
import column_def;
import global_resource_usage;

namespace hybridsearch {

export class TableDef {

public:
    static inline SharedPtr<TableDef>
    Make(SharedPtr<String> schema, SharedPtr<String> table_name, SharedPtr<String> table_comment, Vector<SharedPtr<ColumnDef>> columns) {
        return MakeShared<TableDef>(std::move(schema), std::move(table_name), std::move(table_comment), std::move(columns));
    }

public:
    explicit TableDef(SharedPtr<String> schema, SharedPtr<String> table_name, SharedPtr<String> table_comment, Vector<SharedPtr<ColumnDef>> columns)
        : schema_name_(std::move(schema)), table_name_(std::move(table_name)), table_comment_(std::move(table_comment)),
          columns_(std::move(columns)) {
        SizeT column_count = columns_.size();
        for (SizeT idx = 0; idx < column_count; ++idx) {
            column_name2id_[columns_[idx]->name()] = idx;
        }

#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::IncrObjectCount("BaseResult");
#endif
    }

    virtual ~TableDef() {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::DecrObjectCount("BaseResult");
#endif
    }

    bool operator==(const TableDef &other) const;

    bool operator!=(const TableDef &other) const;

    // Estimated serialized size in bytes, ensured be no less than Write requires, allowed be larger.
    i32 GetSizeInBytes() const;

    // Write to a char buffer
    void WriteAdv(char *&ptr) const;
    // Read from a serialized version
    static SharedPtr<TableDef> ReadAdv(const char *&ptr, i32 maxbytes);

    [[nodiscard]] inline const Vector<SharedPtr<ColumnDef>> &columns() const { return columns_; }

    [[nodiscard]] inline Vector<SharedPtr<ColumnDef>> &columns() { return columns_; }

    [[nodiscard]] inline SizeT column_count() const { return columns_.size(); }

    [[nodiscard]] inline const SharedPtr<String> &table_name() const { return table_name_; }

    [[nodiscard]] inline const SharedPtr<String> &schema_name() const { return schema_name_; }

    [[nodiscard]] inline const SharedPtr<String> &table_comment() const { return table_comment_; }

    [[nodiscard]] inline SizeT GetColIdByName(const String &name) const {
        if (column_name2id_.contains(name)) {
            return column_name2id_.at(name);
        } else {
            return -1;
        }
    }

    String ToString() const;

    void UnionWith(const SharedPtr<TableDef> &other);

private:
    SharedPtr<String> schema_name_{};
    SharedPtr<String> table_name_{};
    SharedPtr<String> table_comment_{};
    Vector<SharedPtr<ColumnDef>> columns_{};
    HashMap<String, SizeT> column_name2id_{};
    Vector<IndexBase> indexes_{};
};

} // namespace hybridsearch
