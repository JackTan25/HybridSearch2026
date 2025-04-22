

#pragma once

#include "parsed_expr.h"

namespace hybridsearch {

class BetweenExpr : public ParsedExpr {
public:
    explicit BetweenExpr() : ParsedExpr(ParsedExprType::kBetween) {}

    ~BetweenExpr() override;

    [[nodiscard]] std::string ToString() const override;

public:
    ParsedExpr *value_{nullptr};
    ParsedExpr *upper_bound_{nullptr};
    ParsedExpr *lower_bound_{nullptr};
};

} // namespace hybridsearch
