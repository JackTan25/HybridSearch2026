

module;

#include <sstream>

module logical_dummy_scan;

import stl;
import column_binding;
import data_type;
import internal_types;

namespace hybridsearch {

Vector<ColumnBinding> LogicalDummyScan::GetColumnBindings() const { return {}; }

SharedPtr<Vector<String>> LogicalDummyScan::GetOutputNames() const { return MakeShared<Vector<String>>(); }

SharedPtr<Vector<SharedPtr<DataType>>> LogicalDummyScan::GetOutputTypes() const { return MakeShared<Vector<SharedPtr<DataType>>>(); }

String LogicalDummyScan::ToString(i64 &space) const {
    std::stringstream ss;
    String arrow_str;
    if (space > 3) {
        space -= 4;
        arrow_str = "->  ";
    }
    ss << String(space, ' ') << "-> "
       << "DummyScan: " << this->table_alias_ << "(" << this->table_index_ << ")";
    space += arrow_str.size();

    return ss.str();
}

} // namespace hybridsearch
