

module;

export module case_expression;

import stl;
import base_expression;
import logical_type;
import internal_types;
import data_type;

namespace hybridsearch {

export struct CaseCheck {
    CaseCheck(SharedPtr<BaseExpression> when_expr, SharedPtr<BaseExpression> then_expr)
        : when_expr_(std::move(when_expr)), then_expr_(std::move(then_expr)) {}

    SharedPtr<BaseExpression> when_expr_;
    SharedPtr<BaseExpression> then_expr_;
};

export class CaseExpression : public BaseExpression {
public:
    CaseExpression();

    void AddCaseCheck(const SharedPtr<BaseExpression> &when_expr, const SharedPtr<BaseExpression> &then_expr);

    void AddElseExpr(const SharedPtr<BaseExpression> &else_expr);

    DataType Type() const override;

    String ToString() const override;

    Vector<CaseCheck> &CaseExpr() { return case_check_; }

    SharedPtr<BaseExpression> &ElseExpr() { return else_expr_; }

    inline void SetReturnType(DataType type) { return_type_ = type; }

private:
    Vector<CaseCheck> case_check_;
    SharedPtr<BaseExpression> else_expr_;
    DataType return_type_{LogicalType::kInvalid};
};

} // namespace hybridsearch
