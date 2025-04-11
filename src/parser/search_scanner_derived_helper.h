

#ifndef SearchScannerSuffix
#error "SearchScannerSuffix is not defined"
#endif
#define hybridsearch_CONCAT_INNER(A, B) A##B
#define hybridsearch_CONCAT(A, B) hybridsearch_CONCAT_INNER(A, B)
#define SearchScannerDerived hybridsearch_CONCAT(SearchScanner, SearchScannerSuffix)
#define yyFlexLexer hybridsearch_CONCAT(SearchScannerDerived, FlexLexer)
// clang-format off
// the order of these includes is important
#include <FlexLexer.h>
#include "search_scanner_derived.h"
// clang-format on
#undef yyFlexLexer
#undef SearchScannerDerived
#undef hybridsearch_CONCAT
#undef hybridsearch_CONCAT_INNER
#undef SearchScannerSuffix
