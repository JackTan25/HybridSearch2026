

#include "create_collection_info.h"
#include <sstream>

namespace hybridsearch {

std::string CreateCollectionInfo::ToString() const {
    std::stringstream ss;
    ss << "CREATE COLLECTION: " << schema_name_ << "." << collection_name_;
    if (this->conflict_type_ == ConflictType::kIgnore) {
        ss << " IgnoreIfNotExists";
    } else {
        ss << " ThrowErrorIfNotExists";
    }
    return ss.str();
}

} // namespace hybridsearch
