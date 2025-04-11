

#include "type/complex/row_id.h"
#include "spdlog/fmt/fmt.h"

#include <string>

namespace hybridsearch {

std::string RowID::ToString() const { return std::to_string(ToUint64()); }

} // namespace hybridsearch
