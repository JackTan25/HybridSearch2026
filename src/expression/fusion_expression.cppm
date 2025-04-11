

module;

export module fusion_expression;

import stl;
import base_expression;
import match_tensor_expression;
import data_type;
import logical_type;
import internal_types;
import search_options;

namespace hybridsearch {

export class FusionExpression : public BaseExpression {
public:
    FusionExpression(const String &method, SharedPtr<SearchOptions> options);

    [[nodiscard]] inline DataType Type() const override { return DataType(LogicalType::kFloat); }

    String ToString() const override;

public:
    String method_{};
    SharedPtr<SearchOptions> options_{};
    SharedPtr<MatchTensorExpression> match_tensor_expr_;
};

} // namespace hybridsearch