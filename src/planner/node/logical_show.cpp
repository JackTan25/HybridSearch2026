

module;

#include <sstream>

module logical_show;

import stl;
import internal_types;
import hybridsearch_exception;
import column_binding;
import logger;

namespace hybridsearch {

String ToString(ShowStmtType type) {
    switch (type) {
        case ShowStmtType::kDatabase:
            return "Show database";
        case ShowStmtType::kTable:
            return "Show table";
        case ShowStmtType::kIndex:
            return "Show index";
        case ShowStmtType::kIndexSegment:
            return "Show index segment";
        case ShowStmtType::kIndexChunk:
            return "Show index chunk";
        case ShowStmtType::kTables:
            return "Show tables";
        case ShowStmtType::kViews:
            return "Show views";
        case ShowStmtType::kColumns:
            return "Show column";
        case ShowStmtType::kIndexes:
            return "Show indexes";
        case ShowStmtType::kDatabases:
            return "Show databases";
        case ShowStmtType::kConfigs:
            return "Show configs";
        case ShowStmtType::kProfiles:
            return "Show profiles";
        case ShowStmtType::kSegments:
            return "Show segments";
        case ShowStmtType::kSegment:
            return "Show segment";
        case ShowStmtType::kBlocks:
            return "Show blocks";
        case ShowStmtType::kBlock:
            return "Show block";
        case ShowStmtType::kBlockColumn:
            return "Show block column";
        case ShowStmtType::kSessionVariable:
            return "Show session variable";
        case ShowStmtType::kSessionVariables:
            return "Show session variables";
        case ShowStmtType::kGlobalVariable:
            return "Show global variable";
        case ShowStmtType::kGlobalVariables:
            return "Show global variables";
        case ShowStmtType::kConfig:
            return "Show config";
        case ShowStmtType::kBuffer:
            return "Show buffer";
        case ShowStmtType::kMemIndex:
            return "Sow memindex";
        case ShowStmtType::kQueries:
            return "Show queries";
        case ShowStmtType::kQuery:
            return "Show query";
        case ShowStmtType::kTransactions:
            return "Show transactions";
        case ShowStmtType::kTransaction:
            return "Show transaction";
        case ShowStmtType::kTransactionHistory:
            return "Show transaction history";
        case ShowStmtType::kLogs:
            return "Show logs";
        case ShowStmtType::kDeltaLogs:
            return "Show delta logs";
        case ShowStmtType::kCatalogs:
            return "Show catalogs";
        case ShowStmtType::kPersistenceFiles:
            return "Show persistence files";
        case ShowStmtType::kPersistenceObjects:
            return "Show persistence objects";
        case ShowStmtType::kPersistenceObject:
            return "Show persistence object";
        case ShowStmtType::kMemory:
            return "Show memory";
        case ShowStmtType::kMemoryObjects:
            return "Show memory objects";
        case ShowStmtType::kMemoryAllocation:
            return "Show memory allocation";
        case ShowStmtType::kFunction:
            return "Show function";
        case ShowStmtType::kCollections:
            return "Show collection";
        case ShowStmtType::kListSnapshots:
            return "List snapshots";
        case ShowStmtType::kShowSnapshot:
            return "Show snapshot";
        case ShowStmtType::kInvalid: {
            String error_message = "Invalid chunk scan type";
            UnrecoverableError(error_message);
        }
    }
    return {};
}

Vector<ColumnBinding> LogicalShow::GetColumnBindings() const { return {}; }

SharedPtr<Vector<String>> LogicalShow::GetOutputNames() const { return MakeShared<Vector<String>>(); }

SharedPtr<Vector<SharedPtr<DataType>>> LogicalShow::GetOutputTypes() const { return MakeShared<Vector<SharedPtr<DataType>>>(); }

String LogicalShow::ToString(i64 &space) const {
    std::stringstream ss;
    String arrow_str;
    if (space > 3) {
        space -= 4;
        arrow_str = "->  ";
    }
    ss << String(space, ' ') << "-> "
       << "Show: ";
    space += arrow_str.size();

    return ss.str();
}

} // namespace hybridsearch
