

#include "drop_collection_info.h"
#include <sstream>

namespace hybridsearch {

std::string DropCollectionInfo::ToString() const {
    std::stringstream ss;
    ss << "DROP COLLECTION: " << schema_name_ << "." << collection_name_;
    if (this->conflict_type_ == ConflictType::kIgnore) {
        ss << " IgnoreIfExists";
    } else {
        ss << " ThrowErrorIfExists";
    }
    return ss.str();
}

} // namespace hybridsearch
