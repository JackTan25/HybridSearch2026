

#include "simd_init_h.h"
#include "NGT_CpuInfo_SimdType.h"

namespace hybridsearch {

struct SupportedSimdTypes {
    std::vector<char const *> types_strs_ = NGT::CpuInfo::getSupportedSimdTypes();
    bool is_f16c_ = NGT::CpuInfo::isF16C();
    bool is_sse2_ = NGT::CpuInfo::isSSE2();
    bool is_avx2_ = NGT::CpuInfo::isAVX2();
    bool is_avx512_ = NGT::CpuInfo::isAVX512();
    bool is_avx512bw_ = NGT::CpuInfo::isAVX512BW();
};

const SupportedSimdTypes &GetSupportedSimdTypes() {
    static SupportedSimdTypes supported_simd_types;
    return supported_simd_types;
}

const std::vector<char const *> &GetSupportedSimdTypesList() { return GetSupportedSimdTypes().types_strs_; }

bool IsF16CSupported() { return GetSupportedSimdTypes().is_f16c_; }

bool IsSSE2Supported() { return GetSupportedSimdTypes().is_sse2_; }

bool IsAVX2Supported() { return GetSupportedSimdTypes().is_avx2_; }

bool IsAVX512Supported() { return GetSupportedSimdTypes().is_avx512_; }

bool IsAVX512BWSupported() { return GetSupportedSimdTypes().is_avx512bw_; }

} // namespace hybridsearch
