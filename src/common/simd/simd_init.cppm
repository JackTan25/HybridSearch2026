

module;

#include "simd_init_h.h"
export module simd_init;
import stl;

namespace hybridsearch {

export using hybridsearch::GetSupportedSimdTypesList;
export using hybridsearch::IsF16CSupported;
export using hybridsearch::IsSSE2Supported;
export using hybridsearch::IsAVX2Supported;
export using hybridsearch::IsAVX512Supported;
export using hybridsearch::IsAVX512BWSupported;

export using F32DistanceFuncType = f32 (*)(const f32 *, const f32 *, SizeT);
export using I8DistanceFuncType = i32 (*)(const i8 *, const i8 *, SizeT);
export using I8CosDistanceFuncType = f32 (*)(const i8 *, const i8 *, SizeT);
export using U8DistanceFuncType = i32 (*)(const u8 *, const u8 *, SizeT);
// dimension in hamming distance is in bytes
export using U8HammingDistanceFuncType = f32 (*)(const u8 *, const u8 *, SizeT);
export using U8CosDistanceFuncType = f32 (*)(const u8 *, const u8 *, SizeT);
export using MaxSimF32BitIPFuncType = f32 (*)(const f32 *, const u8 *, SizeT);
export using MaxSimI32BitIPFuncType = i32 (*)(const i32 *, const u8 *, SizeT);
export using MaxSimI64BitIPFuncType = i64 (*)(const i64 *, const u8 *, SizeT);
export using FilterScoresOutputIdsFuncType = u32 * (*)(u32 *, f32, const f32 *, u32);
export using SearchTop1WithDisF32U32FuncType = void (*)(u32, u32, const f32 *, u32, const f32 *, u32 *, f32 *);
export using BatchBM25FuncType = void (*)(u32, u32, const f32 *, const f32 *, const f32 *, const u32 *, const u32 *, u32 *, f32 *);

// F32 distance functions
export F32DistanceFuncType GetL2DistanceFuncPtr();
export F32DistanceFuncType GetIPDistanceFuncPtr();
export F32DistanceFuncType GetCosineDistanceFuncPtr();

// u32 distance functions
export U8HammingDistanceFuncType GetHammingDistanceFuncPtr();

// HNSW F32
export F32DistanceFuncType Get_HNSW_F32L2_ptr();
export F32DistanceFuncType Get_HNSW_F32L2_16_ptr();
export F32DistanceFuncType Get_HNSW_F32IP_ptr();
export F32DistanceFuncType Get_HNSW_F32IP_16_ptr();
export F32DistanceFuncType Get_HNSW_F32Cos_ptr();
export F32DistanceFuncType Get_HNSW_F32Cos_16_ptr();
// HNSW I8
export I8DistanceFuncType Get_HNSW_I8IP_ptr();
export I8DistanceFuncType Get_HNSW_I8IP_16_ptr();
export I8DistanceFuncType Get_HNSW_I8IP_32_ptr();
export I8DistanceFuncType Get_HNSW_I8IP_64_ptr();
export I8DistanceFuncType Get_HNSW_I8L2_ptr();
export I8DistanceFuncType Get_HNSW_I8L2_16_ptr();
export I8DistanceFuncType Get_HNSW_I8L2_32_ptr();
export I8DistanceFuncType Get_HNSW_I8L2_64_ptr();
export I8CosDistanceFuncType Get_HNSW_I8Cos_ptr();
// HNSW U8
export U8DistanceFuncType Get_HNSW_U8L2_ptr();
export U8DistanceFuncType Get_HNSW_U8L2_16_ptr();
export U8DistanceFuncType Get_HNSW_U8L2_32_ptr();
export U8DistanceFuncType Get_HNSW_U8L2_64_ptr();
export U8DistanceFuncType Get_HNSW_U8IP_ptr();
export U8DistanceFuncType Get_HNSW_U8IP_16_ptr();
export U8DistanceFuncType Get_HNSW_U8IP_32_ptr();
export U8DistanceFuncType Get_HNSW_U8IP_64_ptr();
export U8CosDistanceFuncType Get_HNSW_U8Cos_ptr();
// MaxSim IP
export MaxSimF32BitIPFuncType GetMaxSimF32BitIPFuncPtr();
export MaxSimI32BitIPFuncType GetMaxSimI32BitIPFuncPtr();
export MaxSimI64BitIPFuncType GetMaxSimI64BitIPFuncPtr();
// EMVB
export FilterScoresOutputIdsFuncType GetFilterScoresOutputIdsFuncPtr();
// K-means
export SearchTop1WithDisF32U32FuncType GetSearchTop1WithDisF32U32FuncPtr();
// Batch BM25
export BatchBM25FuncType GetBatchBM25FuncPtr();

} // namespace hybridsearch
