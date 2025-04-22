

module;

export module conjunction_expression;

import stl;
import base_expression;
import data_type;
import logical_type;
import internal_types;

namespace hybridsearch {

export enum class ConjunctionType {
    kInvalid,
    kAnd,
    kOr,
};

export class ConjunctionExpression : public BaseExpression {
public:
    ConjunctionExpression(ConjunctionType conjunction_type,
                          const SharedPtr<BaseExpression> &left_operand,
                          const SharedPtr<BaseExpression> &right_operand);

    inline DataType Type() const override { return DataType{LogicalType::kBoolean}; }

    String ToString() const override;

    ConjunctionType conjunction_type() const { return conjunction_type_; }

private:
    ConjunctionType conjunction_type_{ConjunctionType::kInvalid};
};

} // namespace hybridsearch