

module;

export module between_expression;

import column_binding;
import base_expression;
import data_type;
import stl;
import internal_types;

namespace hybridsearch {

export class BetweenExpression : public BaseExpression {
public:
    BetweenExpression(const SharedPtr<BaseExpression> &value,
                      const SharedPtr<BaseExpression> &upper_bound,
                      const SharedPtr<BaseExpression> &lower_bound,
                      bool upper_inclusive,
                      bool lower_inclusive);

    String ToString() const override;

    DataType Type() const override;

private:
    bool lower_inclusive_{false};
    bool upper_inclusive_{false};
};

} // namespace hybridsearch
