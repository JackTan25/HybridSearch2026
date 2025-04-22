

#pragma once

#include <string>

namespace hybridsearch {

enum class ParsedExprType {
    kConstant,
    kParameter,
    kColumn,
    kFunction,
    kBetween,
    kOperator,
    kSubquery,
    kCase,
    kCast,
    kIn,
    kKnn,
    kMatch,
    kMatchTensor,
    kMatchSparse,
    kFusion,
    kSearch,
    kInsertRow,
};

class ParsedExpr {
public:
    explicit ParsedExpr(ParsedExprType type) : type_(type) {}

    virtual ~ParsedExpr() = default;

    [[nodiscard]] inline std::string GetName() const {
        if (HasAlias())
            return GetAlias();
        return ToString();
    }

    inline std::string GetAlias() const { return alias_; }

    [[nodiscard]] inline bool HasAlias() const { return !alias_.empty(); }

    [[nodiscard]] virtual std::string ToString() const = 0;

    ParsedExprType type_;
    std::string alias_{};
};

} // namespace hybridsearch
