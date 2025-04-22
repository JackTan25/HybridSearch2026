

#include "parser_helper.h"
#include <cctype>
#include <cstring>

void ParserHelper::ToLower(char *str) {
    if (str == nullptr) {
        return;
    }
    size_t str_len = strlen(str);
    for (size_t i = 0; i < str_len; ++i) {
        str[i] = std::tolower(str[i]);
    }
}

void ParserHelper::ToUpper(char *str) {
    if (str == nullptr) {
        return;
    }
    size_t str_len = strlen(str);
    for (size_t i = 0; i < str_len; ++i) {
        str[i] = std::toupper(str[i]);
    }
}
