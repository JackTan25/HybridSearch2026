

module;

export module subquery_expression;

import base_expression;
import bound_select_statement;

import stl;
import column_expression;
import internal_types;
import subquery_expr;
import data_type;

namespace hybridsearch {

export class SubqueryExpression : public BaseExpression {
public:
    explicit SubqueryExpression(UniquePtr<BoundSelectStatement> select_node, SubqueryType subquery_type);

    String ToString() const override;

    DataType Type() const override;

    UniquePtr<BoundSelectStatement> bound_select_statement_ptr_{};
    SubqueryType subquery_type_;

    // Used in IN subquery, EXISTS won't use it.
    SharedPtr<BaseExpression> left_{nullptr};

    // Correlated column expressions;
    Vector<SharedPtr<ColumnExpression>> correlated_columns{};

private:
};

} // namespace hybridsearch
