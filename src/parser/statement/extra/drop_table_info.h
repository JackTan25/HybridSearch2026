

#pragma once

#include "extra_ddl_info.h"
#include "statement/statement_common.h"

namespace hybridsearch {

class DropTableInfo : public ExtraDDLInfo {
public:
    explicit DropTableInfo() : ExtraDDLInfo(DDLType::kTable) {}

    [[nodiscard]] std::string ToString() const final;

    std::string table_name_{};
};

} // namespace hybridsearch
