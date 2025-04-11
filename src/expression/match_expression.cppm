

module;

export module match_expression;

import stl;
import data_type;
import base_expression;
import logical_type;
import internal_types;

namespace hybridsearch {

export class MatchExpression : public BaseExpression {
public:
    MatchExpression(const String &fields, const String &matching_text, const String &options_text, const String &index_names);

    inline DataType Type() const override { return DataType(LogicalType::kFloat); }

    String ToString() const override;

    u64 Hash() const override;

    bool Eq(const BaseExpression &other) const override;

public:
    String fields_;
    String matching_text_;
    String options_text_;
    Vector<String> index_names_;
    SharedPtr<BaseExpression> optional_filter_;
};

} // namespace hybridsearch
