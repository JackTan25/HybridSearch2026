

module;

export module logical_filter;

import stl;
import logical_node_type;
import column_binding;
import logical_node;
import base_expression;
import data_type;
import internal_types;

namespace hybridsearch {

export class LogicalFilter : public LogicalNode {
public:
    explicit LogicalFilter(u64 node_id, SharedPtr<BaseExpression> expression)
        : LogicalNode(node_id, LogicalNodeType::kFilter), expression_(std::move(expression)) {}

    [[nodiscard]] Vector<ColumnBinding> GetColumnBindings() const;

    [[nodiscard]] SharedPtr<Vector<String>> GetOutputNames() const;

    [[nodiscard]] SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const;

    String ToString(i64 &space) const final;

    inline String name() final { return "LogicalFilter"; }

    [[nodiscard]] inline SharedPtr<BaseExpression> &expression() { return expression_; }

    [[nodiscard]] inline const SharedPtr<BaseExpression> &expression() const { return expression_; }

private:
    SharedPtr<BaseExpression> expression_{};
};

} // namespace hybridsearch
