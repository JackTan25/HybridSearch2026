

#pragma once

#include "tensor_type.h"

namespace hybridsearch {

#pragma pack(1)

struct MultiVectorType {
    uint64_t embedding_num_ : 16 = 0;
    uint64_t file_offset_ : 48 = 0;

    [[nodiscard]] static std::string
    MultiVector2String(const char *multivec_ptr, EmbeddingDataType type, size_t embedding_dim, size_t embedding_num) {
        return TensorType::Tensor2String(multivec_ptr, type, embedding_dim, embedding_num);
    }
};

static_assert(sizeof(MultiVectorType) == sizeof(uint64_t));

#pragma pack()

} // namespace hybridsearch
