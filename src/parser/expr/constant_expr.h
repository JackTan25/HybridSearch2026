

#pragma once

#include "json.hpp"
#include "parsed_expr.h"
#include "type/data_type.h"
#include "type/datetime/interval_type.h"

#include <cstdint>
#include <memory>
#include <vector>

namespace hybridsearch {

class ColumnDef;

enum class LiteralType : int32_t {
    kBoolean,
    kDouble,
    kString,
    kInteger,
    kNull,
    kDate,
    kTime,
    kDateTime,
    kTimestamp,
    kIntegerArray,
    kDoubleArray,
    kSubArrayArray,
    kInterval,
    kLongSparseArray,
    kDoubleSparseArray,
    kEmptyArray,
    kCurlyBracketsArray,
};

class ConstantExpr : public ParsedExpr {
public:
    explicit ConstantExpr(LiteralType literal_type) : ParsedExpr(ParsedExprType::kConstant), literal_type_(literal_type) {}

    ConstantExpr(ConstantExpr &&constant_expr) noexcept;

    ~ConstantExpr() override;

    [[nodiscard]] std::string ToString() const override;

    int32_t GetSizeInBytes() const;

    void WriteAdv(char *&ptr) const;

    static std::shared_ptr<ParsedExpr> ReadAdv(const char *&ptr, int32_t maxbytes);

    nlohmann::json Serialize() const;

    static std::shared_ptr<ParsedExpr> Deserialize(const nlohmann::json &constant_expr);

    void TrySortSparseVec(const ColumnDef *col_def);

public:
    LiteralType literal_type_;

    bool bool_value_{false};
    int64_t integer_value_{0};
    double double_value_{0};
    char *str_value_{nullptr};
    TimeUnit interval_type_{TimeUnit::kInvalidUnit};
    char *date_value_{nullptr};
    std::vector<int64_t> long_array_{};
    std::vector<double> double_array_{};
    std::vector<std::shared_ptr<ConstantExpr>> sub_array_array_{};
    std::pair<std::vector<int64_t>, std::vector<int64_t>> long_sparse_array_{};
    std::pair<std::vector<int64_t>, std::vector<double>> double_sparse_array_{};
    std::vector<std::shared_ptr<ConstantExpr>> curly_brackets_array_{};
};

} // namespace hybridsearch
