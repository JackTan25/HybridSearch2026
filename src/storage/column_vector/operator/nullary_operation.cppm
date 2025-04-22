

module;

#include <concepts>
#include <type_traits>

export module nullary_operation;

import stl;
import column_vector;
import internal_types;

namespace hybridsearch {

export class NullaryOperator {

public:
    template <typename ResultType, typename Operator>
    static void inline Execute(SharedPtr<ColumnVector> &result, void *state_ptr) {
        result->Reset();
        result->Initialize(ColumnVectorType::kConstant);
        auto *result_ptr = (ResultType *)(result->data());
        Operator::template Execute<ResultType>(result_ptr[0], state_ptr);

        result->Finalize(1);
    }
};

} // namespace hybridsearch