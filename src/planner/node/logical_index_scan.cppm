

module;

export module logical_index_scan;

import stl;
import logical_node_type;
import logical_node;
import column_binding;
import base_table_ref;
import segment_index_entry;
import base_expression;
import default_values;
import filter_expression_push_down;
import data_type;
import meta_info;
import table_index_entry;
import fast_rough_filter;

namespace hybridsearch {

export class LogicalIndexScan : public LogicalNode {
public:
    explicit LogicalIndexScan(u64 node_id,
                              SharedPtr<BaseTableRef> &&base_table_ref,
                              SharedPtr<BaseExpression> &&index_filter,
                              UniquePtr<IndexFilterEvaluator> &&index_filter_evaluator,
                              UniquePtr<FastRoughFilterEvaluator> &&fast_rough_filter_evaluator,
                              bool add_row_id = true);

    [[nodiscard]] Vector<ColumnBinding> GetColumnBindings() const final;

    [[nodiscard]] SharedPtr<Vector<String>> GetOutputNames() const final;

    [[nodiscard]] SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final;

    [[nodiscard]] TableInfo *table_info() const;

    [[nodiscard]] String TableAlias() const;

    [[nodiscard]] u64 TableIndex() const;

    String ToString(i64 &space) const final;

    inline String name() final { return "LogicalIndexScan"; }

    SharedPtr<BaseTableRef> base_table_ref_{};

    SharedPtr<BaseExpression> index_filter_;
    UniquePtr<IndexFilterEvaluator> index_filter_evaluator_;

    UniquePtr<FastRoughFilterEvaluator> fast_rough_filter_evaluator_;

    bool add_row_id_;
};

} // namespace hybridsearch
