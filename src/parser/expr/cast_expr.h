

#pragma once

#include "parsed_expr.h"
#include "type/data_type.h"
#include "type/logical_type.h"
#include "type/type_info.h"
#include <memory>

namespace hybridsearch {

class CastExpr final : public ParsedExpr {
public:
    explicit CastExpr(DataType data_type) : ParsedExpr(ParsedExprType::kCast), data_type_(std::move(data_type)) {}

    ~CastExpr() final;

    [[nodiscard]] std::string ToString() const override;

public:
    DataType data_type_;
    ParsedExpr *expr_{nullptr};
};

} // namespace hybridsearch
