

module;

export module match_tensor_expression;

import stl;
import data_type;
import logical_type;
import knn_expr;
import internal_types;
import base_expression;
import column_expression;
import match_tensor_expr;

namespace hybridsearch {

export class MatchTensorExpression final : public BaseExpression {
public:
    MatchTensorExpression(Vector<SharedPtr<BaseExpression>> search_column,
                          MatchTensorSearchMethod search_method,
                          EmbeddingDataType embedding_data_type,
                          u32 dimension,
                          EmbeddingT query_embedding,
                          u32 tensor_basic_embedding_dimension,
                          const String &options_text,
                          SharedPtr<BaseExpression> optional_filterm,
                          String index_name,
                          bool ignore_index);

    DataType Type() const override;

    String ToString() const override;

    static String MethodToString(MatchTensorSearchMethod method);

    u64 Hash() const override;

    bool Eq(const BaseExpression &other) const override;

    const MatchTensorSearchMethod search_method_;
    const ColumnExpression *column_expr_ = nullptr;
    const EmbeddingDataType embedding_data_type_;
    const u32 dimension_;                        // num of total elements in the tensor (num of embedding * dimension of single embedding)
    EmbeddingT query_embedding_;                 // treat the query tensor as an embedding here
    const u32 tensor_basic_embedding_dimension_; // dimension of single embedding in the tensor column
    const u32 num_of_embedding_in_query_tensor_ = dimension_ / tensor_basic_embedding_dimension_;
    const String options_text_;
    SharedPtr<BaseExpression> optional_filter_;
    const String index_name_;
    bool ignore_index_;
};

} // namespace hybridsearch
