

module;

#include "simd_common_intrin_include.h"
export module distance_simd_functions;
import stl;

namespace hybridsearch {

export f32 L2Distance_common(const f32 *x, const f32 *y, SizeT d);

export f32 IPDistance_common(const f32 *x, const f32 *y, SizeT d);

export f32 CosineDistance_common(const f32 *x, const f32 *y, SizeT d);

export f32 HammingDistance_common(const u8 *x, const u8 *y, SizeT d);

#if defined(__AVX2__)
export f32 L2Distance_avx2(const f32 *vector1, const f32 *vector2, SizeT dimension);

export f32 IPDistance_avx2(const f32 *vector1, const f32 *vector2, SizeT dimension);

export f32 CosineDistance_avx2(const f32 *vector1, const f32 *vector2, SizeT dimension);

export f32 HammingDistance_avx2(const u8 *vector1, const u8 *vector2, SizeT dimension);
#endif

#if defined(__SSE2__)
export f32 HammingDistance_sse2(const u8 *vector1, const u8 *vector2, SizeT dimesion);
#endif

} // namespace hybridsearch
