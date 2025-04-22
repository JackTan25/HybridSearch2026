

#pragma once

#include "embedding_type.h"
#include <span>

namespace hybridsearch {

#pragma pack(1)

struct TensorArrayType {
    uint64_t tensor_num_ : 16 = 0;
    uint64_t file_offset_ : 48 = 0;

    [[nodiscard]] static std::string
    TensorArray2String(const std::vector<std::pair<std::span<const char>, uint64_t>> &raw_data, EmbeddingDataType type, size_t embedding_dimension);
};

static_assert(sizeof(TensorArrayType) == sizeof(uint64_t));

#pragma pack()

} // namespace hybridsearch
