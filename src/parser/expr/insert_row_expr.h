

#pragma once

#include "parsed_expr.h"
#include <memory>
#include <vector>

namespace hybridsearch {

class InsertRowExpr : public ParsedExpr {
public:
    explicit InsertRowExpr() : ParsedExpr(ParsedExprType::kInsertRow) {}

    ~InsertRowExpr() override = default;

    [[nodiscard]] std::string ToString() const override;

    std::vector<std::string> columns_{};
    std::vector<std::unique_ptr<ParsedExpr>> values_{};
};

} // namespace hybridsearch
