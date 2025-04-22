

module;

export module logical_match_scan_base;

import stl;
import logical_node_type;
import column_binding;
import logical_node;
import base_expression;
import base_table_ref;
import meta_info;
import internal_types;
import data_type;
import fast_rough_filter;
import common_query_filter;

namespace hybridsearch {

export class LogicalMatchScanBase : public LogicalNode {
public:
    LogicalMatchScanBase(u64 node_id, LogicalNodeType node_type, SharedPtr<BaseTableRef> base_table_ref, SharedPtr<BaseExpression> query_expression);

    [[nodiscard]] Vector<ColumnBinding> GetColumnBindings() const override;

    [[nodiscard]] SharedPtr<Vector<String>> GetOutputNames() const override;

    [[nodiscard]] SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const override;

    [[nodiscard]] TableInfo *table_info() const;

    [[nodiscard]] String TableAlias() const;

    [[nodiscard]] u64 TableIndex() const;

    String ToString(i64 &space) const override;

public:
    SharedPtr<BaseTableRef> base_table_ref_{};

    SharedPtr<BaseExpression> query_expression_{};

    SharedPtr<BaseExpression> filter_expression_{};

    SharedPtr<CommonQueryFilter> common_query_filter_{};
};

} // namespace hybridsearch
