

module;

import stl;

export module bound_cast_func;

namespace hybridsearch {

class ColumnVector;

export struct CastParameters {
    bool strict{false};
};

using cast_function_t = bool (*)(const SharedPtr<ColumnVector> &source, SharedPtr<ColumnVector> &result, SizeT count, CastParameters &parameters);

export struct BoundCastFunc {
    explicit inline BoundCastFunc(cast_function_t func) : function(func) {};

    cast_function_t function = nullptr;
};

} // namespace hybridsearch
