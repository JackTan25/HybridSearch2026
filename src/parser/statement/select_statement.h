

#pragma once

#include "base_statement.h"
#include "expr/column_expr.h"
#include "expr/constant_expr.h"
#include "table_reference/base_table_reference.h"

namespace hybridsearch {

class SelectStatement;

enum class SetOperatorType { kUnion, kUnionAll, kIntersect, kExcept };

struct WithExpr {
    ~WithExpr() {
        if (select_ != nullptr) {
            delete select_;
        }
    }
    std::string alias_{};
    BaseStatement *select_{};
};

enum OrderType { kAsc, kDesc };

struct OrderByExpr {
    ~OrderByExpr() { delete expr_; }
    ParsedExpr *expr_{};
    OrderType type_{OrderType::kAsc};
};

class SelectStatement final : public BaseStatement {
public:
    SelectStatement() : BaseStatement(StatementType::kSelect) {}

    ~SelectStatement() final;

    static std::string ToString(OrderType type);
    [[nodiscard]] std::string ToString() const final;

    BaseTableReference *table_ref_{nullptr};
    std::vector<ParsedExpr *> *select_list_{nullptr};
    std::vector<ParsedExpr *> *highlight_list_{nullptr};
    bool select_distinct_{false};
    ParsedExpr *search_expr_{nullptr};
    ParsedExpr *where_expr_{nullptr};
    std::vector<ParsedExpr *> *group_by_list_{nullptr};
    ParsedExpr *having_expr_{nullptr};
    std::vector<OrderByExpr *> *order_by_list_{nullptr};
    ParsedExpr *limit_expr_{nullptr};
    ParsedExpr *offset_expr_{nullptr};
    std::vector<WithExpr *> *with_exprs_{nullptr};

    SetOperatorType set_op_{SetOperatorType::kUnion};
    SelectStatement *nested_select_{nullptr};

    bool total_hits_count_flag_{false};
};

} // namespace hybridsearch
