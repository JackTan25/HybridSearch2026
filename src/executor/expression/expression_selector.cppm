

module;

import stl;
import base_expression;
import expression_state;
import data_block;
import selection;

export module expression_selector;

namespace hybridsearch {
class ColumnVector;

export class ExpressionSelector {
public:
    SizeT Select(const SharedPtr<BaseExpression> &expr,
                 SharedPtr<ExpressionState> &state,
                 const DataBlock *input_data_block,
                 DataBlock *output_data_block,
                 SizeT count);

    void Select(const SharedPtr<BaseExpression> &expr,
                SharedPtr<ExpressionState> &state,
                SizeT count,
                const SharedPtr<Selection> &input_select,
                SharedPtr<Selection> &output_true_select,
                SharedPtr<Selection> &output_false_select);

    void Select(const SharedPtr<BaseExpression> &expr, SharedPtr<ExpressionState> &state, SizeT count, SharedPtr<Selection> &output_true_select);

    static void Select(const SharedPtr<ColumnVector> &bool_column, SizeT count, SharedPtr<Selection> &output_true_select, bool nullable);

private:
    const DataBlock *input_data_{nullptr};
};

} // namespace hybridsearch
