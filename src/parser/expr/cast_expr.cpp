

#include "cast_expr.h"

namespace hybridsearch {

CastExpr::~CastExpr() {
    if (expr_ != nullptr) {
        delete expr_;
        expr_ = nullptr;
    }
}

std::string CastExpr::ToString() const {
    std::stringstream ss;
    ss << "CAST(" << expr_->ToString() << "  AS " << data_type_.ToString() << ")";
    return ss.str();
}

} // namespace hybridsearch
