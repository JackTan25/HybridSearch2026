

module;

#include <sstream>

module logical_fusion;

import stl;
import base_table_ref;
import column_binding;
import logical_node_type;
import fusion_expression;

import default_values;
import internal_types;

namespace hybridsearch {

LogicalFusion::LogicalFusion(const u64 node_id, SharedPtr<BaseTableRef> base_table_ref, SharedPtr<FusionExpression> fusion_expr)
    : LogicalNode(node_id, LogicalNodeType::kFusion), base_table_ref_(std::move(base_table_ref)), fusion_expr_(std::move(fusion_expr)) {}

String LogicalFusion::ToString(i64 &space) const {
    std::stringstream ss;
    String arrow_str;
    if (space != 0) {
        arrow_str = String(space - 2, ' ');
        arrow_str += "-> LogicalFusion ";
    } else {
        arrow_str = "LogicalFusion ";
    }
    ss << arrow_str << fusion_expr_->ToString();
    return ss.str();
}

} // namespace hybridsearch
