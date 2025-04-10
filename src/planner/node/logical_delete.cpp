

module;

#include <sstream>

module logical_delete;

import stl;

import logical_node;
import logical_node_type;
import column_binding;
import base_table_ref;
import column_binding;
import logical_node_type;
import logical_type;
import internal_types;

namespace hybridsearch {

Vector<ColumnBinding> LogicalDelete::GetColumnBindings() const { return {}; }

SharedPtr<Vector<String>> LogicalDelete::GetOutputNames() const {
    SharedPtr<Vector<String>> result = MakeShared<Vector<String>>();
    result->emplace_back("OK");
    return result;
}

SharedPtr<Vector<SharedPtr<DataType>>> LogicalDelete::GetOutputTypes() const {
    SharedPtr<Vector<SharedPtr<DataType>>> result_type = MakeShared<Vector<SharedPtr<DataType>>>();
    result_type->emplace_back(MakeShared<DataType>(LogicalType::kInteger));
    return result_type;
}

String LogicalDelete::ToString(i64 &space) const {
    std::stringstream ss;
    String arrow_str;
    if (space > 3) {
        space -= 4;
        arrow_str = "->  ";
    }
    ss << String(space, ' ') << arrow_str << "DELETE FROM ";
    ss << *table_info_->db_name_ << "." << *table_info_->table_name_;
    space += arrow_str.size();

    return ss.str();
}

} // namespace hybridsearch
