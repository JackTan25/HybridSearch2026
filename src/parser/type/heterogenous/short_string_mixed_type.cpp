

#include "short_string_mixed_type.h"

namespace hybridsearch {

bool ShortStrMixedType::Equal(const std::string &input) {
    if (length != (int8_t)input.length())
        return false;
    for (size_t i = 0; const auto &c : input) {
        if (this->ptr[i++] != c)
            return false;
    }
    return true;
}

} // namespace hybridsearch