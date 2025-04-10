

#include "create_index_info.h"
#include "parser_assert.h"

#include <sstream>

namespace hybridsearch {

std::string IndexInfo::IndexTypeToString(IndexType index_type) {
    switch (index_type) {
        case IndexType::kIVF: {
            return "IVF";
        }
        case IndexType::kHnsw: {
            return "HNSW";
        }
        case IndexType::kFullText: {
            return "FULLTEXT";
        }
        case IndexType::kSecondary: {
            return "SECONDARY";
        }
        case IndexType::kEMVB: {
            return "EMVB";
        }
        case IndexType::kBMP: {
            return "BMP";
        }
        case IndexType::kDiskAnn: {
            return "DISKANN";
        }
        case IndexType::kInvalid: {
            ParserError("Invalid conflict type.");
        }
    }
    return {};
}

IndexType IndexInfo::StringToIndexType(const std::string &index_type_str) {
    if (index_type_str == "IVF") {
        return IndexType::kIVF;
    } else if (index_type_str == "HNSW") {
        return IndexType::kHnsw;
    } else if (index_type_str == "FULLTEXT") {
        return IndexType::kFullText;
    } else if (index_type_str == "SECONDARY") {
        return IndexType::kSecondary;
    } else if (index_type_str == "EMVB") {
        return IndexType::kEMVB;
    } else if (index_type_str == "BMP") {
        return IndexType::kBMP;
    } else if (index_type_str == "DISKANN") {
        return IndexType::kDiskAnn;
    } else {
        return IndexType::kInvalid;
    }
}

IndexInfo::~IndexInfo() {
    if (index_param_list_ != nullptr) {
        for (auto &index_param : *index_param_list_) {
            delete index_param;
            index_param = nullptr;
        }
        delete index_param_list_;
        index_param_list_ = nullptr;
    }
}

CreateIndexInfo::~CreateIndexInfo() {
    if (index_info_ != nullptr) {
        delete index_info_;
        index_info_ = nullptr;
    }
}

std::string CreateIndexInfo::ToString() const {
    std::stringstream ss;
    ss << "CREATE INDEX ";
    switch (conflict_type_) {
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
        case ConflictType::kInvalid: {
            ParserError("Invalid conflict type.");
        }
    }
    if (index_name_.empty()) {
        ss << "DEFAULT_INDEX_NAME ";
    } else {
        ss << index_name_;
    }
    ss << " ON " << table_name_ << "(";
    ss << index_info_->column_name_;
    ss << " USING " << IndexInfo::IndexTypeToString(index_info_->index_type_);
    if (index_info_->index_param_list_ != nullptr && !index_info_->index_param_list_->empty()) {
        ss << " WITH(";
        size_t param_count = index_info_->index_param_list_->size();
        for (size_t param_idx = 0; param_idx < param_count; ++param_idx) {
            ss << (*index_info_->index_param_list_)[param_idx]->param_name_ << " " << (*index_info_->index_param_list_)[param_idx]->param_value_;
            if (param_idx != param_count - 1) {
                ss << ", ";
            }
        }
        ss << ")";
    }
    ss << ";";
    return ss.str();
}

} // namespace hybridsearch
