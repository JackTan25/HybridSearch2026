

module;

#include <sstream>

module logical_compact;

import stl;
import column_binding;
import data_type;
import base_table_ref;
import logical_type;

namespace hybridsearch {

Vector<ColumnBinding> LogicalCompact::GetColumnBindings() const { return {}; }

SharedPtr<Vector<String>> LogicalCompact::GetOutputNames() const {
    auto result = MakeShared<Vector<String>>();
    result->push_back("OK");
    return result;
}

SharedPtr<Vector<SharedPtr<DataType>>> LogicalCompact::GetOutputTypes() const {
    auto result = MakeShared<Vector<SharedPtr<DataType>>>();
    result->push_back(MakeShared<DataType>(LogicalType::kInteger));
    return result;
}

String LogicalCompact::ToString(i64 &space) const {
    std::stringstream ss;
    String arrow_str;
    if (space > 3) {
        space -= 4;
        arrow_str = "->  ";
    }
    ss << String(space, ' ') << arrow_str << "COMPACT ";
    ss << *base_table_ref_->db_name() << "." << *base_table_ref_->table_name();
    space += arrow_str.size();
    return ss.str();
}

} // namespace hybridsearch