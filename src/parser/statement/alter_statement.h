

#pragma once

#include "base_statement.h"
#include "definition/column_def.h"
#include "statement/statement_common.h"

namespace hybridsearch {

enum class AlterStatementType : uint8_t {
    kInvalid,
    kRenameTable,
    kAddColumns,
    kDropColumns,
};

class AlterStatement : public BaseStatement {
public:
    AlterStatement(const char *schema_name, const char *table_name, AlterStatementType type) : BaseStatement(StatementType::kAlter), type_(type) {
        if (schema_name != nullptr) {
            schema_name_ = schema_name;
        }
        table_name_ = table_name;
    }

    ~AlterStatement() = default;

    std::string schema_name_;
    std::string table_name_;

    AlterStatementType type_{AlterStatementType::kInvalid};
};

class RenameTableStatement final : public AlterStatement {
public:
    RenameTableStatement(const char *schema_name, const char *table_name)
        : AlterStatement(schema_name, table_name, AlterStatementType::kRenameTable) {}

    ~RenameTableStatement() = default;

    std::string ToString() const final { return "RenameTableStatement"; }

    std::string new_table_name_;
};

class AddColumnsStatement final : public AlterStatement {
public:
    AddColumnsStatement(const char *schema_name, const char *table_name) : AlterStatement(schema_name, table_name, AlterStatementType::kAddColumns) {}

    ~AddColumnsStatement() = default;

    std::string ToString() const final { return "AddColumnsStatement"; }

    std::vector<std::shared_ptr<ColumnDef>> column_defs_{};
};

class DropColumnsStatement final : public AlterStatement {
public:
    DropColumnsStatement(const char *schema_name, const char *table_name)
        : AlterStatement(schema_name, table_name, AlterStatementType::kDropColumns) {}

    ~DropColumnsStatement() = default;

    std::string ToString() const final { return "DropColumnsStatement"; }

    std::vector<std::string> column_names_;
};

} // namespace hybridsearch
