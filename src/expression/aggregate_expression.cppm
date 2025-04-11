

module;

export module aggregate_expression;

import column_binding;
import base_expression;
import data_type;
import stl;
import aggregate_function;
import internal_types;

namespace hybridsearch {

export class AggregateExpression : public BaseExpression {
public:
    explicit AggregateExpression(AggregateFunction aggregate_function, Vector<SharedPtr<BaseExpression>> arguments);

    DataType Type() const override;

    [[nodiscard]] String ToString() const override;

    bool IsCountStar() const;

public:
    AggregateFunction aggregate_function_;
};

} // namespace hybridsearch
