

#include "create_view_info.h"

namespace hybridsearch {

CreateViewInfo::~CreateViewInfo() {
    if (select_ != nullptr) {
        delete select_;
        select_ = nullptr;
    }
    if (view_columns_ != nullptr) {
        delete view_columns_;
        view_columns_ = nullptr;
    }
}

std::string CreateViewInfo::ToString() const { return "Create view info"; }

} // namespace hybridsearch
