

module;

export module logical_join;

import stl;
import logical_node_type;
import column_binding;
import logical_node;
import data_type;
import base_expression;
import internal_types;
import join_reference;

namespace hybridsearch {

export class LogicalJoin : public LogicalNode {
public:
    explicit LogicalJoin(u64 node_id,
                         JoinType join_type,
                         String alias,
                         Vector<SharedPtr<BaseExpression>> conditions,
                         const SharedPtr<LogicalNode> &left,
                         const SharedPtr<LogicalNode> &right);

    [[nodiscard]] Vector<ColumnBinding> GetColumnBindings() const final;

    [[nodiscard]] SharedPtr<Vector<String>> GetOutputNames() const final;

    [[nodiscard]] SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final;

    String ToString(i64 &space) const final;

    inline String name() final { return "LogicalJoin"; }

    String alias_{};

    u64 mark_index_{}; // Only for mark join

public:
    JoinType join_type_{JoinType::kInner};
    Vector<SharedPtr<BaseExpression>> conditions_{};
};

} // namespace hybridsearch
