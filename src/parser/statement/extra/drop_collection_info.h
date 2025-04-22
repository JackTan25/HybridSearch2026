

#pragma once

#include "extra_ddl_info.h"
#include "statement/statement_common.h"

namespace hybridsearch {

class DropCollectionInfo final : public ExtraDDLInfo {
public:
    explicit DropCollectionInfo() : ExtraDDLInfo(DDLType::kCollection) {}

    [[nodiscard]] std::string ToString() const final;

    std::string collection_name_{};
};

} // namespace hybridsearch
