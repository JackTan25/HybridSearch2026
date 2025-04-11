

#include "insert_row_expr.h"
#include <sstream>

namespace hybridsearch {

std::string InsertRowExpr::ToString() const {
    std::ostringstream oss;
    oss << "InsertRowExpr(columns: [";
    for (size_t i = 0; i < columns_.size(); ++i) {
        if (i > 0) {
            oss << ", ";
        }
        oss << columns_[i];
    }
    oss << "], values: [";
    for (size_t i = 0; i < values_.size(); ++i) {
        if (i > 0) {
            oss << ", ";
        }
        oss << values_[i]->ToString();
    }
    oss << "])";
    return std::move(oss).str();
}

} // namespace hybridsearch
