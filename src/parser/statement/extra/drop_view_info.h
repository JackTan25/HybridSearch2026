

#pragma once

#include "extra_ddl_info.h"
#include "statement/statement_common.h"

namespace hybridsearch {

class DropViewInfo : public ExtraDDLInfo {
public:
    explicit DropViewInfo() : ExtraDDLInfo(DDLType::kView) {}

    [[nodiscard]] std::string ToString() const final;

    std::string view_name_{};
};

} // namespace hybridsearch
