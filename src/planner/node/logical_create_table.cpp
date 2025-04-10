

module;

#include <sstream>

module logical_create_table;

import stl;
import column_binding;

import logical_type;
import internal_types;

namespace hybridsearch {

Vector<ColumnBinding> LogicalCreateTable::GetColumnBindings() const { return {}; }

SharedPtr<Vector<String>> LogicalCreateTable::GetOutputNames() const {
    SharedPtr<Vector<String>> result = MakeShared<Vector<String>>();
    result->emplace_back("OK");
    return result;
}

SharedPtr<Vector<SharedPtr<DataType>>> LogicalCreateTable::GetOutputTypes() const {
    SharedPtr<Vector<SharedPtr<DataType>>> result_type = MakeShared<Vector<SharedPtr<DataType>>>();
    result_type->emplace_back(MakeShared<DataType>(LogicalType::kInteger));
    return result_type;
}

String LogicalCreateTable::ToString(i64 &space) const {
    std::stringstream ss;
    String arrow_str;
    if (space > 3) {
        space -= 4;
        arrow_str = "->  ";
    }
    ss << String(space, ' ') << arrow_str << "Create Table: " << *schema_name_ << "." << table_definition_->ToString();
    space += arrow_str.size();

    return ss.str();
}

} // namespace hybridsearch
