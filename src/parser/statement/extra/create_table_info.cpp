

#include "create_table_info.h"
#include <sstream>

namespace hybridsearch {

CreateTableInfo::~CreateTableInfo() {
    for (auto *&column_def : column_defs_) {
        delete column_def;
        column_def = nullptr;
    }

    for (auto *&table_constraint : constraints_) {
        delete table_constraint;
        table_constraint = nullptr;
    }

    for (auto *&property : properties_) {
        delete property;
        property = nullptr;
    }

    if (select_ != nullptr) {
        delete select_;
        select_ = nullptr;
    }
}

std::string CreateTableInfo::ToString() const {
    std::stringstream ss;
    ss << table_name_ << " ";
    switch (this->conflict_type_) {
        case ConflictType::kIgnore: {
            ss << "Ignore ";
            break;
        }
        case ConflictType::kError: {
            ss << "Error ";
            break;
        }
        case ConflictType::kReplace: {
            ss << "Replace ";
            break;
        }
        default: {
            ParserError("Invalid conflict type.");
        }
    }
    return ss.str();
}

std::string TableConstraint::ToString() const {
    std::stringstream ss;
    ss << ConstrainTypeToString(constraint_) << "(";
    size_t name_count = names_ptr_->size();
    if (name_count > 0) {
        for (size_t idx = 0; idx < name_count - 1; ++idx) {
            ss << names_ptr_->at(idx) << ", ";
        }
        ss << names_ptr_->back();
    } else {
        ParserError("Table constraint without any columns involved.");
    }
    ss << ")";
    return ss.str();
}

} // namespace hybridsearch
