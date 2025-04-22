

#pragma once

#include "base_mixed_type.h"

namespace hybridsearch {

// Not nested data type
struct __attribute__((packed)) IntegerMixedType : public BaseMixedType {
    IntegerMixedType() : BaseMixedType(MixedValueType::kInteger) {}

    int8_t _dummy1{};
    int16_t _dummy2{};
    int32_t _dummy3{};
    int64_t value{0};
};

} // namespace hybridsearch