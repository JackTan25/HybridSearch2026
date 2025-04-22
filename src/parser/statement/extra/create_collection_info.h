

#pragma once

#include "extra_ddl_info.h"

namespace hybridsearch {

class CreateCollectionInfo final : public ExtraDDLInfo {
public:
    explicit CreateCollectionInfo() : ExtraDDLInfo(DDLType::kCollection) {}

    [[nodiscard]] virtual std::string ToString() const final;

    std::string schema_name_;
    std::string collection_name_{};
};

} // namespace hybridsearch
