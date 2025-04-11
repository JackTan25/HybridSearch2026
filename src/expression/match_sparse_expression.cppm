

module;

export module match_sparse_expression;

import stl;
import base_expression;
import data_type;
import expression_type;
import internal_types;
import statement_common;
import match_sparse_expr;
import sparse_info;
import column_expression;
import constant_expr;
import base_expression;

namespace hybridsearch {

export class MatchSparseExpression final : public BaseExpression {
public:
    MatchSparseExpression(Vector<SharedPtr<BaseExpression>> search_column,
                          SharedPtr<BaseExpression> query_sparse_expr,
                          SparseMetricType metric_type,
                          SizeT query_n,
                          SizeT topn,
                          Vector<UniquePtr<InitParameter>> opt_params,
                          SharedPtr<BaseExpression> optional_filter,
                          String index_name,
                          bool ignore_index);

    DataType Type() const override;

    String ToString() const override;

    u64 Hash() const override;

    bool Eq(const BaseExpression &other) const override;

private:
    void MakeQuery(SharedPtr<BaseExpression> query_sparse_expr);

public:
    const ColumnExpression *column_expr_;
    SharedPtr<BaseExpression> query_sparse_expr_;

    SparseMetricType metric_type_;
    SizeT query_n_;
    SizeT topn_;

    Vector<UniquePtr<InitParameter>> opt_params_;

    SharedPtr<BaseExpression> optional_filter_;

    const String index_name_;
    bool ignore_index_;
};

} // namespace hybridsearch
