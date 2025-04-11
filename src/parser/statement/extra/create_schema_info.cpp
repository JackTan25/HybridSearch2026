

#include "create_schema_info.h"
#include <sstream>

namespace hybridsearch {

std::string CreateSchemaInfo::ToString() const {
    std::stringstream ss;
    ss << "CREATE SCHEMA: " << schema_name_;
    if (this->conflict_type_ == ConflictType::kIgnore) {
        ss << " IgnoreIfNotExists";
    } else {
        ss << " ThrowErrorIfNotExists";
    }
    return ss.str();
}

} // namespace hybridsearch
