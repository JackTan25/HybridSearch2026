

#pragma once

#include "parser.h"
#include "parser_result.h"

#include "lexer.h"

#include <memory>
#include <string>

namespace hybridsearch {

class SQLParser {

public:
    SQLParser();

    ~SQLParser();

    void Parse(const std::string &sql_text, ParserResult *result);

    void *scanner_{};
    YY_BUFFER_STATE state_;
};

} // namespace hybridsearch
