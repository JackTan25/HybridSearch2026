

#pragma once

#include "base_statement.h"
#include "expr/insert_row_expr.h"
#include "statement/select_statement.h"

namespace hybridsearch {

class InsertStatement final : public BaseStatement {
public:
    InsertStatement() : BaseStatement(StatementType::kInsert) {}

    ~InsertStatement() override = default;

    [[nodiscard]] std::string ToString() const override { return "insert statement"; }

    std::string schema_name_{};
    std::string table_name_{};

    std::vector<std::unique_ptr<InsertRowExpr>> insert_rows_{};

    std::vector<std::string> columns_for_select_{};
    std::unique_ptr<SelectStatement> select_{};
};

} // namespace hybridsearch
