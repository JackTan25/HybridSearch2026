

module;

import stl;
import base_expression;
import internal_types;
import data_type;

export module correlated_column_expression;

namespace hybridsearch {

class CorrelatedColumnExpression : public BaseExpression {
public:
    CorrelatedColumnExpression(DataType data_type, String column_name);

    inline DataType Type() const override { return data_type_; }

    String ToString() const override;

private:
    DataType data_type_;
    String column_name_;
};

} // namespace hybridsearch