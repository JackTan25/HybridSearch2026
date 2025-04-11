

module;

export module logical_knn_scan;

import stl;
import logical_node;
import logical_node_type;
import base_expression;
import knn_expression;
import base_table_ref;
import logical_match_scan_base;

namespace hybridsearch {

export class LogicalKnnScan : public LogicalMatchScanBase {
public:
    explicit LogicalKnnScan(const u64 node_id,
                            SharedPtr<BaseTableRef> base_table_ref,
                            SharedPtr<BaseExpression> knn_expression,
                            const u64 table_index)
        : LogicalMatchScanBase(node_id, LogicalNodeType::kKnnScan, std::move(base_table_ref), std::move(knn_expression)),
          knn_table_index_(table_index) {}

    SharedPtr<KnnExpression> knn_expression() const { return std::static_pointer_cast<KnnExpression>(query_expression_); }

    inline String name() final { return "LogicalKnnScan"; }

public:
    u64 knn_table_index_{};
};

} // namespace hybridsearch
