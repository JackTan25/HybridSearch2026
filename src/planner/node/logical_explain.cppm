

module;

export module logical_explain;

import stl;
import logical_node_type;
import column_binding;
import logical_node;
import data_type;
import internal_types;
import explain_statement;

namespace hybridsearch {

export class LogicalExplain : public LogicalNode {
public:
    explicit LogicalExplain(u64 node_id, ExplainType type) : LogicalNode(node_id, LogicalNodeType::kExplain), explain_type_(type) {}

    [[nodiscard]] Vector<ColumnBinding> GetColumnBindings() const final;

    [[nodiscard]] SharedPtr<Vector<String>> GetOutputNames() const final;

    [[nodiscard]] SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final;

    String ToString(i64 &space) const final;

    inline String name() final { return "LogicalExplain"; }

    [[nodiscard]] inline ExplainType explain_type() const { return explain_type_; }

    inline void SetText(const SharedPtr<Vector<SharedPtr<String>>> &texts) { texts_ = texts; }

    [[nodiscard]] inline const SharedPtr<Vector<SharedPtr<String>>> &TextArray() const { return texts_; }

private:
    ExplainType explain_type_{ExplainType::kPhysical};
    SharedPtr<Vector<SharedPtr<String>>> texts_{nullptr};
};

} // namespace hybridsearch
