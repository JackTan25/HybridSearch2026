

module;

#include "simd_common_intrin_include.h"

module simd_common_tools;

import stl;
import hybridsearch_exception;

namespace hybridsearch {

struct U8MaskArray {
    alignas(64) u32 masks[256][8] = {};
    U8MaskArray() {
        for (u32 i = 0; i < 256; ++i) {
            for (u32 j = 0; j < 8; ++j) {
                masks[i][j] = (i & (1u << j)) ? std::numeric_limits<u32>::max() : 0;
            }
        }
    }
};

U8MaskPtr GetU8MasksForAVX2() {
    static U8MaskArray u8_mask_array;
    return u8_mask_array.masks;
}

} // namespace hybridsearch
