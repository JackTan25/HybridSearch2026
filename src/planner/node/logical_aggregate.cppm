

module;

export module logical_aggregate;

import stl;
import logical_node_type;
import column_binding;
import logical_node;
import base_expression;

import base_table_ref;
import internal_types;
import data_type;

namespace hybridsearch {

export class LogicalAggregate : public LogicalNode {
public:
    explicit LogicalAggregate(u64 node_id,
                              SharedPtr<BaseTableRef> base_table_ref,
                              Vector<SharedPtr<BaseExpression>> groups,
                              u64 groupby_index,
                              Vector<SharedPtr<BaseExpression>> aggregates,
                              u64 aggregate_index)
        : LogicalNode(node_id, LogicalNodeType::kAggregate), groups_(std::move(groups)), groupby_index_(groupby_index),
          aggregates_(std::move(aggregates)), aggregate_index_(aggregate_index), base_table_ref_(std::move(base_table_ref)) {}

    [[nodiscard]] Vector<ColumnBinding> GetColumnBindings() const final;

    [[nodiscard]] SharedPtr<Vector<String>> GetOutputNames() const final;

    [[nodiscard]] SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final;

    String ToString(i64 &space) const final;

    inline String name() final { return "LogicalAggregate"; }

    Vector<SharedPtr<BaseExpression>> groups_{};
    u64 groupby_index_{};

    Vector<SharedPtr<BaseExpression>> aggregates_{};
    u64 aggregate_index_{};

    SharedPtr<BaseTableRef> base_table_ref_;
};

} // namespace hybridsearch
