

module;

#include <sstream>

module unnest_expression;

import logical_type;
import internal_types;
import third_party;
import hybridsearch_exception;
import array_info;
import column_expression;
import expression_type;

namespace hybridsearch {

UnnestExpression::UnnestExpression(SharedPtr<BaseExpression> column_expression) : BaseExpression(ExpressionType::kUnnest, {column_expression}) {}

DataType UnnestExpression::Type() const {
    const auto &expr = arguments_[0];
    const DataType &column_type = expr->Type();
    if (column_type.type() != LogicalType::kArray) {
        UnrecoverableError("Unnest expression must be applied to an array column.");
    }
    auto *array_info = static_cast<const ArrayInfo *>(column_type.type_info().get());
    return array_info->ElemType();
}

String UnnestExpression::ToString() const {
    const auto &expr = arguments_[0];
    std::stringstream ss;
    ss << "Unnest(" << expr->ToString() << ")";
    return std::move(ss).str();
}

} // namespace hybridsearch