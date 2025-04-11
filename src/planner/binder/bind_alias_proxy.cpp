

module;

module bind_alias_proxy;

import stl;
import base_expression;

import bind_context;
import expression_binder;
import status;
import hybridsearch_exception;
import third_party;
import parsed_expr;
import logger;

namespace hybridsearch {

SharedPtr<BaseExpression>
BindAliasProxy::BindAlias(ExpressionBinder &expression_binder, const ParsedExpr &expr, BindContext *bind_context_ptr, i64 depth, bool root) {
    String expr_name = expr.GetName();

    auto alias_pair = bind_context_ptr->select_alias2index_.find(expr_name);
    if (alias_pair == bind_context_ptr->select_alias2index_.end()) {
        return nullptr;
    }

    const ParsedExpr *select_expr = bind_context_ptr->select_expression_[alias_pair->second];

    if (binding_alias_) {
        Status status = Status::SyntaxError(fmt::format("Trying to bind an alias table_name: {} in another alias", expr_name));
        RecoverableError(status);
    }

    binding_alias_ = true;
    SharedPtr<BaseExpression> bound_alias_expr = expression_binder.BuildExpression(*select_expr, bind_context_ptr, depth, root);
    binding_alias_ = false;

    return bound_alias_expr;
}

} // namespace hybridsearch
