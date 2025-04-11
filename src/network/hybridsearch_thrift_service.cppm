

module;

export module hybridsearch_thrift_service;

import hybridsearch_thrift_types;
import hybridsearch;
import stl;
import query_options;
import column_def;
import statement_common;
import data_type;
import status;
import embedding_info;
import constant_expr;
import column_expr;
import function_expr;
import in_expr;
import knn_expr;
import match_sparse_expr;
import match_tensor_expr;
import match_expr;
import fusion_expr;
import parsed_expr;
import update_statement;
import search_expr;
import explain_statement;
import create_index_info;
import data_block;
import table_def;
import internal_types;
import column_vector;
import query_result;
import select_statement;
import global_resource_usage;

namespace hybridsearch {

struct ClientVersions {
    ClientVersions();

    HashMap<i64, String> client_version_map_;

    Pair<const char *, Status> GetVersionByIndex(i64);
};

export class hybridsearchThriftService final : public hybridsearch_thrift_rpc::hybridsearchServiceIf {
private:
    static constexpr std::string_view ErrorMsgHeader = "[THRIFT ERROR]";
    static constexpr i64 current_version_index_{29}; // 0.6.0.dev3

    static std::mutex hybridsearch_session_map_mutex_;
    static HashMap<u64, SharedPtr<hybridsearch>> hybridsearch_session_map_;

    static ClientVersions client_version_;

public:
    static u32 ClearSessionMap();

    hybridsearchThriftService() {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::IncrObjectCount("hybridsearchThriftService");
#endif
    }

    virtual ~hybridsearchThriftService() {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::DecrObjectCount("hybridsearchThriftService");
#endif
    }

    void Connect(hybridsearch_thrift_rpc::CommonResponse &response, const hybridsearch_thrift_rpc::ConnectRequest &request) final;

    void Disconnect(hybridsearch_thrift_rpc::CommonResponse &response, const hybridsearch_thrift_rpc::CommonRequest &request) final;

    void CreateDatabase(hybridsearch_thrift_rpc::CommonResponse &response, const hybridsearch_thrift_rpc::CreateDatabaseRequest &request) final;

    void DropDatabase(hybridsearch_thrift_rpc::CommonResponse &response, const hybridsearch_thrift_rpc::DropDatabaseRequest &request) final;

    void CreateTable(hybridsearch_thrift_rpc::CommonResponse &response, const hybridsearch_thrift_rpc::CreateTableRequest &request) final;

    void DropTable(hybridsearch_thrift_rpc::CommonResponse &response, const hybridsearch_thrift_rpc::DropTableRequest &request) final;

    void Insert(hybridsearch_thrift_rpc::CommonResponse &response, const hybridsearch_thrift_rpc::InsertRequest &request) final;

    Tuple<CopyFileType, Status> GetCopyFileType(hybridsearch_thrift_rpc::CopyFileType::type copy_file_type);

    void Import(hybridsearch_thrift_rpc::CommonResponse &response, const hybridsearch_thrift_rpc::ImportRequest &request) final;

    void Export(hybridsearch_thrift_rpc::CommonResponse &response, const hybridsearch_thrift_rpc::ExportRequest &request) final;

    void Select(hybridsearch_thrift_rpc::SelectResponse &response, const hybridsearch_thrift_rpc::SelectRequest &request) final;

    void Explain(hybridsearch_thrift_rpc::SelectResponse &response, const hybridsearch_thrift_rpc::ExplainRequest &request) final;

    void Delete(hybridsearch_thrift_rpc::DeleteResponse &response, const hybridsearch_thrift_rpc::DeleteRequest &request) final;

    void Update(hybridsearch_thrift_rpc::CommonResponse &response, const hybridsearch_thrift_rpc::UpdateRequest &request) final;

    void Optimize(hybridsearch_thrift_rpc::CommonResponse &response, const hybridsearch_thrift_rpc::OptimizeRequest &request) final;

    void AddColumns(hybridsearch_thrift_rpc::CommonResponse &response, const hybridsearch_thrift_rpc::AddColumnsRequest &request) final;

    void DropColumns(hybridsearch_thrift_rpc::CommonResponse &response, const hybridsearch_thrift_rpc::DropColumnsRequest &request) final;

    void Cleanup(hybridsearch_thrift_rpc::CommonResponse &response, const hybridsearch_thrift_rpc::CommonRequest &request) final;

    void ListDatabase(hybridsearch_thrift_rpc::ListDatabaseResponse &response, const hybridsearch_thrift_rpc::ListDatabaseRequest &request) final;

    void ListTable(hybridsearch_thrift_rpc::ListTableResponse &response, const hybridsearch_thrift_rpc::ListTableRequest &request) final;

    void ShowDatabase(hybridsearch_thrift_rpc::ShowDatabaseResponse &response, const hybridsearch_thrift_rpc::ShowDatabaseRequest &request) final;

