

#include "expr_parser.h"

#include "lexer.h"

#include <iostream>

namespace hybridsearch {

ExprParser::ExprParser() {
    scanner_ = nullptr;
    state_ = nullptr;
    if (sqllex_init(&scanner_)) {
        std::cerr << "Init lexer error" << std::endl;
    }
}

ExprParser::~ExprParser() { expressionlex_destroy(scanner_); }

void ExprParser::Parse(const std::string &expr_text, ExpressionParserResult *result) {

    state_ = expression_scan_string(expr_text.c_str(), scanner_);

    // WARNING: shall reset result to avoid polluting later Parse!
    result->Reset();
    if (expressionparse(scanner_, result)) {
        std::cerr << "Parse expression error: " << expr_text << std::endl;
    }

    expression_delete_buffer(state_, scanner_);
}

} // namespace hybridsearch
