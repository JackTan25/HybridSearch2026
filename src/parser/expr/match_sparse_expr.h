

#pragma once

#include "constant_expr.h"
#include "parsed_expr.h"
#include "statement/statement_common.h"

namespace hybridsearch {

enum class SparseMetricType : uint8_t {
    kInnerProduct,
    kInvalid,
};

class MatchSparseExpr final : public ParsedExpr {
public:
    explicit MatchSparseExpr(bool own_memory = true) : ParsedExpr(ParsedExprType::kMatchSparse), own_memory_(own_memory) {}

    void SetSearchColumn(ParsedExpr *&column_expr);

    void SetQuerySparse(ConstantExpr *&raw_sparse_expr);

    void SetMetricType(char *&raw_metric_type);

    void SetMetricType(const std::string &metric_type);

    void SetOptParams(size_t topn, std::vector<InitParameter *> *&opt_params);

    void SetOptionalFilter(ParsedExpr *&filter_expr);

    static std::string MetricTypeToString(SparseMetricType metric);

    [[nodiscard]] std::string ToString() const override;

private:
    void SetMetricTypeInner(std::string_view metric_type);

public:
    const bool own_memory_;
    std::unique_ptr<ParsedExpr> column_expr_;

    std::unique_ptr<ConstantExpr> query_sparse_expr_{};
    uint32_t nnz_ = 0;

    SparseMetricType metric_type_;
    size_t query_n_ = 1; // TODO: support multiple queries
    size_t topn_{DEFAULT_MATCH_SPARSE_TOP_N};

    std::vector<std::unique_ptr<InitParameter>> opt_params_{};

    bool ignore_index_{false};
    std::string index_name_;

    std::unique_ptr<ParsedExpr> filter_expr_;
};

} // namespace hybridsearch