    void ShowTable(hybridsearch_thrift_rpc::ShowTableResponse &response, const hybridsearch_thrift_rpc::ShowTableRequest &request) final;

    void ShowColumns(hybridsearch_thrift_rpc::SelectResponse &response, const hybridsearch_thrift_rpc::ShowColumnsRequest &request) final;

    void ShowTables(hybridsearch_thrift_rpc::SelectResponse &response, const hybridsearch_thrift_rpc::ShowTablesRequest &request) final;

    void ShowSegments(hybridsearch_thrift_rpc::SelectResponse &_return, const hybridsearch_thrift_rpc::ShowSegmentsRequest &request) final;

    void ShowSegment(hybridsearch_thrift_rpc::ShowSegmentResponse &_return, const hybridsearch_thrift_rpc::ShowSegmentRequest &request) final;

    void ShowBlocks(hybridsearch_thrift_rpc::SelectResponse &_return, const hybridsearch_thrift_rpc::ShowBlocksRequest &request) final;

    void ShowBlock(hybridsearch_thrift_rpc::ShowBlockResponse &_return, const hybridsearch_thrift_rpc::ShowBlockRequest &request) final;

    void ShowBlockColumn(hybridsearch_thrift_rpc::ShowBlockColumnResponse &_return, const hybridsearch_thrift_rpc::ShowBlockColumnRequest &request) final;

    void GetDatabase(hybridsearch_thrift_rpc::CommonResponse &response, const hybridsearch_thrift_rpc::GetDatabaseRequest &request) final;

    void GetTable(hybridsearch_thrift_rpc::CommonResponse &response, const hybridsearch_thrift_rpc::GetTableRequest &request) final;

    void CreateIndex(hybridsearch_thrift_rpc::CommonResponse &response, const hybridsearch_thrift_rpc::CreateIndexRequest &request) final;

    void DropIndex(hybridsearch_thrift_rpc::CommonResponse &response, const hybridsearch_thrift_rpc::DropIndexRequest &request) final;

    void ListIndex(hybridsearch_thrift_rpc::ListIndexResponse &response, const hybridsearch_thrift_rpc::ListIndexRequest &request) final;

    void ShowIndex(hybridsearch_thrift_rpc::ShowIndexResponse &response, const hybridsearch_thrift_rpc::ShowIndexRequest &request) final;

    void ShowCurrentNode(hybridsearch_thrift_rpc::ShowCurrentNodeResponse &response, const hybridsearch_thrift_rpc::ShowCurrentNodeRequest &request) final;

    void Command(hybridsearch_thrift_rpc::CommonResponse &response, const hybridsearch_thrift_rpc::CommandRequest &request) final;

    void Flush(hybridsearch_thrift_rpc::CommonResponse &response, const hybridsearch_thrift_rpc::FlushRequest &request) final;

    void Compact(hybridsearch_thrift_rpc::CommonResponse &response, const hybridsearch_thrift_rpc::CompactRequest &request) final;

    template <typename T>
    static void
    HandleArrayTypeRecursively(String &output_str, const DataType &data_type, const T &data_value, const SharedPtr<ColumnVector> &column_vector);

private:
    Tuple<hybridsearch *, Status> GethybridsearchBySessionID(i64 session_id);

    Status GetAndRemoveSessionID(i64 session_id);

    static Tuple<ColumnDef *, Status> GetColumnDefFromProto(const hybridsearch_thrift_rpc::ColumnDef &column_def);

    static SharedPtr<DataType> GetColumnTypeFromProto(const hybridsearch_thrift_rpc::DataType &type);

    static ConstraintType GetConstraintTypeFromProto(hybridsearch_thrift_rpc::Constraint::type constraint);

    static EmbeddingDataType GetEmbeddingDataTypeFromProto(const hybridsearch_thrift_rpc::ElementType::type &type);

    static IndexType GetIndexTypeFromProto(const hybridsearch_thrift_rpc::IndexType::type &type);

    static ConstantExpr *GetConstantFromProto(Status &status, const hybridsearch_thrift_rpc::ConstantExpr &expr);

    static ColumnExpr *GetColumnExprFromProto(const hybridsearch_thrift_rpc::ColumnExpr &column_expr);

    static FunctionExpr *GetFunctionExprFromProto(Status &status, const hybridsearch_thrift_rpc::FunctionExpr &function_expr);

    static KnnExpr *GetKnnExprFromProto(Status &status, const hybridsearch_thrift_rpc::KnnExpr &expr);

    static MatchSparseExpr *GetMatchSparseExprFromProto(Status &status, const hybridsearch_thrift_rpc::MatchSparseExpr &expr);

    static MatchTensorExpr *GetMatchTensorExprFromProto(Status &status, const hybridsearch_thrift_rpc::MatchTensorExpr &expr);

