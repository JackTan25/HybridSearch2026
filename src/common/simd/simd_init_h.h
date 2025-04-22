

#pragma once
#include <vector>

namespace hybridsearch {

const std::vector<char const *> &GetSupportedSimdTypesList();
bool IsF16CSupported();
bool IsSSE2Supported();
bool IsAVX2Supported();
bool IsAVX512Supported();
bool IsAVX512BWSupported();

} // namespace hybridsearch
