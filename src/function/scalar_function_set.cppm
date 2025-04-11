

module;

import stl;
import base_expression;
import scalar_function;
import function;
import function_set;

export module scalar_function_set;

namespace hybridsearch {

export class ScalarFunctionSet final : public FunctionSet {
public:
    explicit ScalarFunctionSet(String name) : FunctionSet(std::move(name), FunctionType::kScalar) {}

    ~ScalarFunctionSet() final;

    void AddFunction(const ScalarFunction &func);

    ScalarFunction GetMostMatchFunction(const Vector<SharedPtr<BaseExpression>> &input_arguments);

    const Vector<ScalarFunction> &GetAllScalarFunctions() const { return functions_; }

private:
    static i64 MatchFunctionCost(const ScalarFunction &func, const Vector<SharedPtr<BaseExpression>> &arguments);

    Vector<ScalarFunction> functions_;
};

} // namespace hybridsearch
