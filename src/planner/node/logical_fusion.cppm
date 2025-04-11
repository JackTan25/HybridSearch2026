

module;

export module logical_fusion;

import stl;
import logical_node_type;
import column_binding;
import logical_node;

import fusion_expression;
import match_expression;
import base_table_ref;

import internal_types;
import data_type;

namespace hybridsearch {

export class LogicalFusion : public LogicalNode {
public:
    explicit LogicalFusion(u64 node_id, SharedPtr<BaseTableRef> base_table_ref, SharedPtr<FusionExpression> fusion_expr);

    Vector<ColumnBinding> GetColumnBindings() const final { return left_node_->GetColumnBindings(); };

    SharedPtr<Vector<String>> GetOutputNames() const final { return left_node_->GetOutputNames(); };

    SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final { return left_node_->GetOutputTypes(); };

    String ToString(i64 &space) const final;

    inline String name() final { return "LogicalFusion"; }

    Vector<SharedPtr<LogicalNode>> other_children_{};
    SharedPtr<BaseTableRef> base_table_ref_{};
    SharedPtr<FusionExpression> fusion_expr_{};
};

} // namespace hybridsearch
