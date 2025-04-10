

#include "base_mixed_type.h"

namespace hybridsearch {

static const char *type2name[] = {
    "Invalid",
    "Integer",
    "Float",
    "Long String",
    "Short String",
    "Tuple",
    "Array",
    "Null",
    "Missing",
    "Dummy",
};

std::string BaseMixedType::GetTypeName(MixedValueType type) noexcept {
    int8_t type_id = static_cast<int8_t>(type);
    return type2name[type_id];
}

} // namespace hybridsearch
