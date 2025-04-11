

module;

import stl;
import table_function;

module table_function_set;

namespace hybridsearch {

TableFunctionSet::~TableFunctionSet() { functions_.clear(); }

void TableFunctionSet::AddFunction(TableFunction func) { functions_.emplace_back(func); }

} // namespace hybridsearch
