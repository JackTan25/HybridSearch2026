

#pragma once

#include "base_statement.h"
#include "statement/extra/create_collection_info.h"
#include "statement/extra/create_index_info.h"
#include "statement/extra/create_schema_info.h"
#include "statement/extra/create_table_info.h"
#include "statement/extra/create_view_info.h"

#include <string>

namespace hybridsearch {

enum class FlushType {
    kDelta,
    kData,
    kLog,
    kBuffer,
};

class FlushStatement : public BaseStatement {
public:
    explicit FlushStatement() : BaseStatement(StatementType::kFlush) {}

    [[nodiscard]] std::string ToString() const final;

    inline FlushType type() const { return type_; }

    FlushType type_;
};

} // namespace hybridsearch