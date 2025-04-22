

#pragma once

#include "base_table_reference.h"

namespace hybridsearch {

class CrossProductReference : public BaseTableReference {

public:
    explicit CrossProductReference() : BaseTableReference(TableRefType::kCrossProduct) {}

    ~CrossProductReference() override;

    std::string ToString() override;

    std::vector<BaseTableReference *> tables_;
};

} // namespace hybridsearch
