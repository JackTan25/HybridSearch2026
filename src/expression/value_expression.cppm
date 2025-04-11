

module;

import base_expression;
import bound_select_statement;

import column_vector;
import expression_type;
import value;
import stl;
import internal_types;
import data_type;

export module value_expression;

namespace hybridsearch {

export class ValueExpression : public BaseExpression {
public:
    explicit ValueExpression(Value value) : BaseExpression(ExpressionType::kValue, {}), value_(std::move(value)) {}

    String ToString() const override;

    inline DataType Type() const override { return value_.type(); }

    inline void AppendToChunk(SharedPtr<ColumnVector> &column_vector) { column_vector->AppendValue(value_); }

    const Value &GetValue() const { return value_; }

    u64 Hash() const override;

    bool Eq(const BaseExpression &other) const override;

private:
    Value value_;
};

} // namespace hybridsearch
