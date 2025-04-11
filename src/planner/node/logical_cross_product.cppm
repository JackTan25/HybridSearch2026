

module;

export module logical_cross_product;

import stl;
import logical_node_type;
import column_binding;
import logical_node;
import data_type;
import internal_types;

namespace hybridsearch {

export class LogicalCrossProduct : public LogicalNode {
public:
    explicit LogicalCrossProduct(u64 node_id, String alias, const SharedPtr<LogicalNode> &left, const SharedPtr<LogicalNode> &right);

    [[nodiscard]] Vector<ColumnBinding> GetColumnBindings() const final;

    [[nodiscard]] SharedPtr<Vector<String>> GetOutputNames() const final;

    [[nodiscard]] SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final;

    String ToString(i64 &space) const final;

    inline String name() final { return "LogicalCrossProduct"; }

    String alias_{};
};

} // namespace hybridsearch
