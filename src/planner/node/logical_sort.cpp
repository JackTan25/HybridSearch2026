

module;

#include <sstream>

module logical_sort;

import stl;
import logical_node_type;
import column_binding;
import logical_node;

import base_expression;
import internal_types;
import select_statement;

namespace hybridsearch {

// consider load_meta
Vector<ColumnBinding> LogicalSort::GetColumnBindings() const { return LogicalCommonFunctionUsingLoadMeta::GetColumnBindings(*this); }

SharedPtr<Vector<String>> LogicalSort::GetOutputNames() const { return LogicalCommonFunctionUsingLoadMeta::GetOutputNames(*this); }

SharedPtr<Vector<SharedPtr<DataType>>> LogicalSort::GetOutputTypes() const { return LogicalCommonFunctionUsingLoadMeta::GetOutputTypes(*this); }

String LogicalSort::ToString(i64 &space) const {

    std::stringstream ss;
    String arrow_str;
    if (space > 3) {
        space -= 4;
        arrow_str = "->  ";
    }
    ss << String(space, ' ') << arrow_str << "Order by: ";
    SizeT expression_count = expressions_.size();
    for (SizeT i = 0; i < expression_count - 1; ++i) {
        ss << expressions_[i]->Name() << " " << SelectStatement::ToString(order_by_types_[i]) << ", ";
    }
    ss << expressions_.back()->Name() << " " << SelectStatement::ToString(order_by_types_.back());
    space += arrow_str.size();

    return ss.str();
}

} // namespace hybridsearch
