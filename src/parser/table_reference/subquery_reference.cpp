

#include "subquery_reference.h"

namespace hybridsearch {

SubqueryReference::~SubqueryReference() {
    if (select_statement_ != nullptr) {
        delete select_statement_;
        select_statement_ = nullptr;
    }
}

std::string SubqueryReference::ToString() { return "SubqueryReference"; }

}; // namespace hybridsearch
