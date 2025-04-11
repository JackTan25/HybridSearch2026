

module;

export module cast_expression;

import column_binding;

import stl;
import expression_type;
import bound_cast_func;
import base_expression;
import internal_types;
import data_type;

namespace hybridsearch {

export class CastExpression : public BaseExpression {
public:
    CastExpression(BoundCastFunc cast_function, const SharedPtr<BaseExpression> &argument, DataType target_type)
        : BaseExpression(ExpressionType::kCast, {argument}), func_(cast_function), target_type_(std::move(target_type)) {}

    inline DataType Type() const override { return target_type_; }

    String ToString() const override;

    static bool CanCast(const DataType &source, const DataType &target);

    static SharedPtr<BaseExpression> AddCastToType(const SharedPtr<BaseExpression> &expr, const DataType &target_type);

    BoundCastFunc func_;

    u64 Hash() const override;

    bool Eq(const BaseExpression &other) const override;

private:
    DataType target_type_;
};

} // namespace hybridsearch
