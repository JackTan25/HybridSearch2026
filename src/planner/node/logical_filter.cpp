

module;

#include <sstream>

module logical_filter;

import stl;
import column_binding;
import logical_node;
import internal_types;

namespace hybridsearch {

Vector<ColumnBinding> LogicalFilter::GetColumnBindings() const { return LogicalCommonFunctionUsingLoadMeta::GetColumnBindings(*this); }

SharedPtr<Vector<String>> LogicalFilter::GetOutputNames() const { return LogicalCommonFunctionUsingLoadMeta::GetOutputNames(*this); }

SharedPtr<Vector<SharedPtr<DataType>>> LogicalFilter::GetOutputTypes() const { return LogicalCommonFunctionUsingLoadMeta::GetOutputTypes(*this); }

String LogicalFilter::ToString(i64 &space) const {
    std::stringstream ss;
    String arrow_str;
    if (space > 3) {
        space -= 4;
        arrow_str = "->  ";
    }
    ss << String(space, ' ') << arrow_str << "Logical Filter: " << expression_->Name();
    space += arrow_str.size();
    return ss.str();
}

} // namespace hybridsearch
