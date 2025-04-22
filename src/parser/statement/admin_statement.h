

#pragma once

#include "base_statement.h"

namespace hybridsearch {

enum class AdminStmtType {
    kListLogFiles,
    kShowLogFile,
    kListLogIndexes,
    kShowLogIndex,
    kListCatalogs,
    kShowCatalog,
    kListDatabases,
    kShowDatabase,
    kListTables,
    kShowTable,
    kListSegments,
    kShowSegment,
    kListBlocks,
    kShowBlock,
    kListColumns,
    kShowColumn,
    kListIndexes,
    kShowIndex,
    kListIndexSegments,
    kShowIndexSegment,
    kListConfigs,
    kListVariables,
    kShowVariable,
    kListNodes,
    kShowNode,
    kShowCurrentNode,
    kListSnapshots,
    kShowSnapshot,
    kRemoveNode,
    kSetRole,
    kCreateSnapshot,
    kDeleteSnapshot,
    kExportSnapshot,
    kRecoverFromSnapshot,
    kInvalid,
};

enum class NodeRole { kAdmin, kStandalone, kLeader, kFollower, kLearner, kUnInitialized };

std::string ToString(NodeRole);

class AdminStatement : public BaseStatement {
public:
    explicit AdminStatement() : BaseStatement(StatementType::kAdmin) {}

    [[nodiscard]] std::string ToString() const final;

    AdminStmtType admin_type_{AdminStmtType::kInvalid};
    std::optional<int64_t> catalog_file_index_{};
    std::optional<int64_t> catalog_file_start_index_{};
    std::optional<int64_t> catalog_file_end_index_{};
    std::optional<int64_t> database_meta_index_{};
    std::optional<int64_t> database_entry_index_{};
    std::optional<int64_t> table_meta_index_{};
    std::optional<int64_t> table_entry_index_{};
    std::optional<int64_t> segment_index_{};
    std::optional<int64_t> block_index_{};
    std::optional<int64_t> column_index_{};
    std::optional<int64_t> index_meta_index_{};
    std::optional<int64_t> index_entry_index_{};
    std::optional<int64_t> log_file_index_{};
    std::optional<int64_t> log_index_in_file_{};
    std::optional<NodeRole> node_role_{};
    std::optional<std::string> leader_address_{};
    std::optional<std::string> variable_name_{};
    std::optional<std::string> node_name_{};
    std::optional<std::string> snapshot_name_{};
    std::optional<std::string> export_path_{};
};

}
// namespace hybridsearch
