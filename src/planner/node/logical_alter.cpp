

module;

module logical_alter;

import logical_type;

namespace hybridsearch {

Vector<ColumnBinding> LogicalAlter::GetColumnBindings() const { return {}; }

SharedPtr<Vector<String>> LogicalAlter::GetOutputNames() const {
    SharedPtr<Vector<String>> result = MakeShared<Vector<String>>();
    result->emplace_back("OK");
    return result;
}

SharedPtr<Vector<SharedPtr<DataType>>> LogicalAlter::GetOutputTypes() const {
    SharedPtr<Vector<SharedPtr<DataType>>> result_type = MakeShared<Vector<SharedPtr<DataType>>>();
    result_type->emplace_back(MakeShared<DataType>(LogicalType::kInteger));
    return result_type;
}

String LogicalRenameTable::ToString(i64 &space) const {
    //
    return "";
}

String LogicalAddColumns::ToString(i64 &space) const {
    //
    return "";
}

String LogicalDropColumns::ToString(i64 &space) const {
    //
    return "";
}

} // namespace hybridsearch