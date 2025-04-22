

#pragma once

#include "extra_ddl_info.h"
#include "statement/statement_common.h"
#include <string>
#include <vector>

namespace hybridsearch {

enum class IndexType {
    kIVF,
    kHnsw,
    kBMP,
    kFullText,
    kSecondary,
    kEMVB,
    kInvalid,
    kDiskAnn,
};

struct IndexInfo {
    ~IndexInfo();
    IndexType index_type_{IndexType::kInvalid};
    std::string column_name_{};
    std::vector<InitParameter *> *index_param_list_{nullptr};

    static std::string IndexTypeToString(IndexType index_type);
    static IndexType StringToIndexType(const std::string &index_type_str);
};

class CreateIndexInfo final : public ExtraDDLInfo {
public:
    explicit CreateIndexInfo() : ExtraDDLInfo(DDLType::kIndex) {}

    ~CreateIndexInfo() final;

    [[nodiscard]] std::string ToString() const final;

    std::string schema_name_{};
    std::string table_name_{};
    std::string index_name_{};

    IndexInfo *index_info_{};
};

} // namespace hybridsearch
