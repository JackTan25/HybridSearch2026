

module;

export module filter_expression_push_down_helper;

import stl;
import base_expression;
import value;
import internal_types;

namespace hybridsearch {

export enum class FilterCompareType : i8 { kEqual, kLess, kLessEqual, kGreater, kGreaterEqual, kAlwaysFalse, kAlwaysTrue, kInvalid };

export class FilterExpressionPushDownHelper {
public:
    static Value CalcValueResult(const SharedPtr<BaseExpression> &expression);

    static Tuple<ColumnID, Value, FilterCompareType>
    UnwindCast(const SharedPtr<BaseExpression> &cast_expr, Value &&right_val, FilterCompareType compare_type);
};

} // namespace hybridsearch