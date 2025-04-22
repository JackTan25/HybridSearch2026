

#pragma once

#include "base_statement.h"
#include "expr/parsed_expr.h"
#include <string>

namespace hybridsearch {

class DeleteStatement final : public BaseStatement {
public:
    DeleteStatement() : BaseStatement(StatementType::kDelete) {}

    ~DeleteStatement() final;

    [[nodiscard]] std::string ToString() const final;

    std::string schema_name_{};
    std::string table_name_{};

    ParsedExpr *where_expr_{nullptr};
};

} // namespace hybridsearch
