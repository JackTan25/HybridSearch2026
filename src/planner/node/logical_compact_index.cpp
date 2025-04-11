

module;

#include <sstream>

module logical_compact_index;

import stl;
import column_binding;
import data_type;
import table_entry;
import logical_type;

namespace hybridsearch {

Vector<ColumnBinding> LogicalCompactIndex::GetColumnBindings() const { return {}; }

SharedPtr<Vector<String>> LogicalCompactIndex::GetOutputNames() const {
    auto result = MakeShared<Vector<String>>();
    result->push_back("OK");
    return result;
}

SharedPtr<Vector<SharedPtr<DataType>>> LogicalCompactIndex::GetOutputTypes() const {
    auto result = MakeShared<Vector<SharedPtr<DataType>>>();
    result->push_back(MakeShared<DataType>(LogicalType::kInteger));
    return result;
}

String LogicalCompactIndex::ToString(i64 &space) const {
    std::stringstream ss;
    String arrow_str;
    if (space > 3) {
        space -= 4;
        arrow_str = "->  ";
    }
    ss << String(space, ' ') << arrow_str << "COMPACT INDEX";
    auto *table_info = base_table_ref_->table_info_.get();
    ss << *table_info->db_name_ << "." << *table_info->table_name_;
    space += arrow_str.size();
    return ss.str();
}

} // namespace hybridsearch
