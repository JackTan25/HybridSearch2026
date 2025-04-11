

#include "array_mixed_type.h"
#include "mixed_array_value.h"
#include "mixed_type.h"

namespace hybridsearch {

void ArrayMixedType::Reset(bool in_constructor) {
    if (in_constructor) {
        ptr = nullptr;
        count = 0;
        return;
    }

    if (ptr != nullptr) {

        auto *array_value_ptr = (MixedArrayValue *)(this->ptr);
        for (uint16_t i = 0; i < count; ++i) {
            MixedType &slot_ref = array_value_ptr->array[i];
            slot_ref.Reset();
        }

        delete[] ptr;

        ptr = nullptr;
        count = 0;
    }
}

} // namespace hybridsearch
