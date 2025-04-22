

#pragma once

#include "base_table_reference.h"

namespace hybridsearch {

enum class JoinType {
    kInner,
    kFull,
    kLeft,
    kRight,
    kCross,
    kNatural,
    kSemi,
    kMark,
    kAnti,
};

class JoinReference : public BaseTableReference {

public:
    explicit JoinReference() : BaseTableReference(TableRefType::kJoin) {}

    ~JoinReference() override;

    static std::string ToString(JoinType type);

    std::string ToString() override;

    BaseTableReference *left_{nullptr};
    BaseTableReference *right_{nullptr};

    ParsedExpr *condition_{nullptr};
    JoinType join_type_{JoinType::kInner};

    std::vector<char *> *using_columns_{nullptr};
};

} // namespace hybridsearch
