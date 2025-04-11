

module;

export module unnest_expression;

import stl;
import base_expression;
import data_type;

namespace hybridsearch {

class ColumnExpression;

export class UnnestExpression : public BaseExpression {
public:
    explicit UnnestExpression(SharedPtr<BaseExpression> column_expression);

    DataType Type() const override;

    [[nodiscard]] String ToString() const override;
};

} // namespace hybridsearch