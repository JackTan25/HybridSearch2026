

module;

export module logical_drop_collection;

import stl;

import logical_node;
import logical_node_type;
import column_binding;
import internal_types;
import extra_ddl_info;
import data_type;

namespace hybridsearch {

export class LogicalDropCollection final : public LogicalNode {
public:
    LogicalDropCollection(u64 node_id, SharedPtr<String> schema_name, SharedPtr<String> collection_name, ConflictType conflict_type)
        : LogicalNode(node_id, LogicalNodeType::kDropCollection), schema_name_(std::move(schema_name)), collection_name_(std::move(collection_name)),
          conflict_type_(conflict_type) {}

    [[nodiscard]] Vector<ColumnBinding> GetColumnBindings() const final;

    [[nodiscard]] SharedPtr<Vector<String>> GetOutputNames() const final;

    [[nodiscard]] SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final;

    String ToString(i64 &space) const final;

    inline String name() final { return "LogicalDropCollection"; }

    [[nodiscard]] SharedPtr<String> collection_name() const { return collection_name_; }

    [[nodiscard]] SharedPtr<String> schema_name() const { return schema_name_; }

    [[nodiscard]] inline ConflictType conflict_type() const { return conflict_type_; }

private:
    SharedPtr<String> schema_name_{};
    SharedPtr<String> collection_name_{};
    ConflictType conflict_type_{ConflictType::kInvalid};
};

} // namespace hybridsearch
