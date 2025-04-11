

module;

#include <sstream>

module logical_limit;

import internal_types;
import stl;
import logical_node_type;
import column_binding;
import logical_node;

import base_expression;

namespace hybridsearch {

Vector<ColumnBinding> LogicalLimit::GetColumnBindings() const { return left_node_->GetColumnBindings(); }

SharedPtr<Vector<String>> LogicalLimit::GetOutputNames() const { return left_node_->GetOutputNames(); }

SharedPtr<Vector<SharedPtr<DataType>>> LogicalLimit::GetOutputTypes() const { return left_node_->GetOutputTypes(); }

String LogicalLimit::ToString(i64 &space) const {
    std::stringstream ss;
    String arrow_str;
    if (space > 3) {
        space -= 4;
        arrow_str = "->  ";
    }

    ss << String(space, ' ') << arrow_str << "Limit (limit: " << limit_expression_->Name();
    if (offset_expression_.get() != nullptr) {
        ss << ", offset: " << offset_expression_->Name();
    }
    ss << ")";
    space += arrow_str.size();

    return ss.str();
}

} // namespace hybridsearch
