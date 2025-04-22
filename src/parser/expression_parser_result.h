

#pragma once

#include "expr/parsed_expr.h"

#include <vector>

namespace hybridsearch {

class ExpressionParserResult {
public:
    ~ExpressionParserResult();

    void SetErrorMessage(const char *msg, size_t error_line, size_t error_position);

    std::string ToString();

    [[nodiscard]] inline bool IsError() const { return !error_message_.empty(); }

    void Reset() {
        if (exprs_ptr_ != nullptr) {
            for (auto *expr_ptr : *exprs_ptr_) {
                delete expr_ptr;
            }
            delete exprs_ptr_;
            exprs_ptr_ = nullptr;
        }
        error_message_.clear();
        error_line_ = 0;
        error_position_ = 0;
    }

    std::vector<ParsedExpr *> *exprs_ptr_;
    std::string error_message_;
    size_t error_line_;
    size_t error_position_;
};

} // namespace hybridsearch
