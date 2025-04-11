

module;

export module logical_top;

import stl;
import logical_node_type;
import column_binding;
import logical_node;

import base_expression;
import base_table_ref;
import internal_types;
import select_statement;
import data_type;

namespace hybridsearch {

export class LogicalTop : public LogicalNode {
public:
    inline explicit LogicalTop(u64 node_id,
                               SharedPtr<BaseTableRef> base_table_ref,
                               SharedPtr<BaseExpression> limit_expression,
                               SharedPtr<BaseExpression> offset_expression,
                               Vector<SharedPtr<BaseExpression>> sort_expressions,
                               Vector<OrderType> order_by_types,
                               bool total_hits_count_flag)
        : LogicalNode(node_id, LogicalNodeType::kTop), base_table_ref_(std::move(base_table_ref)), limit_expression_(std::move(limit_expression)),
          offset_expression_(std::move(offset_expression)), sort_expressions_(std::move(sort_expressions)),
          order_by_types_(std::move(order_by_types)), total_hits_count_flag_(total_hits_count_flag) {}

    [[nodiscard]] Vector<ColumnBinding> GetColumnBindings() const final;

    [[nodiscard]] SharedPtr<Vector<String>> GetOutputNames() const final;

    [[nodiscard]] SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final;

    String ToString(i64 &space) const final;

    inline String name() final { return "LogicalTop"; }

    SharedPtr<BaseTableRef> base_table_ref_{};
    SharedPtr<BaseExpression> limit_expression_{};
    SharedPtr<BaseExpression> offset_expression_{};
    Vector<SharedPtr<BaseExpression>> sort_expressions_{};
    Vector<OrderType> order_by_types_{};
    bool total_hits_count_flag_{};
};

} // namespace hybridsearch
