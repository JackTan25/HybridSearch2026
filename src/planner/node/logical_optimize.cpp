

module;

#include <sstream>

module logical_optimize;

import stl;

import column_binding;
import internal_types;

namespace hybridsearch {

Vector<ColumnBinding> LogicalOptimize::GetColumnBindings() const { return {}; }

SharedPtr<Vector<String>> LogicalOptimize::GetOutputNames() const { return MakeShared<Vector<String>>(); }

SharedPtr<Vector<SharedPtr<DataType>>> LogicalOptimize::GetOutputTypes() const { return MakeShared<Vector<SharedPtr<DataType>>>(); }

String LogicalOptimize::ToString(i64 &space) const {
    std::stringstream ss;
    String arrow_str;
    if (space > 3) {
        space -= 4;
        arrow_str = "->  ";
    }

    ss << String(space, ' ') << "-> " << "Optimize Table: ";
    space += arrow_str.size();

    return ss.str();
}

} // namespace hybridsearch
