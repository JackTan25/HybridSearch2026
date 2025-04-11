

module;

#include "simd_common_intrin_include.h"
export module search_top_1;

import stl;
import simd_init;
import search_top_1_sgemm;

namespace hybridsearch {

export template <typename DistType, typename TypeX, typename TypeY, typename ID>
void search_top_1_without_dis(u32 dimension, u32 nx, const TypeX *x, u32 ny, const TypeY *y, ID *labels) {
    if constexpr (std::is_same_v<DistType, TypeX> && std::is_same_v<DistType, TypeY> && std::is_same_v<DistType, f32>) {
#if defined(__AVX2__)
        if (IsAVX2Supported()) {
            return search_top_1_without_dis_avx2(dimension, nx, x, ny, y, labels);
        }
#endif
#if defined(__SSE2__)
        if (IsSSE2Supported()) {
            return search_top_1_without_dis_sse2(dimension, nx, x, ny, y, labels);
        }
#endif
    }
    return search_top_1_simple_without_dis<DistType>(dimension, nx, x, ny, y, labels);
}

} // namespace hybridsearch
