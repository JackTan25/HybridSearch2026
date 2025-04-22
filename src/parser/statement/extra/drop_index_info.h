

#pragma once

#include "extra_ddl_info.h"
#include "statement/statement_common.h"

namespace hybridsearch {

class DropIndexInfo : public ExtraDDLInfo {
public:
    explicit DropIndexInfo() : ExtraDDLInfo(DDLType::kIndex) {}

    [[nodiscard]] std::string ToString() const final;

    std::string table_name_{};
    std::string index_name_{};
};

} // namespace hybridsearch
