

#pragma once

#include "search_parser.h"

namespace hybridsearch {

class SearchScanner {
public:
    virtual ~SearchScanner() = default;
    virtual int yylex(SearchParser::semantic_type *lval, SearchParser::location_type *location) = 0;
};

} // namespace hybridsearch
