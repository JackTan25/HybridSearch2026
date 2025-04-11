

#pragma once

#include "expression_parser.h"
#include "expression_parser_result.h"

#include "expression_lexer.h"

#include <memory>
#include <string>

namespace hybridsearch {

class ExprParser {

public:
    ExprParser();

    ~ExprParser();

    void Parse(const std::string &expr_text, ExpressionParserResult *result);

    void *scanner_{};
    YY_BUFFER_STATE state_;
};

} // namespace hybridsearch
