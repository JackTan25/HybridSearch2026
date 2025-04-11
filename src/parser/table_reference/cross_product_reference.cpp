

#include "cross_product_reference.h"

namespace hybridsearch {

CrossProductReference::~CrossProductReference() {
    for (auto *table_ptr : tables_) {
        delete (table_ptr);
    }
}

std::string CrossProductReference::ToString() { return "CrossProductReference"; }

}; // namespace hybridsearch
