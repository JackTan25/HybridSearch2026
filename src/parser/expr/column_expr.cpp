

#include "column_expr.h"
#include "parser_assert.h"
#include <sstream>

namespace hybridsearch {

ColumnExpr::~ColumnExpr() = default;

std::string ColumnExpr::ToString() const {
    std::stringstream ss;
    size_t name_count = names_.size();
    if (name_count == 0) {
        if (star_) {
            return "*";
        }
        ParserError("Empty column expression");
        return std::string();
    }
    for (size_t i = 0; i < name_count - 1; ++i) {
        ss << names_[i] << ".";
    }
    ss << names_[name_count - 1];
    if (star_) {
        ss << ".*";
    }

    return ss.str();
}

} // namespace hybridsearch
