

module;

#include "simd_common_intrin_include.h"
export module search_top_k;

import stl;
import simd_init;
import search_top_k_sgemm;

namespace hybridsearch {

export template <typename TypeX, typename TypeY, typename ID, typename DistType>
void search_top_k_with_dis(u32 k, u32 dimension, u32 nx, const TypeX *x, u32 ny, const TypeY *y, ID *labels, DistType *distances, bool sort_) {
    if constexpr (std::is_same_v<DistType, TypeX> && std::is_same_v<DistType, TypeY> && std::is_same_v<DistType, f32>) {
#if defined(__AVX2__)
        if (IsAVX2Supported()) {
            return search_top_k_with_sgemm_avx2(k, dimension, nx, x, ny, y, labels, distances, sort_);
        }
#endif
#if defined(__SSE2__)
        if (IsSSE2Supported()) {
            return search_top_k_with_sgemm_sse2(k, dimension, nx, x, ny, y, labels, distances, sort_);
        }
#endif
    }
    return search_top_k_simple_with_dis(k, dimension, nx, x, ny, y, labels, distances, sort_);
}

} // namespace hybridsearch
