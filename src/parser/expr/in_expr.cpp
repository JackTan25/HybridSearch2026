

#include "in_expr.h"
#include <sstream>

namespace hybridsearch {

InExpr::~InExpr() {
    if (left_ != nullptr) {
        delete left_;
        left_ = nullptr;
    }

    if (arguments_ != nullptr) {
        for (auto &expr_ptr : *arguments_) {
            delete expr_ptr;
        }
        delete arguments_;
        arguments_ = nullptr;
    }
}

std::string InExpr::ToString() const {
    std::stringstream ss;
    ss << left_->ToString();
    if (in_) {
        ss << "IN (";
    } else {
        ss << "NOT IN (";
    }
    if (arguments_ != nullptr) {
        for (ParsedExpr *expr_ptr : *arguments_) {
            ss << expr_ptr->ToString();
        }
    }
    ss << ')';
    return ss.str();
}

} // namespace hybridsearch
