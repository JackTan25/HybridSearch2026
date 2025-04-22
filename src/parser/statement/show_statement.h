

#pragma once

#include "base_statement.h"
#include <optional>

namespace hybridsearch {

enum class ShowStmtType {
    kInvalid,
    kDatabase,
    kTable,
    kIndex,
    kDatabases,
    kColumns,
    kTables,
    kCollections,
    kViews,
    kIndexes,
    kConfigs,
    kProfiles,
    kSegments,
    kSegment,
    kBlocks,
    kBlock,
    kBlockColumn,
    kSessionVariable,
    kSessionVariables,
    kGlobalVariable,
    kGlobalVariables,
    kConfig,
    kBuffer,
    kMemIndex,
    kQueries,
    kQuery,
    kTransactions,
    kTransaction,
    kTransactionHistory,
    kIndexSegment,
    kIndexChunk,
    kLogs,
    kDeltaLogs,
    kCatalogs,
    kPersistenceFiles,
    kPersistenceObjects,
    kPersistenceObject,
    kMemory,
    kMemoryObjects,
    kMemoryAllocation,
    kFunction,
    kListSnapshots,
    kShowSnapshot
};

class ShowStatement : public BaseStatement {
public:
    explicit ShowStatement() : BaseStatement(StatementType::kShow) {}

    [[nodiscard]] std::string ToString() const final;

    ShowStmtType show_type_{ShowStmtType::kTables};
    std::string schema_name_{};
    std::string table_name_{};
    std::optional<std::string> index_name_{};
    std::optional<std::string> file_name_{};
    std::optional<std::string> function_name_{};
    std::optional<int64_t> segment_id_{};
    std::optional<int64_t> block_id_{};
    std::optional<int64_t> chunk_id_{};
    std::optional<u_int64_t> column_id_{};
    std::optional<u_int64_t> session_id_{};
    std::optional<u_int64_t> txn_id_{};
    std::string var_name_{};
    std::optional<std::string> snapshot_name_{};
};

} // namespace hybridsearch
