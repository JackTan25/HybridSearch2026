

module;

import stl;
import logical_node_type;
import column_binding;
import logical_node;

import base_table_ref;
import meta_info;
import internal_types;
import data_type;
import fast_rough_filter;

export module logical_table_scan;

namespace hybridsearch {

export class LogicalTableScan : public LogicalNode {
public:
    explicit LogicalTableScan(u64 node_id, SharedPtr<BaseTableRef> base_table_ref, bool add_row_id = true);

    [[nodiscard]] Vector<ColumnBinding> GetColumnBindings() const final;

    [[nodiscard]] SharedPtr<Vector<String>> GetOutputNames() const final;

    [[nodiscard]] SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final;

    [[nodiscard]] TableInfo *table_info() const;

    [[nodiscard]] String TableAlias() const;

    [[nodiscard]] u64 TableIndex() const;

    String ToString(i64 &space) const final;

    inline String name() final { return "LogicalTableScan"; }

    SharedPtr<BaseTableRef> base_table_ref_{};

    UniquePtr<FastRoughFilterEvaluator> fast_rough_filter_evaluator_;

    bool add_row_id_;
};

} // namespace hybridsearch
