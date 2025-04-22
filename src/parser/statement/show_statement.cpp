

#include "show_statement.h"
#include <sstream>

namespace hybridsearch {

std::string ShowStatement::ToString() const {
    std::stringstream ss;
    switch (show_type_) {
        case ShowStmtType::kInvalid: {
            ParserError("Invalid show statement");
            break;
        }
        case ShowStmtType::kDatabase: {
            ss << "Show database, database: " << schema_name_;
            break;
        }
        case ShowStmtType::kTable: {
            ss << "Show table, database: " << schema_name_;
            break;
        }
        case ShowStmtType::kIndex: {
            ss << "Show index, database: " << schema_name_;
            break;
        }
        case ShowStmtType::kBuffer: {
            ss << "Show buffer";
            break;
        }
        case ShowStmtType::kMemIndex: {
            ss << "Show mem index";
            break;
        }
        case ShowStmtType::kIndexSegment: {
            ss << "Show index segment, database: " << schema_name_ << ", table_name: " << table_name_;
            break;
        }
        case ShowStmtType::kIndexChunk: {
            ss << "Show index chunk, database: " << schema_name_ << ", table_name: " << table_name_;
            break;
        }
        case ShowStmtType::kTables: {
            ss << "Show tables, database: " << schema_name_;
            break;
        }
        case ShowStmtType::kCollections: {
            ss << "Show collections, database: " << schema_name_;
            break;
        }
        case ShowStmtType::kViews: {
            ss << "Show views, database: " << schema_name_;
            break;
        }
        case ShowStmtType::kColumns: {
            ss << "Describe table: " << schema_name_ << "." << table_name_;
            break;
        }
        case ShowStmtType::kIndexes: {
            ss << "Describe index of table: " << schema_name_ << ", table: " << table_name_;
            break;
        }
        case ShowStmtType::kDatabases: {
            ss << "Show databases";
            break;
        }
        case ShowStmtType::kSegments: {
            ss << "Show Segments";
            break;
        }
        case ShowStmtType::kSegment: {
            ss << "Show Segment";
            break;
        }
        case ShowStmtType::kBlocks: {
            ss << "Show Blocks";
            break;
        }
        case ShowStmtType::kBlock: {
            ss << "Show Block";
            break;
        }
        case ShowStmtType::kBlockColumn: {
            ss << "Show Block Column";
            break;
        }
        case ShowStmtType::kConfigs: {
            ss << "Show configs";
            break;
        }
        case ShowStmtType::kProfiles: {
            ss << "Show profiles";
            break;
        }
        case ShowStmtType::kQueries: {
            ss << "Show queries";
            break;
        }
        case ShowStmtType::kQuery: {
            ss << "Show query";
            break;
        }
        case ShowStmtType::kTransactions: {
            ss << "Show transactions";
            break;
        }
        case ShowStmtType::kTransaction: {
            ss << "Show transaction";
            break;
        }
        case ShowStmtType::kTransactionHistory: {
            ss << "Show transaction history";
            break;
        }
        case ShowStmtType::kGlobalVariable: {
            ss << "Show global variable";
            break;
        }
        case ShowStmtType::kSessionVariable: {
            ss << "Show session variable";
            break;
        }
        case ShowStmtType::kGlobalVariables: {
            ss << "Show global variables";
            break;
        }
        case ShowStmtType::kSessionVariables: {
            ss << "Show session variables";
            break;
        }
        case ShowStmtType::kConfig: {
            ss << "Show config";
            break;
        }
        case ShowStmtType::kLogs: {
            ss << "Show config";
            break;
        }
        case ShowStmtType::kDeltaLogs: {
            ss << "Show config";
            break;
        }
        case ShowStmtType::kCatalogs: {
            ss << "Show catalogs";
            break;
        }
        case ShowStmtType::kPersistenceFiles: {
            ss << "Show persistence files";
            break;
        }
        case ShowStmtType::kPersistenceObjects: {
            ss << "Show persistence objects";
            break;
        }
        case ShowStmtType::kPersistenceObject: {
            ss << "Show persistence object";
            break;
        }
        case ShowStmtType::kMemory: {
            ss << "Show memory";
            break;
        }
        case ShowStmtType::kMemoryObjects: {
            ss << "Show memory objects";
            break;
        }
        case ShowStmtType::kMemoryAllocation: {
            ss << "Show memory allocation";
            break;
        }
        case ShowStmtType::kFunction: {
            ss << "Show function";
            break;
        }
        case ShowStmtType::kListSnapshots: {
            ss << "List snapshots";
            break;
        }
        case ShowStmtType::kShowSnapshot: {
            ss << "Show snapshot";
            break;
        }
    }
    return ss.str();
}

} // namespace hybridsearch
