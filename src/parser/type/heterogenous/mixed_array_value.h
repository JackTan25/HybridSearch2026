

#pragma once

#include "base_mixed_type.h"
#include "mixed_type.h"

namespace hybridsearch {

struct MixedArrayValue {
public:
    MixedType array[0];
    // private:
    //     static char*
    //     Make(int32_t count);
    //
    //     static void
    //     Set(int32_t index, const MixedType& mixed_value);
    //
    //     static MixedType&
    //     Get(int32_t index);
};

} // namespace hybridsearch