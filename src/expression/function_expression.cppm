

module;

export module function_expression;

import stl;
import base_expression;
import data_type;
import scalar_function;
import internal_types;

namespace hybridsearch {

export class FunctionExpression : public BaseExpression {
public:
    FunctionExpression(ScalarFunction function, Vector<SharedPtr<BaseExpression>> arguments);

    inline DataType Type() const override { return func_.return_type(); }

    [[nodiscard]] inline const String &ScalarFunctionName() const { return func_.name(); }

    String ToString() const override;

    u64 Hash() const override;

    bool Eq(const BaseExpression &other) const override;

public:
    ScalarFunction func_;
    bool nullary_{false};
};

} // namespace hybridsearch
