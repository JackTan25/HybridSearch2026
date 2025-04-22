

#pragma once

#include "extra_ddl_info.h"

namespace hybridsearch {

class CreateSchemaInfo final : public ExtraDDLInfo {
public:
    explicit CreateSchemaInfo() : ExtraDDLInfo(DDLType::kDatabase) {}

    [[nodiscard]] std::string ToString() const final;
};

} // namespace hybridsearch
