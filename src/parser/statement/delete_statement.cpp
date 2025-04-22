

#include "delete_statement.h"

namespace hybridsearch {

DeleteStatement::~DeleteStatement() {
    if (where_expr_ != nullptr) {
        delete where_expr_;
        where_expr_ = nullptr;
    }
}

std::string DeleteStatement::ToString() const { return "Delete statement"; }

} // namespace hybridsearch
