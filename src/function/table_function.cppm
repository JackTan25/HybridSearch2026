

module;

export module table_function;

import stl;
import function;
import function_data;

// import query_context;
import data_block;
import logical_type;

namespace hybridsearch {

class QueryContext;

export class TableFunctionData : public FunctionData {
public:
    ~TableFunctionData() override = default;
};

export using TableFunctionType = std::function<void(QueryContext *query_context, TableFunctionData *data, DataBlock &output)>;

export class TableFunction : public Function {
public:
    explicit TableFunction(String name, Vector<LogicalType> argument_types, TableFunctionType function);

    [[nodiscard]] String ToString() const override;

    TableFunctionType main_function_;

private:
    Vector<LogicalType> parameter_types_;
};
} // namespace hybridsearch
