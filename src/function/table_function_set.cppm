

module;

import function_set;
import stl;
import table_function;
import function;

export module table_function_set;

namespace hybridsearch {

class TableFunctionSet final : public FunctionSet {
public:
    explicit TableFunctionSet(String name) : FunctionSet(std::move(name), FunctionType::kTable) {}

    ~TableFunctionSet() final;

    void AddFunction(TableFunction func);

private:
    Vector<TableFunction> functions_;
};

} // namespace hybridsearch
