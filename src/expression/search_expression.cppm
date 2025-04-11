

module;

export module search_expression;

import stl;
import base_expression;
import fusion_expression;
import match_expression;
import knn_expression;
import match_tensor_expression;
import match_sparse_expression;
import logical_type;
import internal_types;
import data_type;

namespace hybridsearch {

export class SearchExpression final : public BaseExpression {
public:
    SearchExpression(Vector<SharedPtr<BaseExpression>> &match_exprs, Vector<SharedPtr<FusionExpression>> &fusion_exprs);

    inline DataType Type() const override { return DataType(LogicalType::kFloat); }

    String ToString() const override;

public:
    // Eash match_expr shall be one of MatchExpression, KnnExpression, MatchTensorExpression, MatchSparseExpression
    Vector<SharedPtr<BaseExpression>> match_exprs_{};
    Vector<SharedPtr<FusionExpression>> fusion_exprs_{};
    bool have_filter_in_subsearch_{false};
};

} // namespace hybridsearch