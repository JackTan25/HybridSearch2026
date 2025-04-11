

module;

#include <sstream>

module correlated_column_expression;

import stl;
import expression_type;
import data_type;
import internal_types;

namespace hybridsearch {

CorrelatedColumnExpression::CorrelatedColumnExpression(DataType data_type, String column_name)
    : BaseExpression(ExpressionType::kCorrelatedColumn, {}), data_type_(std::move(data_type)), column_name_(std::move(column_name)) {}

String CorrelatedColumnExpression::ToString() const { return {}; }

} // namespace hybridsearch
