

module;

export module expression_evaluator;

import stl;
import base_expression;
import aggregate_expression;
import case_expression;
import cast_expression;
import column_expression;
import function_expression;
import reference_expression;
import value_expression;
import in_expression;
import filter_fulltext_expression;
import data_block;
import column_vector;
import expression_state;

namespace hybridsearch {

export class ExpressionEvaluator {
public:
    void Init(const DataBlock *input_data_block);

    void Execute(const SharedPtr<BaseExpression> &expr, SharedPtr<ExpressionState> &state, SharedPtr<ColumnVector> &output_column_vector);

    void Execute(const SharedPtr<AggregateExpression> &expr, SharedPtr<ExpressionState> &state, SharedPtr<ColumnVector> &output_column_vector);

    void Execute(const SharedPtr<CastExpression> &expr, SharedPtr<ExpressionState> &state, SharedPtr<ColumnVector> &output_column_vector);

    void Execute(const SharedPtr<CaseExpression> &expr, SharedPtr<ExpressionState> &state, SharedPtr<ColumnVector> &output_column_vector);

    void Execute(const SharedPtr<ColumnExpression> &expr, SharedPtr<ExpressionState> &state, SharedPtr<ColumnVector> &output_column_vector);

    void Execute(const SharedPtr<FunctionExpression> &expr, SharedPtr<ExpressionState> &state, SharedPtr<ColumnVector> &output_column_vector);

    void Execute(const SharedPtr<ValueExpression> &expr, SharedPtr<ExpressionState> &state, SharedPtr<ColumnVector> &output_column_vector);

    void Execute(const SharedPtr<ReferenceExpression> &expr, SharedPtr<ExpressionState> &state, SharedPtr<ColumnVector> &output_column_vector);

    void Execute(const SharedPtr<InExpression> &expr, SharedPtr<ExpressionState> &state, SharedPtr<ColumnVector> &output_column_vector);

    void Execute(const SharedPtr<FilterFulltextExpression> &expr, SharedPtr<ExpressionState> &state, SharedPtr<ColumnVector> &output_column_vector);

private:
    const DataBlock *input_data_block_{};
    bool in_aggregate_{false};
};

} // namespace hybridsearch
