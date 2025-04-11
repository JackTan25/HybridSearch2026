

module;

export module logical_unnest;

import stl;
import logical_node_type;
import column_binding;
import logical_node;
import base_expression;
import data_type;
import internal_types;

namespace hybridsearch {

export class LogicalUnnest : public LogicalNode {
public:
    explicit LogicalUnnest(u64 node_id, Vector<SharedPtr<BaseExpression>> expressions, u64 unnest_idx)
        : LogicalNode(node_id, LogicalNodeType::kUnnest), expression_list_(std::move(expressions)), unnest_idx_(unnest_idx) {}

    [[nodiscard]] Vector<ColumnBinding> GetColumnBindings() const;

    [[nodiscard]] Vector<ColumnBinding> RemoveColumnBindings() const;

    [[nodiscard]] SharedPtr<Vector<String>> GetOutputNames() const;

    [[nodiscard]] SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const;

    String ToString(i64 &space) const final;

    inline String name() final { return "LogicalUnnest"; }

public:
    // getter
    Vector<SharedPtr<BaseExpression>> &expression_list() { return expression_list_; }
    const Vector<SharedPtr<BaseExpression>> &expression_list() const { return expression_list_; }
    u64 unnest_idx() const { return unnest_idx_; }

private:
    Vector<SharedPtr<BaseExpression>> expression_list_{};
    u64 unnest_idx_{};
};

} // namespace hybridsearch
