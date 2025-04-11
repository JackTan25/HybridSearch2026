

module;

export module cast_function;

import bound_cast_func;
import data_type;
import internal_types;

namespace hybridsearch {

export class CastFunction {
public:
    static BoundCastFunc GetBoundFunc(const DataType &source, const DataType &target);
};

} // namespace hybridsearch
