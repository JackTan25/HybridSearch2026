

#pragma once

#include <utility>

#include "definition/column_def.h"
#include "extra_ddl_info.h"
#include "statement/select_statement.h"
#include "statement/statement_common.h"
#include "type/data_type.h"
#include "type/info/bitmap_info.h"
#include "type/info/decimal_info.h"
#include "type/info/embedding_info.h"
#include "type/info/sparse_info.h"
#include <unordered_set>

namespace hybridsearch {

class CreateTableInfo final : public ExtraDDLInfo {
public:
    explicit CreateTableInfo() : ExtraDDLInfo(DDLType::kTable) {}

    ~CreateTableInfo() final;

    [[nodiscard]] std::string ToString() const final;

    std::string table_name_{};

    std::vector<ColumnDef *> column_defs_{};
    std::vector<TableConstraint *> constraints_{};

    std::vector<hybridsearch::InitParameter *> properties_{};

    SelectStatement *select_{nullptr};
};

} // namespace hybridsearch
