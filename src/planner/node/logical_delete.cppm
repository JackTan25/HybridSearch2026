

module;

export module logical_delete;

import stl;

import logical_node;
import logical_node_type;
import column_binding;

import internal_types;
import data_type;
import meta_info;

namespace hybridsearch {

export class LogicalDelete final : public LogicalNode {

public:
    LogicalDelete(u64 node_id, SharedPtr<TableInfo> table_info)
        : LogicalNode(node_id, LogicalNodeType::kDelete), table_info_(std::move(table_info)) {}

    [[nodiscard]] Vector<ColumnBinding> GetColumnBindings() const final;

    [[nodiscard]] SharedPtr<Vector<String>> GetOutputNames() const final;

    [[nodiscard]] SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final;

    String ToString(i64 &space) const final;

    inline String name() final { return "LogicalDelete"; }

    SharedPtr<TableInfo> table_info_{};
};

} // namespace hybridsearch
