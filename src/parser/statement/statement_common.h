

#pragma once

#include "../type/serialize.h"
#include <memory>
#include <string>
#include <vector>

namespace hybridsearch {

constexpr size_t DEFAULT_MATCH_VECTOR_TOP_N = 10;
constexpr size_t DEFAULT_MATCH_SPARSE_TOP_N = 10;

enum class CopyFileType {
    kCSV,
    kJSON,
    kJSONL,
    kFVECS,
    kCSR,
    kBVECS,
    kPARQUET,
    kInvalid,
};

inline std::shared_ptr<std::string> CopyFileTypeToStr(CopyFileType copy_file_type) {
    switch (copy_file_type) {
        case CopyFileType::kCSV:
            return std::make_shared<std::string>("CSV");
        case CopyFileType::kJSON:
            return std::make_shared<std::string>("JSON");
        case CopyFileType::kFVECS:
            return std::make_shared<std::string>("FVECS");
        case CopyFileType::kJSONL:
            return std::make_shared<std::string>("JSONL");
        case CopyFileType::kCSR:
            return std::make_shared<std::string>("CSR");
        case CopyFileType::kBVECS:
            return std::make_shared<std::string>("BVECS");
        case CopyFileType::kPARQUET:
            return std::make_shared<std::string>("PARQUET");
        case CopyFileType::kInvalid:
            return std::make_shared<std::string>("Invalid");
    }
}

struct TableName {
    char *schema_name_ptr_{nullptr};
    char *table_name_ptr_{nullptr};
};

struct InitParameter {
    std::string param_name_{};
    std::string param_value_{};

    std::string ToString() { return param_name_ + ": " + param_value_; }

    size_t GetSizeInBytes() const { return sizeof(int) + param_name_.size() + sizeof(int) + param_value_.size(); }

    void WriteAdv(char *&p) {
        WriteBufAdv(p, param_name_);
        WriteBufAdv(p, param_value_);
    }

    static std::unique_ptr<InitParameter> ReadAdv(const char *&p) {
        auto init_param = std::make_unique<InitParameter>();
        init_param->param_name_ = ReadBufAdv<std::string>(p);
        init_param->param_value_ = ReadBufAdv<std::string>(p);
        return init_param;
    }

    static std::vector<std::unique_ptr<InitParameter>> MakeInitParameterList(std::vector<InitParameter *> *init_params) {
        std::vector<std::unique_ptr<InitParameter>> results;
        for (auto *init_param : *init_params) {
            results.push_back(std::unique_ptr<InitParameter>(init_param));
        }
        delete init_params;
        return results;
    }

    bool operator==(const InitParameter &other) const { return param_name_ == other.param_name_ && param_value_ == other.param_value_; }
};

struct IfExistsInfo {
    bool exists_{false};
    bool if_exists_{false};
    std::string info_{};
};

struct IfNotExistsInfo {
    bool exists_{false};
    bool if_not_exists_{false};
    std::string info_{};
};
} // namespace hybridsearch
