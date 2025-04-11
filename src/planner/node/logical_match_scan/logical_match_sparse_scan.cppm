

module;

export module logical_match_sparse_scan;

import stl;
import logical_node_type;
import column_binding;
import logical_node;
import logical_match_scan_base;
import base_expression;
import match_sparse_expression;
import base_table_ref;
import table_entry;
import internal_types;
import data_type;
import fast_rough_filter;
import common_query_filter;

namespace hybridsearch {

export class LogicalMatchSparseScan final : public LogicalMatchScanBase {
public:
    explicit LogicalMatchSparseScan(u64 node_id, SharedPtr<BaseTableRef> base_table_ref, SharedPtr<MatchSparseExpression> match_sparse_expr)
        : LogicalMatchScanBase(node_id, LogicalNodeType::kMatchSparseScan, base_table_ref, match_sparse_expr) {}

    String name() override { return "LogicalMatchSparseScan"; }

public:
};
} // namespace hybridsearch
