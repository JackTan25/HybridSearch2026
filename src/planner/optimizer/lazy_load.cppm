

module;

export module lazy_load;

import stl;
import logical_node_visitor;
import logical_node;
import logical_node_type;
import base_expression;
import column_binding;
import query_context;
import column_expression;
import optimizer_rule;
import load_meta;
import internal_types;
import data_type;
import base_table_ref;

namespace hybridsearch {

class RefencecColumnCollection : public LogicalNodeVisitor {
public:
    void VisitNode(LogicalNode &op) final;

private:
    SharedPtr<BaseExpression> VisitReplace(const SharedPtr<ColumnExpression> &expression) final;

    HashMap<SizeT, Vector<ColumnBinding>> scan_bindings_{};
    HashMap<SizeT, SharedPtr<Vector<SharedPtr<DataType>>>> column_types_{};
    HashMap<SizeT, SharedPtr<Vector<String>>> column_names_{};

    HashSet<ColumnBinding> unloaded_bindings_;
    Vector<LoadMeta> load_metas_;
};

class CleanScan : public LogicalNodeVisitor {
public:
    void VisitNode(LogicalNode &op) final;

private:
    SharedPtr<BaseExpression> VisitReplace(const SharedPtr<ColumnExpression> &expression) final;

    SharedPtr<Vector<LoadMeta>> last_op_load_metas_{};
    u64 last_op_node_id_{};
    Vector<SizeT> scan_table_indexes_{};
};

export class LazyLoad : public OptimizerRule {
public:
    inline void ApplyToPlan(QueryContext *query_context_ptr, SharedPtr<LogicalNode> &logical_plan) final {
        auto logic_op_type = logical_plan->operator_type();
        switch (logic_op_type) {
            case LogicalNodeType::kInsert:
            case LogicalNodeType::kImport:
            case LogicalNodeType::kExport:
            case LogicalNodeType::kCreateTable:
            case LogicalNodeType::kCreateIndex:
            case LogicalNodeType::kDropTable:
            case LogicalNodeType::kDropIndex:
            case LogicalNodeType::kCreateSchema:
            case LogicalNodeType::kDropSchema:
            case LogicalNodeType::kShow:
            case LogicalNodeType::kCommand:
            case LogicalNodeType::kPrepare:
                return;
            default:
                collector.VisitNode(*logical_plan);
                cleaner_.VisitNode(*logical_plan);
        }
    }

    [[nodiscard]] inline String name() const final { return "Lazy Load"; }

private:
    RefencecColumnCollection collector{};
    CleanScan cleaner_{};
};

export Optional<BaseTableRef *> GetScanTableRef(LogicalNode &op);

} // namespace hybridsearch