

module;

export module logical_compact;

import stl;
import logical_node;
import logical_node_type;
import column_binding;
import data_type;
import base_table_ref;
import query_context;
import compact_statement;

namespace hybridsearch {

export class LogicalCompact : public LogicalNode {
public:
    LogicalCompact(u64 node_id, SharedPtr<BaseTableRef> base_table_ref, CompactStatementType compact_type)
        : LogicalNode(node_id, LogicalNodeType::kCompact), base_table_ref_(base_table_ref), compact_type_(compact_type) {}

    [[nodiscard]] Vector<ColumnBinding> GetColumnBindings() const final;

    [[nodiscard]] SharedPtr<Vector<String>> GetOutputNames() const final;

    [[nodiscard]] SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final;

    String ToString(i64 &space) const final;

    inline String name() final { return "LogicalCompact"; }

public:
    SharedPtr<BaseTableRef> base_table_ref_;
    CompactStatementType compact_type_;
};

} // namespace hybridsearch
