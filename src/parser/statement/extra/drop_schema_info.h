

#pragma once

#include "extra_ddl_info.h"

namespace hybridsearch {

class DropSchemaInfo : public ExtraDDLInfo {
public:
    explicit DropSchemaInfo() : ExtraDDLInfo(DDLType::kDatabase) {}

    [[nodiscard]] std::string ToString() const final;
};

} // namespace hybridsearch
