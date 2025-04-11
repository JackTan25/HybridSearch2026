

module;

import stl;
import base_expression;

import bind_context;
import expression_binder;
import parsed_expr;

export module bind_alias_proxy;

namespace hybridsearch {

export class BindAliasProxy {
public:
    SharedPtr<BaseExpression>
    BindAlias(ExpressionBinder &expression_binder, const ParsedExpr &expr, BindContext *bind_context_ptr, i64 depth, bool root);

private:
    bool binding_alias_{false};
};

} // namespace hybridsearch
