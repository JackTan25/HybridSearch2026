

#pragma once

#include "base_statement.h"
#include "expr/parsed_expr.h"
#include "statement_common.h"

namespace hybridsearch {

enum class CopyOptionType {
    kFormat,
    kDelimiter,
    kHeader,
    kOffset,
    kLimit,
    kRowLimit,
};

struct CopyOption {
    CopyOptionType option_type_{CopyOptionType::kFormat};
    bool header_{false};
    CopyFileType file_type_{CopyFileType::kInvalid};
    char delimiter_{','};
    size_t offset_{0};
    size_t limit_{0};
    size_t row_limit_{0};
};

class CopyStatement final : public BaseStatement {
public:
    CopyStatement() : BaseStatement(StatementType::kCopy) {}

    ~CopyStatement() final;

    [[nodiscard]] std::string ToString() const final;

    bool copy_from_{false};
    std::string file_path_{};
    std::string table_name_{};
    std::string schema_name_{};
    bool header_{false};
    CopyFileType copy_file_type_{CopyFileType::kInvalid};
    char delimiter_{','};
    size_t offset_{0};
    size_t limit_{0};
    size_t row_limit_{0};

    // EXPORT columns
    std::vector<ParsedExpr *> *expr_array_{nullptr};
};

} // namespace hybridsearch
