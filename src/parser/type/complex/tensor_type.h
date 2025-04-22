

#pragma once

#include "embedding_type.h"

namespace hybridsearch {

#pragma pack(1)

struct TensorType {
    uint64_t embedding_num_ : 16 = 0;
    uint64_t file_offset_ : 48 = 0;

    [[nodiscard]] static std::string Tensor2String(const char *tensor_ptr, EmbeddingDataType type, size_t embedding_dimension, size_t embedding_num);
};

static_assert(sizeof(TensorType) == sizeof(uint64_t));

#pragma pack()

} // namespace hybridsearch
