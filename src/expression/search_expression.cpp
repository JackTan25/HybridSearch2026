

module;

#include <sstream>
module search_expression;
import stl;
import expression_type;
import scalar_function;
import match_expression;
import knn_expression;
import match_tensor_expression;
import fusion_expression;

namespace hybridsearch {

SearchExpression::SearchExpression(Vector<SharedPtr<BaseExpression>> &match_exprs, Vector<SharedPtr<FusionExpression>> &fusion_exprs)
    : BaseExpression(ExpressionType::kSearch, Vector<SharedPtr<BaseExpression>>()), match_exprs_(match_exprs), fusion_exprs_(fusion_exprs) {}

String SearchExpression::ToString() const {
    if (!alias_.empty()) {
        return alias_;
    }

    OStringStream oss;
    oss << "SEARCH(";
    int cnt = 0;
    for (auto &match_expr : match_exprs_) {
        if (cnt != 0)
            oss << ", ";
        cnt++;
        oss << match_expr->ToString();
    }
    for (auto &fusion_expr_ : fusion_exprs_) {
        if (cnt != 0)
            oss << ", ";
        cnt++;
        oss << fusion_expr_->ToString();
    }
    oss << ")";
    return oss.str();
}

} // namespace hybridsearch
