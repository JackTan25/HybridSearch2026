

#pragma once

#include "base_mixed_type.h"

namespace hybridsearch {

struct __attribute__((packed)) ArrayMixedType : public BaseMixedType {

public:
    void Reset(bool in_constructor = false);

    ~ArrayMixedType() { Reset(); }

public:
    ArrayMixedType() : BaseMixedType(MixedValueType::kArray) {}

    int8_t _dummy1{};
    int16_t _dummy2{};
    int16_t _dummy3{};
    uint16_t count{0};  // Array count, which limit is 65535;
    char *ptr{nullptr}; // pointer to child mixed type
};

} // namespace hybridsearch
