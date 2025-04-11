

module;

#include <sstream>

module logical_unnest_aggregate;

import stl;
import column_binding;
import logical_node;
import internal_types;

namespace hybridsearch {

Vector<ColumnBinding> LogicalUnnestAggregate::GetColumnBindings() const { return LogicalCommonFunctionUsingLoadMeta::GetColumnBindings(*this); }

SharedPtr<Vector<String>> LogicalUnnestAggregate::GetOutputNames() const { return LogicalCommonFunctionUsingLoadMeta::GetOutputNames(*this); }

SharedPtr<Vector<SharedPtr<DataType>>> LogicalUnnestAggregate::GetOutputTypes() const {
    return LogicalCommonFunctionUsingLoadMeta::GetOutputTypes(*this);
}

String LogicalUnnestAggregate::ToString(i64 &space) const {
    std::stringstream ss;
    String arrow_str;
    if (space > 3) {
        space -= 4;
        arrow_str = "->  ";
    }
    ss << String(space, ' ') << arrow_str << "Logical Unnest Aggregate: ";

    SizeT expr_count = unnest_expression_list_.size();
    for (SizeT idx = 0; idx < expr_count; ++idx) {
        ss << unnest_expression_list_[idx]->Name();
        if (idx < expr_count - 1) {
            ss << ", ";
        }
    }

    if (!aggregates_.empty()) {
        ss << "Aggregate on: ";
        SizeT expression_count = aggregates_.size();
        for (SizeT i = 0; i < expression_count - 1; ++i) {
            ss << aggregates_[i]->Name() << ", ";
        }
        ss << aggregates_.back()->Name();
    }

    if (!groups_.empty()) {
        if (aggregates_.empty()) {
            ss << "Group by: ";
        } else {
            ss << ", Group by: ";
        }
        SizeT expression_count = groups_.size();
        for (SizeT i = 0; i < expression_count - 1; ++i) {
            ss << groups_[i]->Name() << ", ";
        }
        ss << groups_.back()->Name();
    }

    space += arrow_str.size();
    return ss.str();
}

} // namespace hybridsearch
