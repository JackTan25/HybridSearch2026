

module;

#include <sstream>

module logical_update;

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

Vector<ColumnBinding> LogicalUpdate::GetColumnBindings() const { return {}; }

SharedPtr<Vector<String>> LogicalUpdate::GetOutputNames() const {
    SharedPtr<Vector<String>> result = MakeShared<Vector<String>>();
    result->emplace_back("OK");
    return result;
}

SharedPtr<Vector<SharedPtr<DataType>>> LogicalUpdate::GetOutputTypes() const {
    SharedPtr<Vector<SharedPtr<DataType>>> result_type = MakeShared<Vector<SharedPtr<DataType>>>();
    result_type->emplace_back(MakeShared<DataType>(LogicalType::kInteger));
    return result_type;
}

String LogicalUpdate::ToString(i64 &space) const {
    std::stringstream ss;
    String arrow_str;
    if (space > 3) {
        space -= 4;
        arrow_str = "->  ";
    }
    ss << String(space, ' ') << arrow_str << "UPDATE ";
    ss << *table_info_->db_name_ << "." << *table_info_->table_name_;
    ss << " SET";
    for (SizeT i = 0; i < update_columns_.size(); i++) {
        if (i > 0)
            ss << ", ";
        ss << " ";
        ss << table_info_->GetColumnDefByID(update_columns_[i].first)->name_;
        ss << " = " << update_columns_[i].second->Name();
    }
    return ss.str();
}

} // namespace hybridsearch
