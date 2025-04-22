

#pragma once

#include "expr/constant_expr.h"
#include "parsed_expr.h"
#include "statement/statement_common.h"
#include "type/complex/embedding_type.h"

namespace hybridsearch {

enum class KnnDistanceType {
    kInvalid,
    kL2, // Euclidean distance
    kCosine,
    kInnerProduct,
    kHamming,
};

class KnnExpr : public ParsedExpr {
public:
    explicit KnnExpr(bool own_memory = true) : ParsedExpr(ParsedExprType::kKnn), own_memory_(own_memory) {}

    ~KnnExpr() override;

    [[nodiscard]] std::string ToString() const override;

    bool InitDistanceType(const char *distance_type);

    bool InitEmbedding(const char *data_type, const ConstantExpr *query_vec);

public:
    static std::string KnnDistanceType2Str(KnnDistanceType knn_distance_type);

public:
    const bool own_memory_;

    ParsedExpr *column_expr_{};
    void *embedding_data_ptr_{}; // Pointer to the embedding data ,the data type include float, int ,char ...., so we use void* here
    int64_t dimension_{};
    EmbeddingDataType embedding_data_type_{EmbeddingDataType::kElemInvalid};
    KnnDistanceType distance_type_{KnnDistanceType::kInvalid};
    int64_t topn_{DEFAULT_MATCH_VECTOR_TOP_N};
    std::vector<InitParameter *> *opt_params_{};

    bool ignore_index_{false};
    std::string index_name_;

    std::unique_ptr<ParsedExpr> filter_expr_;
};

} // namespace hybridsearch