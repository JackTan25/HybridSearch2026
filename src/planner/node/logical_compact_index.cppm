

module;

export module logical_compact_index;

import stl;
import logical_node;
import logical_node_type;
import column_binding;
import data_type;
import base_table_ref;

namespace hybridsearch {

export class LogicalCompactIndex : public LogicalNode {
public:
    LogicalCompactIndex(u64 node_id, SharedPtr<BaseTableRef> base_table_ref)
        : LogicalNode(node_id, LogicalNodeType::kCompactIndex), base_table_ref_(base_table_ref) {}

    [[nodiscard]] Vector<ColumnBinding> GetColumnBindings() const final;

    [[nodiscard]] SharedPtr<Vector<String>> GetOutputNames() const final;

    [[nodiscard]] SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final;

    String ToString(i64 &space) const final;

    inline String name() final { return "LogicalCompactIndex"; }

public:
    SharedPtr<BaseTableRef> base_table_ref_{};
};

} // namespace hybridsearch