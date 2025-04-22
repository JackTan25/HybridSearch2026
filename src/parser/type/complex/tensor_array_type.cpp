

#include "tensor_array_type.h"
#include "tensor_type.h"
#include <sstream>

namespace hybridsearch {
std::string TensorArrayType::TensorArray2String(const std::vector<std::pair<std::span<const char>, uint64_t>> &raw_data,
                                                EmbeddingDataType type,
                                                size_t embedding_dimension) {
    std::ostringstream oss;
    oss << '[';
    size_t tensor_num = raw_data.size();
    for (size_t tensor_id = 0; tensor_id < tensor_num; ++tensor_id) {
        const auto &[data, embedding_num] = raw_data[tensor_id];
        oss << TensorType::Tensor2String(data.data(), type, embedding_dimension, embedding_num);
        if (tensor_id != tensor_num - 1) {
            oss << ',';
        }
    }
    oss << ']';
    return std::move(oss).str();
}
} // namespace hybridsearch