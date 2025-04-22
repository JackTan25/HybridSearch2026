

#pragma once

#include <cstdint>

namespace hybridsearch {

enum class MixedValueType : int8_t {
    kInvalid,
    // Not nested data type
    kInteger,
    kFloat,
    kLongStr,
    kShortStr,
    kTuple,
    kArray,
    kNull,
    kMissing,

    kDummy,
};

}