    static MatchExpr *GetMatchExprFromProto(Status &status, const hybridsearch_thrift_rpc::MatchExpr &expr);

    static ParsedExpr *GetGenericMatchExprFromProto(Status &status, const hybridsearch_thrift_rpc::GenericMatchExpr &expr);

    static FusionExpr *GetFusionExprFromProto(const hybridsearch_thrift_rpc::FusionExpr &expr);

    static InExpr *GetInExprFromProto(Status &status, const hybridsearch_thrift_rpc::InExpr &in_expr);

    static ParsedExpr *GetParsedExprFromProto(Status &status, const hybridsearch_thrift_rpc::ParsedExpr &expr);

    static OrderByExpr *GetOrderByExprFromProto(Status &status, const hybridsearch_thrift_rpc::OrderByExpr &expr);

    static KnnDistanceType GetDistanceTypeFormProto(const hybridsearch_thrift_rpc::KnnDistanceType::type &type);

    static ExplainType GetExplainTypeFromProto(const hybridsearch_thrift_rpc::ExplainType::type &type);

    static Tuple<void *, i64, Status> GetEmbeddingDataTypeDataPtrFromProto(const hybridsearch_thrift_rpc::EmbeddingData &embedding_data);

    static Tuple<UpdateExpr *, Status> GetUpdateExprFromProto(const hybridsearch_thrift_rpc::UpdateExpr &update_expr);

    static OptimizeOptions GetParsedOptimizeOptionFromProto(const hybridsearch_thrift_rpc::OptimizeOptions &options);

    static hybridsearch_thrift_rpc::ColumnType::type DataTypeToProtoColumnType(const SharedPtr<DataType> &data_type);

    UniquePtr<hybridsearch_thrift_rpc::DataType> DataTypeToProtoDataType(const SharedPtr<DataType> &data_type);

    hybridsearch_thrift_rpc::ElementType::type EmbeddingDataTypeToProtoElementType(const EmbeddingDataType &embedding_data_type);

    void
    ProcessDataBlocks(const QueryResult &result, hybridsearch_thrift_rpc::SelectResponse &response, Vector<hybridsearch_thrift_rpc::ColumnField> &columns);

    Status ProcessColumns(const SharedPtr<DataBlock> &data_block, SizeT column_count, Vector<hybridsearch_thrift_rpc::ColumnField> &columns);

    void HandleColumnDef(hybridsearch_thrift_rpc::SelectResponse &response,
                         SizeT column_count,
                         SharedPtr<TableDef> table_def,
                         Vector<hybridsearch_thrift_rpc::ColumnField> &all_column_vectors);

    Status
    ProcessColumnFieldType(hybridsearch_thrift_rpc::ColumnField &output_column_field, SizeT row_count, const SharedPtr<ColumnVector> &column_vector);

    static void HandleBoolType(hybridsearch_thrift_rpc::ColumnField &output_column_field, SizeT row_count, const SharedPtr<ColumnVector> &column_vector);

    static void HandlePodType(hybridsearch_thrift_rpc::ColumnField &output_column_field, SizeT row_count, const SharedPtr<ColumnVector> &column_vector);

    static void HandleArrayType(hybridsearch_thrift_rpc::ColumnField &output_column_field, SizeT row_count, const SharedPtr<ColumnVector> &column_vector);

    static void
    HandleVarcharType(hybridsearch_thrift_rpc::ColumnField &output_column_field, SizeT row_count, const SharedPtr<ColumnVector> &column_vector);

    static void
    HandleEmbeddingType(hybridsearch_thrift_rpc::ColumnField &output_column_field, SizeT row_count, const SharedPtr<ColumnVector> &column_vector);

    static void
    HandleMultiVectorType(hybridsearch_thrift_rpc::ColumnField &output_column_field, SizeT row_count, const SharedPtr<ColumnVector> &column_vector);

    static void
    HandleTensorType(hybridsearch_thrift_rpc::ColumnField &output_column_field, SizeT row_count, const SharedPtr<ColumnVector> &column_vector);

    static void
    HandleTensorArrayType(hybridsearch_thrift_rpc::ColumnField &output_column_field, SizeT row_count, const SharedPtr<ColumnVector> &column_vector);

    static void
    HandleSparseType(hybridsearch_thrift_rpc::ColumnField &output_column_field, SizeT row_count, const SharedPtr<ColumnVector> &column_vector);

    static void HandleRowIDType(hybridsearch_thrift_rpc::ColumnField &output_column_field, SizeT row_count, const SharedPtr<ColumnVector> &column_vector);

    static void
    HandleTimeRelatedTypes(hybridsearch_thrift_rpc::ColumnField &output_column_field, SizeT row_count, const SharedPtr<ColumnVector> &column_vector);

