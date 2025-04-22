

#pragma once

#include "parsed_expr.h"
#include <string>
#include <vector>

namespace hybridsearch {

class InExpr : public ParsedExpr {
public:
    explicit InExpr(bool in = true) : ParsedExpr(ParsedExprType::kIn), in_(in) {}

    ~InExpr() override;

    [[nodiscard]] std::string ToString() const override;

public:
    ParsedExpr *left_{nullptr};
    std::vector<ParsedExpr *> *arguments_{nullptr};
    bool in_{false};
};

} // namespace hybridsearch
