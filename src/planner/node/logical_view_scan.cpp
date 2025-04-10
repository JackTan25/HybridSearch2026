

module;

#include <sstream>

module logical_view_scan;

import stl;
import column_binding;

import base_expression;
import view;
import internal_types;

namespace hybridsearch {

Vector<ColumnBinding> LogicalViewScan::GetColumnBindings() const { return left_node_->GetColumnBindings(); }

SharedPtr<Vector<String>> LogicalViewScan::GetOutputNames() const { return left_node_->GetOutputNames(); }

SharedPtr<Vector<SharedPtr<DataType>>> LogicalViewScan::GetOutputTypes() const { return left_node_->GetOutputTypes(); }

String LogicalViewScan::ToString(i64 &space) const {
    std::stringstream ss;
    String arrow_str;
    if (space > 3) {
        space -= 4;
        arrow_str = "->  ";
    }
    ss << String(space, ' ') << arrow_str << "ViewScan: " << view_ptr_->view_name();
    space += arrow_str.size();

    return ss.str();
}

} // namespace hybridsearch