    static void
    ProcessStatus(hybridsearch_thrift_rpc::CommonResponse &response, const Status &status, const std::string_view error_header = ErrorMsgHeader);

    static void
    ProcessStatus(hybridsearch_thrift_rpc::DeleteResponse &response, const Status &status, const std::string_view error_header = ErrorMsgHeader);

    static void
    ProcessStatus(hybridsearch_thrift_rpc::ShowDatabaseResponse &response, const Status &status, const std::string_view error_header = ErrorMsgHeader);

    static void
    ProcessStatus(hybridsearch_thrift_rpc::ShowTableResponse &response, const Status &status, const std::string_view error_header = ErrorMsgHeader);

    static void
    ProcessStatus(hybridsearch_thrift_rpc::ShowIndexResponse &response, const Status &status, const std::string_view error_header = ErrorMsgHeader);

    static void
    ProcessStatus(hybridsearch_thrift_rpc::SelectResponse &response, const Status &status, const std::string_view error_header = ErrorMsgHeader);

    static void
    ProcessStatus(hybridsearch_thrift_rpc::ListDatabaseResponse &response, const Status &status, const std::string_view error_header = ErrorMsgHeader);

    static void
    ProcessStatus(hybridsearch_thrift_rpc::ListTableResponse &response, const Status &status, const std::string_view error_header = ErrorMsgHeader);

    static void
    ProcessStatus(hybridsearch_thrift_rpc::ListIndexResponse &response, const Status &status, const std::string_view error_header = ErrorMsgHeader);

    static void
    ProcessStatus(hybridsearch_thrift_rpc::ShowSegmentResponse &response, const Status &status, const std::string_view error_header = ErrorMsgHeader);

    static void
    ProcessStatus(hybridsearch_thrift_rpc::ShowBlockResponse &response, const Status &status, const std::string_view error_header = ErrorMsgHeader);

    static void
    ProcessStatus(hybridsearch_thrift_rpc::ShowBlockColumnResponse &response, const Status &status, const std::string_view error_header = ErrorMsgHeader);

    static void
    ProcessStatus(hybridsearch_thrift_rpc::ShowCurrentNodeResponse &response, const Status &status, const std::string_view error_header = ErrorMsgHeader);

    static void ProcessQueryResult(hybridsearch_thrift_rpc::CommonResponse &response,
                                   const QueryResult &result,
                                   const std::string_view error_header = ErrorMsgHeader);

    static void ProcessQueryResult(hybridsearch_thrift_rpc::SelectResponse &response,
                                   const QueryResult &result,
                                   const std::string_view error_header = ErrorMsgHeader);

    static void ProcessQueryResult(hybridsearch_thrift_rpc::DeleteResponse &response,
                                   const QueryResult &result,
                                   const std::string_view error_header = ErrorMsgHeader);

    static void ProcessQueryResult(hybridsearch_thrift_rpc::ListDatabaseResponse &response,
                                   const QueryResult &result,
                                   const std::string_view error_header = ErrorMsgHeader);

    static void ProcessQueryResult(hybridsearch_thrift_rpc::ListTableResponse &response,
                                   const QueryResult &result,
                                   const std::string_view error_header = ErrorMsgHeader);

    static void ProcessQueryResult(hybridsearch_thrift_rpc::ListIndexResponse &response,
                                   const QueryResult &result,
                                   const std::string_view error_header = ErrorMsgHeader);

    static void ProcessQueryResult(hybridsearch_thrift_rpc::ShowDatabaseResponse &response,
                                   const QueryResult &result,
                                   const std::string_view error_header = ErrorMsgHeader);

    static void ProcessQueryResult(hybridsearch_thrift_rpc::ShowTableResponse &response,
                                   const QueryResult &result,
                                   const std::string_view error_header = ErrorMsgHeader);

    static void ProcessQueryResult(hybridsearch_thrift_rpc::ShowIndexResponse &response,
                                   const QueryResult &result,
                                   const std::string_view error_header = ErrorMsgHeader);

    static void ProcessQueryResult(hybridsearch_thrift_rpc::ShowSegmentResponse &response,
                                   const QueryResult &result,
                                   const std::string_view error_header = ErrorMsgHeader);

    static void ProcessQueryResult(hybridsearch_thrift_rpc::ShowBlockResponse &response,
                                   const QueryResult &result,
                                   const std::string_view error_header = ErrorMsgHeader);

    static void ProcessQueryResult(hybridsearch_thrift_rpc::ShowBlockColumnResponse &response,
                                   const QueryResult &result,
                                   const std::string_view error_header = ErrorMsgHeader);

    static void ProcessQueryResult(hybridsearch_thrift_rpc::ShowCurrentNodeResponse &response,
                                   const QueryResult &result,
                                   const std::string_view error_header = ErrorMsgHeader);
};

} // namespace hybridsearch
