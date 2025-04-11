

module;

#include <sstream>

module logical_top;

import stl;
import logical_node_type;
import column_binding;
import logical_node;

import base_expression;
import internal_types;
import select_statement;

namespace hybridsearch {

Vector<ColumnBinding> LogicalTop::GetColumnBindings() const { return LogicalCommonFunctionUsingLoadMeta::GetColumnBindings(*this); }

SharedPtr<Vector<String>> LogicalTop::GetOutputNames() const { return LogicalCommonFunctionUsingLoadMeta::GetOutputNames(*this); }

SharedPtr<Vector<SharedPtr<DataType>>> LogicalTop::GetOutputTypes() const { return LogicalCommonFunctionUsingLoadMeta::GetOutputTypes(*this); }

String LogicalTop::ToString(i64 &space) const {
    std::stringstream ss;
    String arrow_str;
    if (space > 3) {
        space -= 4;
        arrow_str = "->  ";
    }

    ss << String(space, ' ') << arrow_str << "Top: Limit (limit: " << limit_expression_->Name();
    if (offset_expression_) {
        ss << ", offset: " << offset_expression_->Name();
    }
    ss << ") Order by: ";
    SizeT expression_count = sort_expressions_.size();
    for (SizeT i = 0; i < expression_count - 1; ++i) {
        ss << sort_expressions_[i]->Name() << " " << SelectStatement::ToString(order_by_types_[i]) << ", ";
    }
    ss << sort_expressions_.back()->Name() << " " << SelectStatement::ToString(order_by_types_.back());
    space += arrow_str.size();

    return std::move(ss).str();
}

} // namespace hybridsearch
