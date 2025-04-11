

module;

export module logical_sort;

import stl;
import logical_node_type;
import column_binding;
import logical_node;
import data_type;
import base_expression;
import internal_types;
import select_statement;

namespace hybridsearch {

export class LogicalSort : public LogicalNode {
public:
    inline LogicalSort(u64 node_id, Vector<SharedPtr<BaseExpression>> expressions, Vector<OrderType> order_by_types)
        : LogicalNode(node_id, LogicalNodeType::kSort), expressions_(std::move(expressions)), order_by_types_(std::move(order_by_types)) {}

    [[nodiscard]] Vector<ColumnBinding> GetColumnBindings() const final;

    [[nodiscard]] SharedPtr<Vector<String>> GetOutputNames() const final;

    [[nodiscard]] SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final;

    String ToString(i64 &space) const final;

    inline String name() final { return "LogicalSort"; }

    Vector<SharedPtr<BaseExpression>> expressions_{};
    Vector<OrderType> order_by_types_{};
};

} // namespace hybridsearch