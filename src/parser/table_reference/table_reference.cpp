

#include "table_reference.h"

namespace hybridsearch {

std::string TableReference::ToString() { return "TableReference"; }

std::string TableReference::GetTableName() const {
    if (alias_ != nullptr) {
        return std::string(alias_->alias_);
    }
    return table_name_;
}

}; // namespace hybridsearch
