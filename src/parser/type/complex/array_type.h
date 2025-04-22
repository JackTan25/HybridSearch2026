

#pragma once

#include <cstdint>

namespace hybridsearch {

#pragma pack(1)

struct ArrayType {
    uint64_t element_num_ : 16 = 0;
    uint64_t file_offset_ : 48 = 0;
};

static_assert(sizeof(ArrayType) == sizeof(uint64_t));
static_assert(sizeof(ArrayType) == 8u);

#pragma pack()

} // namespace hybridsearch
