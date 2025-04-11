

module;

export module column_expression;

import column_binding;
import base_expression;
import data_type;
import special_function;
import stl;
import internal_types;

namespace hybridsearch {

export class ColumnExpression final : public BaseExpression {
public:
    static inline SharedPtr<ColumnExpression> Make(DataType data_type,
                                                   String table_name,
                                                   u64 table_index,
                                                   String column_name,
                                                   i64 column_index,
                                                   i64 depth,
                                                   Optional<SpecialType> special = None) {
        return MakeShared<ColumnExpression>(std::move(data_type),
                                            std::move(table_name),
                                            table_index,
                                            std::move(column_name),
                                            column_index,
                                            depth,
                                            special);
    }

public:
    ColumnExpression(DataType data_type,
                     String table_name,
                     u64 table_index,
                     String column_name,
                     i64 column_index,
                     i64 depth,
                     Optional<SpecialType> special = None);

    inline DataType Type() const override { return data_type_; };

    const String &table_name() const { return table_name_; }

    //    inline u64
    //    table_index() const {
    //        return binding_.table_idx;
    //    }

    const String &column_name() const { return column_name_; }

    inline void SetBinding(SizeT table_idx, SizeT column_idx) {
        binding_.table_idx = table_idx;
        binding_.column_idx = column_idx;
    }

    inline ColumnBinding binding() const { return binding_; }

    inline bool IsCorrelated() const { return depth_ > 0; }

    inline void SetDepth(i64 depth) { depth_ = depth; }

    inline i64 depth() const { return depth_; }

    inline Optional<SpecialType> special() const { return special_; }

    u64 Hash() const override;

    bool Eq(const BaseExpression &other) const override;

    String ToString() const final;

private:
    DataType data_type_;

    ColumnBinding binding_{};

    String table_name_{};

    String column_name_{};

    i64 depth_{};

    Optional<SpecialType> special_{None};
};

} // namespace hybridsearch