

#pragma once

#include "base_statement.h"
#include <memory>
#include <vector>

namespace hybridsearch {

struct SegmentEntry;

enum class CompactStatementType { kManual, kAuto, kInvalid };

class CompactStatement : public BaseStatement {
public:
    CompactStatement(CompactStatementType compact_type, const std::string &db_name, const std::string &table_name)
        : BaseStatement(StatementType::kCompact), compact_type_(compact_type), db_name_(db_name), table_name_(table_name) {}

    [[nodiscard]] std::string ToString() const final { return "Compact Table {}.{}"; }

public:
    CompactStatementType compact_type_;
    std::string db_name_{};
    std::string table_name_{};
};

class ManualCompactStatement : public CompactStatement {
public:
    ManualCompactStatement(const std::string &db_name, const std::string &table_name)
        : CompactStatement{CompactStatementType::kManual, db_name, table_name} {}

public:
};

struct AutoCompactStatement : public CompactStatement {
public:
    AutoCompactStatement(const std::string &db_name, const std::string &table_name, std::vector<SegmentEntry *> segments_to_compact)
        : CompactStatement{CompactStatementType::kAuto, db_name, table_name}, segments_to_compact_(std::move(segments_to_compact)) {}

public:
    std::vector<SegmentEntry *> segments_to_compact_;
};

} // namespace hybridsearch