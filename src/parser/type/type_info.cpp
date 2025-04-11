

#include "type_info.h"

namespace hybridsearch {

bool TypeInfo::operator!=(const TypeInfo &other) const { return !operator==(other); }

} // namespace hybridsearch
