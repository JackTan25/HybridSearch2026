

#include "base_table_reference.h"

namespace hybridsearch {

TableAlias::~TableAlias() {

    if (alias_ != nullptr) {
        free(alias_);
        alias_ = nullptr;
    }

    if (column_alias_array_ != nullptr) {
        delete column_alias_array_;
        column_alias_array_ = nullptr;
    }
}

BaseTableReference::~BaseTableReference() {
    if (alias_ != nullptr) {
        delete alias_;
        alias_ = nullptr;
    }
}

} // namespace hybridsearch