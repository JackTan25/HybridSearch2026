

module;

export module physical_import;

import stl;

import query_context;
import operator_state;
import physical_operator;
import physical_operator_type;
import third_party;
import txn;
import txn_store;
import constant_expr;
import segment_entry;
import block_entry;
import zsv;
import load_meta;
import hybridsearch_exception;
import column_vector;
import internal_types;
import statement_common;
import data_type;
import logger;
import sparse_info;
import meta_info;

namespace hybridsearch {

class ZxvParserCtx {
public:
    ZsvParser parser_;
    SizeT row_count_{};
    SharedPtr<String> err_msg_{};
    TableInfo *const table_info_{};
    Txn *const txn_{};
    SharedPtr<SegmentEntry> segment_entry_{};
    UniquePtr<BlockEntry> block_entry_{};
    Vector<ColumnVector> column_vectors_{};
    const char delimiter_{};

public:
    ZxvParserCtx(TableInfo *table_info,
                 Txn *txn,
                 SharedPtr<SegmentEntry> segment_entry,
                 UniquePtr<BlockEntry> block_entry,
                 Vector<ColumnVector> &&column_vectors,
                 char delimiter)
        : row_count_(0), err_msg_(nullptr), table_info_(table_info), txn_(txn), segment_entry_(segment_entry), block_entry_(std::move(block_entry)),
          column_vectors_(std::move(column_vectors)), delimiter_(delimiter) {}
};

export class PhysicalImport : public PhysicalOperator {
public:
    explicit PhysicalImport(u64 id,
                            const SharedPtr<TableInfo>& table_info,
                            String file_path,
                            bool header,
                            char delimiter,
                            CopyFileType type,
                            SharedPtr<Vector<LoadMeta>> load_metas)
        : PhysicalOperator(PhysicalOperatorType::kImport, nullptr, nullptr, id, load_metas), table_info_(table_info), file_type_(type),
          file_path_(std::move(file_path)), header_(header), delimiter_(delimiter) {}

    ~PhysicalImport() override = default;

    void Init(QueryContext *query_context) override;

    bool Execute(QueryContext *query_context, OperatorState *operator_state) final;

    inline SharedPtr<Vector<String>> GetOutputNames() const final { return output_names_; }

    inline SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final { return output_types_; }

    void ImportFVECS(QueryContext *query_context, ImportOperatorState *import_op_state);

    void ImportCSR(QueryContext *query_context, ImportOperatorState *import_op_state);

    void ImportBVECS(QueryContext *query_context, ImportOperatorState *import_op_state);

    /// for push based execution
    void ImportCSV(QueryContext *query_context, ImportOperatorState *import_op_state);

    /// for push based execution
    void ImportJSON(QueryContext *query_context, ImportOperatorState *import_op_state);

    void ImportJSONL(QueryContext *query_context, ImportOperatorState *import_op_state);

    void ImportPARQUET(QueryContext *query_context, ImportOperatorState *import_op_state);

    inline const TableInfo* table_info() const { return table_info_.get(); }

    inline CopyFileType FileType() const { return file_type_; }

    inline const String &file_path() const { return file_path_; }

    inline bool header() const { return header_; }

    inline char delimiter() const { return delimiter_; }

    static void SaveSegmentData(TableInfo *table_info, Txn *txn, SharedPtr<SegmentEntry> segment_entry);

private:
    static void CSVHeaderHandler(void *);

    static void CSVRowHandler(void *);

    void JSONLRowHandler(const nlohmann::json &line_json, Vector<ColumnVector> &column_vectors);

    void ParquetValueHandler(const SharedPtr<arrow::Array> &array, ColumnVector &column_vector, u64 value_idx);

private:
    SharedPtr<Vector<String>> output_names_{};
    SharedPtr<Vector<SharedPtr<DataType>>> output_types_{};

    SharedPtr<TableInfo> table_info_{};
    CopyFileType file_type_{CopyFileType::kInvalid};
    String file_path_{};
    bool header_{false};
    char delimiter_{','};
};

export SharedPtr<ConstantExpr> BuildConstantExprFromJson(const nlohmann::json &json_object);
export SharedPtr<ConstantExpr> BuildConstantSparseExprFromJson(const nlohmann::json &json_object, const SparseInfo *sparse_info);

} // namespace hybridsearch