

#pragma once

#include "base_statement.h"
#include "statement/extra/create_collection_info.h"
#include "statement/extra/create_index_info.h"
#include "statement/extra/create_schema_info.h"
#include "statement/extra/create_table_info.h"
#include "statement/extra/create_view_info.h"

#include <string>

namespace hybridsearch {

class OptimizeStatement : public BaseStatement {
public:
    explicit OptimizeStatement() : BaseStatement(StatementType::kOptimize) {}

    [[nodiscard]] std::string ToString() const final;

    std::string schema_name_{};
    std::string table_name_{};

    std::string index_name_{};
    std::vector<std::unique_ptr<InitParameter>> opt_params_{};
};

} // namespace hybridsearch