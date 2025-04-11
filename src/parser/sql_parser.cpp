

#include "sql_parser.h"

#include "lexer.h"

#include <iostream>

namespace hybridsearch {

SQLParser::SQLParser() {
    scanner_ = nullptr;
    state_ = nullptr;
    if (sqllex_init(&scanner_)) {
        std::cerr << "Init lexer error" << std::endl;
    }
}

SQLParser::~SQLParser() { sqllex_destroy(scanner_); }

void SQLParser::Parse(const std::string &sql_text, ParserResult *result) {

    state_ = sql_scan_string(sql_text.c_str(), scanner_);

    // WARNNING: shall reset result to avoid polluting later Parse!
    result->Reset();
    if (sqlparse(scanner_, result)) {
        std::cerr << "Parse error: " << sql_text << std::endl;
    }

    sql_delete_buffer(state_, scanner_);
}

} // namespace hybridsearch
