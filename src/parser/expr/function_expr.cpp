

#include "function_expr.h"
#include "expr/column_expr.h"
#include <sstream>

namespace hybridsearch {

FunctionExpr::~FunctionExpr() {
    if (arguments_ != nullptr) {
        for (auto &expr_ptr : *arguments_) {
            delete expr_ptr;
        }
        delete arguments_;
        arguments_ = nullptr;
    }
}

std::string FunctionExpr::ToString() const {
    std::stringstream ss;

    if (arguments_ == nullptr) {
        // No argument function
        ss << func_name_ << "()";
        return ss.str();
    }

    if (arguments_->size() == 1) {
        // Unary argument function
        ColumnExpr *column_expr = dynamic_cast<ColumnExpr *>(arguments_->at(0));
        if (column_expr && column_expr->star_) {
            ss << func_name_ << "(star)";
            return ss.str();
        } else {
            ss << func_name_ << '(' << arguments_->at(0)->ToString() << ")";
            return ss.str();
        }
    }
    if (arguments_->size() == 2) {
        // Binary argument function
        ss << '(' << arguments_->at(0)->ToString() << " " << func_name_ << " " << arguments_->at(1)->ToString() << ")";
        return ss.str();
    }

    ss << func_name_ << '(';
    if (arguments_ != nullptr) {
        for (ParsedExpr *expr_ptr : *arguments_) {
            ss << expr_ptr->ToString();
        }
    }
    ss << ')';
    return ss.str();
}

} // namespace hybridsearch
