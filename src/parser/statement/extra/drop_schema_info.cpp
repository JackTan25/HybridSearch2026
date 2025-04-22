

#include "drop_schema_info.h"
#include <sstream>

namespace hybridsearch {

std::string DropSchemaInfo::ToString() const {
    std::stringstream ss;
    ss << "DROP SCHEMA: " << schema_name_;
    if (this->conflict_type_ == ConflictType::kIgnore) {
        ss << " IgnoreIfExists";
    } else {
        ss << " ThrowErrorIfExists";
    }
    return ss.str();
}

} // namespace hybridsearch
