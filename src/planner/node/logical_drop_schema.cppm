

module;

export module logical_drop_schema;

import stl;

import logical_node;
import logical_node_type;
import column_binding;
import internal_types;
import extra_ddl_info;
import data_type;

namespace hybridsearch {

export class LogicalDropSchema final : public LogicalNode {
public:
    LogicalDropSchema(u64 node_id, SharedPtr<String> schema_name, ConflictType conflict_type)
        : LogicalNode(node_id, LogicalNodeType::kDropSchema), schema_name_(std::move(schema_name)), conflict_type_(conflict_type) {}

    [[nodiscard]] Vector<ColumnBinding> GetColumnBindings() const final;

    [[nodiscard]] SharedPtr<Vector<String>> GetOutputNames() const final;

    [[nodiscard]] SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final;

    String ToString(i64 &space) const final;

    inline String name() final { return "LogicalDropSchema"; }

    [[nodiscard]] SharedPtr<String> schema_name() const { return schema_name_; }

    [[nodiscard]] ConflictType conflict_type() const { return conflict_type_; }

private:
    SharedPtr<String> schema_name_{};
    ConflictType conflict_type_{ConflictType::kInvalid};
};

} // namespace hybridsearch
