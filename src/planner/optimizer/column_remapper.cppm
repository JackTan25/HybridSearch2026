

module;

export module column_remapper;

import internal_types;
import stl;
import logical_node_visitor;
import logical_node;
import base_expression;
import column_binding;
import query_context;
import column_expression;
import data_type;

import optimizer_rule;
import logical_node_type;

namespace hybridsearch {

class BindingRemapper : public LogicalNodeVisitor {
public:
    void VisitNode(LogicalNode &op) final;

private:
    virtual SharedPtr<BaseExpression> VisitReplace(const SharedPtr<ColumnExpression> &expression) override;

    Vector<ColumnBinding> bindings_;
    SharedPtr<Vector<SharedPtr<DataType>>> output_types_;
    u32 column_cnt_{};
};

export class ColumnRemapper : public OptimizerRule {
public:
    inline void ApplyToPlan(QueryContext *, SharedPtr<LogicalNode> &logical_plan) final { return remapper_.VisitNode(*logical_plan); }

    [[nodiscard]] inline String name() const final { return "Column Remapper"; }

private:
    BindingRemapper remapper_{};
};

} // namespace hybridsearch
