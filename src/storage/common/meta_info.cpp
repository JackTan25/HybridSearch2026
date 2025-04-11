

module;

#include <string>

module meta_info;

import stl;
import status;
import column_def;
import hybridsearch_exception;

namespace hybridsearch {

u64 TableInfo::GetColumnIdByName(const String &column_name) const {
    auto iter = std::find_if(column_defs_.begin(), column_defs_.end(), [column_name](const SharedPtr<ColumnDef> &column_def) {
        return column_def->name() == column_name;
    });
    if (iter == column_defs_.end()) {
        Status status = Status::ColumnNotExist(column_name);
        RecoverableError(status);
    }
    return (*iter)->id();
}

const ColumnDef *TableInfo::GetColumnDefByID(ColumnID column_id) const {
    auto iter = std::find_if(column_defs_.begin(), column_defs_.end(), [column_id](const SharedPtr<ColumnDef> &column_def) {
        return static_cast<ColumnID>(column_def->id()) == column_id;
    });
    if (iter == column_defs_.end()) {
        return nullptr;
    }
    return iter->get();
}

const ColumnDef *TableInfo::GetColumnDefByIdx(SizeT idx) const {
    if (idx >= column_defs_.size()) {
        return nullptr;
    }
    return column_defs_[idx].get();
}

const ColumnDef *TableInfo::GetColumnDefByName(const String &column_name) const {
    auto iter = std::find_if(column_defs_.begin(), column_defs_.end(), [column_name](const SharedPtr<ColumnDef> &column_def) {
        return column_def->name() == column_name;
    });
    if (iter == column_defs_.end()) {
        return nullptr;
    }
    return iter->get();
}

SizeT TableInfo::GetColumnIdxByID(ColumnID column_id) const {
    auto iter = std::find_if(column_defs_.begin(), column_defs_.end(), [column_id](const SharedPtr<ColumnDef> &column_def) {
        return static_cast<ColumnID>(column_def->id()) == column_id;
    });
    if (iter == column_defs_.end()) {
        return -1;
    }
    return std::distance(column_defs_.begin(), iter);
}

} // namespace hybridsearch