

module;

export module reference_expression;

import stl;
import expression_type;
import base_expression;
import data_type;
import internal_types;

namespace hybridsearch {

export class ReferenceExpression : public BaseExpression {
public:
    static inline SharedPtr<ReferenceExpression> Make(DataType data_type, String table_name, String column_name, String alias, SizeT column_idx) {
        return MakeShared<ReferenceExpression>(std::move(data_type), std::move(table_name), std::move(column_name), std::move(alias), column_idx);
    }

public:
    explicit ReferenceExpression(DataType data_type, String table_name, String column_name, String alias, SizeT column_idx)
        : BaseExpression(ExpressionType::kReference, {}, std::move(alias)), data_type_(std::move(data_type)), table_name_(std::move(table_name)),
          column_name_(std::move(column_name)), column_index_(column_idx) {}

    inline SizeT column_index() const { return column_index_; }

    inline DataType Type() const override { return data_type_; };

    String ToString() const override;

    u64 Hash() const override;

    bool Eq(const BaseExpression &other) const override;

private:
    DataType data_type_;

    String table_name_{};

    String column_name_{};

    SizeT column_index_{};
};

} // namespace hybridsearch
