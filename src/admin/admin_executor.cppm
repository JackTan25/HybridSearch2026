

module;
export module admin_executor;

import admin_statement;
import query_context;
import query_result;
import stl;
import wal_entry;
import catalog;
import status;

namespace hybridsearch {

export class AdminExecutor {
public:
    static QueryResult Execute(QueryContext *query_context, const AdminStatement *admin_statement);

private:
    static QueryResult ListLogFiles(QueryContext *query_context, const AdminStatement *admin_statement);
    static QueryResult ShowLogFile(QueryContext *query_context, const AdminStatement *admin_statement);
    static QueryResult ListLogIndexes(QueryContext *query_context, const AdminStatement *admin_statement);
    static QueryResult ShowLogIndex(QueryContext *query_context, const AdminStatement *admin_statement);
    static QueryResult ListCatalogs(QueryContext *query_context, const AdminStatement *admin_statement);
    static QueryResult ShowCatalog(QueryContext *query_context, const AdminStatement *admin_statement);
    static QueryResult ListDatabases(QueryContext *query_context, const AdminStatement *admin_statement);
    static QueryResult ShowDatabase(QueryContext *query_context, const AdminStatement *admin_statement);
    static QueryResult ListTables(QueryContext *query_context, const AdminStatement *admin_statement);
    static QueryResult ShowTable(QueryContext *query_context, const AdminStatement *admin_statement);
    static QueryResult ListSegments(QueryContext *query_context, const AdminStatement *admin_statement);
    static QueryResult ShowSegment(QueryContext *query_context, const AdminStatement *admin_statement);
    static QueryResult ListBlocks(QueryContext *query_context, const AdminStatement *admin_statement);
    static QueryResult ShowBlock(QueryContext *query_context, const AdminStatement *admin_statement);
    static QueryResult ListColumns(QueryContext *query_context, const AdminStatement *admin_statement);
    static QueryResult ShowColumn(QueryContext *query_context, const AdminStatement *admin_statement);
    static QueryResult ListIndexes(QueryContext *query_context, const AdminStatement *admin_statement);
    static QueryResult ShowIndex(QueryContext *query_context, const AdminStatement *admin_statement);
    static QueryResult ListIndexSegments(QueryContext *query_context, const AdminStatement *admin_statement);
    static QueryResult ShowIndexSegment(QueryContext *query_context, const AdminStatement *admin_statement);
    static QueryResult ListConfigs(QueryContext *query_context, const AdminStatement *admin_statement);
    static QueryResult ListVariables(QueryContext *query_context, const AdminStatement *admin_statement);
    static QueryResult ShowVariable(QueryContext *query_context, const AdminStatement *admin_statement);
    static QueryResult ListNodes(QueryContext *query_context, const AdminStatement *admin_statement);
    static QueryResult ShowNode(QueryContext *query_context, const AdminStatement *admin_statement);
    static QueryResult RemoveNode(QueryContext *query_context, const AdminStatement *admin_statement);
    static QueryResult ShowCurrentNode(QueryContext *query_context, const AdminStatement *admin_statement);
    static QueryResult SetRole(QueryContext *query_context, const AdminStatement *admin_statement);
    static QueryResult CreateSnapshot(QueryContext *query_context, const AdminStatement *admin_statement);
    static QueryResult ListSnapshots(QueryContext *query_context, const AdminStatement *admin_statement);
    static QueryResult ShowSnapshot(QueryContext *query_context, const AdminStatement *admin_statement);
    static QueryResult DeleteSnapshot(QueryContext *query_context, const AdminStatement *admin_statement);
    static QueryResult ExportSnapshot(QueryContext *query_context, const AdminStatement *admin_statement);
    static QueryResult RecoverFromSnapshot(QueryContext *query_context, const AdminStatement *admin_statement);

    static Vector<SharedPtr<WalEntry>> GetAllCheckpointEntries(QueryContext *query_context, const AdminStatement *admin_statement);
    static Vector<String> GetAllCheckpointFiles(QueryContext *query_context, const AdminStatement *admin_statement);
    static Tuple<UniquePtr<Catalog>, Status>
    LoadCatalogFiles(QueryContext *query_context, const AdminStatement *admin_statement, Vector<SharedPtr<WalEntry>> &ckp_entries);
};

} // namespace hybridsearch
