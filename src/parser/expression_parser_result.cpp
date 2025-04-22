

#include "expression_parser_result.h"

#include <sstream>

namespace hybridsearch {

ExpressionParserResult::~ExpressionParserResult() {
    if (exprs_ptr_ == nullptr)
        return;
    for (auto &expr : *exprs_ptr_) {
        delete expr;
    }
    delete exprs_ptr_;
}

void ExpressionParserResult::SetErrorMessage(const char *msg, size_t error_line, size_t error_position) {
    this->error_message_ = std::string(msg);
    this->error_line_ = error_line;
    this->error_position_ = error_position;
}

std::string ExpressionParserResult::ToString() {
    std::stringstream ss;
    if (error_message_.empty()) {
        // Success
        for (const auto &expr : *exprs_ptr_) {
            ss << expr->ToString() << std::endl;
        }
        return ss.str();
    } else {
        // Fail with error
        ss << "Error(" << error_line_ << ":" << error_position_ << "): " << error_message_;
        return ss.str();
    }
}

} // namespace hybridsearch