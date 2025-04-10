

#include "search_scanner.h"

namespace hybridsearch {

class SearchScannerDerived final : public SearchScanner, public yyFlexLexer {
public:
    explicit SearchScannerDerived(std::istream *in) : yyFlexLexer(in) {}
    // get rid of override virtual function warning
    using FlexLexer::yylex;
    int yylex(SearchParser::semantic_type *lval, SearchParser::location_type *location) override;

private:
    /* yylval ptr */
    SearchParser::semantic_type *yylval = nullptr;
};

} // namespace hybridsearch
