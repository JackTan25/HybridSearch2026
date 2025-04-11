

module;

export module logical_insert;

import stl;
import logical_node_type;
import column_binding;
import logical_node;

import base_expression;
import meta_info;
import internal_types;
import data_type;

namespace hybridsearch {

export class LogicalInsert : public LogicalNode {
public:
    explicit inline LogicalInsert(u64 node_id, SharedPtr<TableInfo> table_info, u64 table_index, Vector<Vector<SharedPtr<BaseExpression>>> value_list)
        : LogicalNode(node_id, LogicalNodeType::kInsert), table_info_(std::move(table_info)), value_list_(std::move(value_list)),
          table_index_(table_index) {};

    [[nodiscard]] Vector<ColumnBinding> GetColumnBindings() const final;

    [[nodiscard]] SharedPtr<Vector<String>> GetOutputNames() const final;

    [[nodiscard]] SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final;

    String ToString(i64 &space) const final;

    inline String name() final { return "LogicalInsert"; }

    inline void set_value_list(const Vector<Vector<SharedPtr<BaseExpression>>> &value_list) { value_list_ = value_list; }

    inline SharedPtr<TableInfo>& table_info() { return table_info_; }
    inline const SharedPtr<TableInfo>& table_info() const { return table_info_; }

    inline Vector<Vector<SharedPtr<BaseExpression>>> &value_list() { return value_list_; }

    [[nodiscard]] inline const Vector<Vector<SharedPtr<BaseExpression>>> &value_list() const { return value_list_; }

    [[nodiscard]] inline u64 table_index() const { return table_index_; }

public:
    static bool NeedCastInInsert(const DataType &from, const DataType &to) {
        if (from.type() == to.type()) {
            switch (from.type()) {
                // Embedding
                // Bitmap
                // Decimal
                default:
                    break;
            }
        }
        return true;
    }

private:
    SharedPtr<TableInfo> table_info_{};
    Vector<Vector<SharedPtr<BaseExpression>>> value_list_{};
    u64 table_index_{};
};

} // namespace hybridsearch
