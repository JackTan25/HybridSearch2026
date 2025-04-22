

#include "between_expr.h"
#include <sstream>

namespace hybridsearch {

BetweenExpr::~BetweenExpr() {
    delete value_;
    delete upper_bound_;
    delete lower_bound_;
}

std::string BetweenExpr::ToString() const {
    std::stringstream ss;
    ss << value_->ToString() << " BETWEEN " << upper_bound_->ToString() << " AND " << lower_bound_->ToString();
    return ss.str();
}

} // namespace hybridsearch
