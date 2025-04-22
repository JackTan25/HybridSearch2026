

#include "array_info.h"
#include "type/complex/array_type.h"

namespace hybridsearch {

bool ArrayInfo::operator==(const TypeInfo &other) const {
    if (other.type() != TypeInfoType::kArray) {
        return false;
    }
    auto *array_info_ptr = dynamic_cast<const ArrayInfo *>(&other);
    return this->elem_type_ == array_info_ptr->elem_type_;
}

size_t ArrayInfo::Size() const { return sizeof(ArrayType); }

std::string ArrayInfo::ToString() const { return elem_type_.ToString(); }

nlohmann::json ArrayInfo::Serialize() const { return elem_type_.Serialize(); }

} // namespace hybridsearch
