

#include "copy_statement.h"
#include <sstream>

namespace hybridsearch {

CopyStatement::~CopyStatement() {
    if (expr_array_ != nullptr) {
        for (auto &expr_ptr : *expr_array_) {
            delete expr_ptr;
        }
        delete expr_array_;
        expr_array_ = nullptr;
    }
}

std::string CopyStatement::ToString() const {
    std::stringstream ss;
    std::string copy_direction;
    if (copy_from_) {
        copy_direction = " FROM ";
    } else {
        copy_direction = " TO ";
    }

    std::string file_format;
    switch (copy_file_type_) {
        case CopyFileType::kCSV: {
            file_format = "CSV";
            break;
        }
        case CopyFileType::kJSON: {
            file_format = "JSON";
            break;
        }
        case CopyFileType::kFVECS: {
            file_format = "FVECS";
            break;
        }
        case CopyFileType::kJSONL: {
            file_format = "JSONL";
            break;
        }
        case CopyFileType::kCSR: {
            file_format = "CSR";
            break;
        }
        case CopyFileType::kBVECS: {
            file_format = "BVECS";
            break;
        }
        case CopyFileType::kPARQUET: {
            file_format = "PARQUET";
            break;
        }
        case CopyFileType::kInvalid: {
            file_format = "Invalid";
            break;
        }
    }

    ss << "COPY: " << schema_name_ << "." << table_name_ << copy_direction << file_path_ << " WITH " << file_format << " delimiter: " << delimiter_;
    return ss.str();
}

} // namespace hybridsearch
