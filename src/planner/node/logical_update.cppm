

module;

export module logical_update;

import stl;

import logical_node;
import logical_node_type;
import column_binding;
import meta_info;
import base_expression;
import internal_types;
import data_type;

namespace hybridsearch {

export class LogicalUpdate final : public LogicalNode {

public:
    LogicalUpdate(u64 node_id,
                  SharedPtr<TableInfo> table_info,
                  const Vector<Pair<SizeT, SharedPtr<BaseExpression>>> &update_columns,
                  const Vector<SharedPtr<BaseExpression>> &all_columns_in_table,
                  const Vector<SharedPtr<BaseExpression>> &final_result_columns)
        : LogicalNode(node_id, LogicalNodeType::kUpdate), table_info_(std::move(table_info)), update_columns_(update_columns),
          all_columns_in_table_(all_columns_in_table), final_result_columns_(final_result_columns) {}

    [[nodiscard]] Vector<ColumnBinding> GetColumnBindings() const final;

    [[nodiscard]] SharedPtr<Vector<String>> GetOutputNames() const final;

    [[nodiscard]] SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final;

    String ToString(i64 &space) const final;

    inline String name() final { return "LogicalUpdate"; }

    SharedPtr<TableInfo> table_info_{};
    Vector<Pair<SizeT, SharedPtr<BaseExpression>>> update_columns_; // Column ID = Expression
    Vector<SharedPtr<BaseExpression>> all_columns_in_table_{};      // columns in the table
    Vector<SharedPtr<BaseExpression>> final_result_columns_{};      // columns for the new blocks
};

} // namespace hybridsearch
