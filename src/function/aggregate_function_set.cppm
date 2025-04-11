

module;

import stl;
import base_expression;
import aggregate_function;
import function;
import function_set;

export module aggregate_function_set;

namespace hybridsearch {

export class AggregateFunctionSet final : public FunctionSet {
public:
    explicit AggregateFunctionSet(String name) : FunctionSet(std::move(name), FunctionType::kAggregate) {}

    ~AggregateFunctionSet() final;

    void AddFunction(const AggregateFunction &func);

    AggregateFunction GetMostMatchFunction(const SharedPtr<BaseExpression> &input_argument);

private:
    static i64 MatchFunctionCost(const AggregateFunction &func, const SharedPtr<BaseExpression> &argument);

    Vector<AggregateFunction> functions_;
};

} // namespace hybridsearch
