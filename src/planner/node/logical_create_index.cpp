

module;

#include <sstream>

module logical_create_index;

import stl;
import column_binding;

import base_expression;
import index_base;
import logical_type;
import internal_types;

namespace hybridsearch {

Vector<ColumnBinding> LogicalCreateIndex::GetColumnBindings() const { return {}; };

SharedPtr<Vector<String>> LogicalCreateIndex::GetOutputNames() const {
    SharedPtr<Vector<String>> result = MakeShared<Vector<String>>();
    result->emplace_back("OK");
    return result;
}

SharedPtr<Vector<SharedPtr<DataType>>> LogicalCreateIndex::GetOutputTypes() const {
    SharedPtr<Vector<SharedPtr<DataType>>> result_type = MakeShared<Vector<SharedPtr<DataType>>>();
    result_type->emplace_back(MakeShared<DataType>(LogicalType::kInteger));
    return result_type;
}

String LogicalCreateIndex::ToString(i64 &space) const {
    std::stringstream ss;
    String arrow_str;
    if (space > 3) {
        space -= 4;
        arrow_str = "->  ";
    }
    ss << String(space, ' ') << arrow_str << "Create Table: " << *base_table_ref_->table_name() << "." << index_definition_->ToString();
    space += arrow_str.size();

    return ss.str();
}
} // namespace hybridsearch
