

module;

export module logical_create_schema;

import stl;
import logical_node_type;
import column_binding;
import logical_node;
import data_type;
import internal_types;
import extra_ddl_info;

namespace hybridsearch {

export class LogicalCreateSchema : public LogicalNode {
public:
    static inline SharedPtr<LogicalCreateSchema>
    Make(u64 node_id, SharedPtr<String> schema_name, ConflictType conflict_type, SharedPtr<String> comment) {
        return MakeShared<LogicalCreateSchema>(node_id, std::move(schema_name), conflict_type, std::move(comment));
    }

public:
    LogicalCreateSchema(u64 node_id, const SharedPtr<String> schema_name, ConflictType conflict_type, SharedPtr<String> comment)
        : LogicalNode(node_id, LogicalNodeType::kCreateSchema), schema_name_(std::move(schema_name)), conflict_type_(conflict_type),
          comment_(std::move(comment)) {}

    [[nodiscard]] Vector<ColumnBinding> GetColumnBindings() const final;

    [[nodiscard]] SharedPtr<Vector<String>> GetOutputNames() const final;

    [[nodiscard]] SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final;

    String ToString(i64 &space) const final;

    inline String name() final { return "LogicalCreateSchema"; }

    [[nodiscard]] inline SharedPtr<String> schema_name() const { return schema_name_; }

    [[nodiscard]] inline ConflictType conflict_type() const { return conflict_type_; }

    [[nodiscard]] inline SharedPtr<String> comment() const { return comment_; }

private:
    SharedPtr<String> schema_name_{};
    ConflictType conflict_type_{ConflictType::kInvalid};
    SharedPtr<String> comment_{};
};

} // namespace hybridsearch
