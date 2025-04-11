

#include "tensor_type.h"
#include <iostream>
#include <sstream>

namespace hybridsearch {

std::string
TensorType::Tensor2String(const char *tensor_p, const EmbeddingDataType type, const size_t embedding_dimension, const size_t embedding_num) {
    auto *tensor_ptr = const_cast<char *>(tensor_p);
    const size_t embedding_size = EmbeddingType::EmbeddingSize(type, embedding_dimension);
    std::ostringstream oss;
    // first embedding
    EmbeddingType embedding(nullptr, false);
    embedding.ptr = tensor_ptr;
    oss << '[' << EmbeddingType::Embedding2String(embedding, type, embedding_dimension);
    for (size_t i = 1; i < embedding_num; ++i) {
        // next embedding
        embedding.ptr = tensor_ptr + i * embedding_size;
        oss << ',' << EmbeddingType::Embedding2String(embedding, type, embedding_dimension);
    }
    oss << ']';
    return std::move(oss).str();
}

} // namespace hybridsearch
