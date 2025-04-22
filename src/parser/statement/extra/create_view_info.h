

#pragma once

#include "extra_ddl_info.h"
#include "statement/select_statement.h"

namespace hybridsearch {

class CreateViewInfo final : public ExtraDDLInfo {
public:
    explicit CreateViewInfo() : ExtraDDLInfo(DDLType::kView) {}

    ~CreateViewInfo() final;

    [[nodiscard]] std::string ToString() const final;

    std::string view_name_{};

    std::vector<std::string> *view_columns_{};
    SelectStatement *select_{nullptr};
};

} // namespace hybridsearch
