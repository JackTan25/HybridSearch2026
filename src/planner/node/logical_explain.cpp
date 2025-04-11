

module;

module logical_explain;

import stl;
import column_binding;

import base_expression;
import internal_types;

namespace hybridsearch {

Vector<ColumnBinding> LogicalExplain::GetColumnBindings() const { return {}; }

SharedPtr<Vector<String>> LogicalExplain::GetOutputNames() const { return MakeShared<Vector<String>>(); }

SharedPtr<Vector<SharedPtr<DataType>>> LogicalExplain::GetOutputTypes() const { return MakeShared<Vector<SharedPtr<DataType>>>(); }

String LogicalExplain::ToString(i64 &space) const { return "LogicalExplain"; }

} // namespace hybridsearch