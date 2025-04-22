

#pragma once

#include "base_table_reference.h"

namespace hybridsearch {

class TableReference : public BaseTableReference {

public:
    TableReference() : BaseTableReference(TableRefType::kTable) {}

    ~TableReference() override = default;

    std::string ToString() override;

    std::string db_name_{};
    std::string table_name_{};

    [[nodiscard]] std::string GetTableName() const;
};

} // namespace hybridsearch
