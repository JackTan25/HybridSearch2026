

module;

export module knn_expression;

import stl;
import base_expression;
import data_type;
import logical_type;
import internal_types;
import knn_expr;
import statement_common;

namespace hybridsearch {

export class KnnExpression : public BaseExpression {
public:
    static String KnnDistanceType2Str(KnnDistanceType type);

public:
    KnnExpression(EmbeddingDataType embedding_data_type,
                  i64 dimension,
                  KnnDistanceType knn_distance_type,
                  EmbeddingT query_embedding,
                  Vector<SharedPtr<BaseExpression>> arguments,
                  i64 topn,
                  Vector<InitParameter *> *opt_params,
                  SharedPtr<BaseExpression> optional_filter,
                  String using_index,
                  bool ignore_index);

    inline DataType Type() const override { return DataType(LogicalType::kFloat); }

    String ToString() const override;

    bool IsKnnMinHeap() const {
        switch (distance_type_) {
            case KnnDistanceType::kL2:
            case KnnDistanceType::kHamming: {
                return false;
            }
            case KnnDistanceType::kCosine:
            case KnnDistanceType::kInnerProduct:
            default: {
                return true;
            }
        }
    }

    u64 Hash() const override;

    bool Eq(const BaseExpression &other) const override;

public:
    const i64 dimension_{0};
    const EmbeddingDataType embedding_data_type_{EmbeddingDataType::kElemInvalid};
    const KnnDistanceType distance_type_{KnnDistanceType::kInvalid};
    EmbeddingT query_embedding_;
    const i64 topn_;
    Vector<InitParameter> opt_params_;
    const String using_index_;
    bool ignore_index_;
    SharedPtr<BaseExpression> optional_filter_;
};

} // namespace hybridsearch