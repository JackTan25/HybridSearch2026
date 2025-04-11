

module;

import stl;
import logical_node_type;
import column_binding;
import logical_node;
import data_type;
import internal_types;

export module logical_dummy_scan;

namespace hybridsearch {

export class LogicalDummyScan : public LogicalNode {
public:
    explicit LogicalDummyScan(u64 node_id, String table_alias, u64 table_index)
        : LogicalNode(node_id, LogicalNodeType::kDummyScan), table_alias_(std::move(table_alias)), table_index_(table_index) {}

    [[nodiscard]] Vector<ColumnBinding> GetColumnBindings() const final;

    [[nodiscard]] SharedPtr<Vector<String>> GetOutputNames() const final;

    [[nodiscard]] SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final;

    String ToString(i64 &space) const final;

    inline String name() final { return "LogicalDummyScan"; }

    String table_alias_;
    u64 table_index_;
};

} // namespace hybridsearch
