

module;

export module physical_create_table;

import stl;

import query_context;
import operator_state;
import physical_operator;
import physical_operator_type;
import index_base;
import table_def;
import load_meta;
import hybridsearch_exception;
import internal_types;
import extra_ddl_info;
import data_type;
import logger;

namespace hybridsearch {

export class PhysicalCreateTable final : public PhysicalOperator {
public:
    explicit PhysicalCreateTable(SharedPtr<String> schema_name,
                                 SharedPtr<TableDef> table_def_ptr,
                                 SharedPtr<Vector<String>> output_names,
                                 SharedPtr<Vector<SharedPtr<DataType>>> output_types,
                                 ConflictType conflict_type,
                                 u64 table_index,
                                 u64 id,
                                 SharedPtr<Vector<LoadMeta>> load_metas);

    explicit PhysicalCreateTable(SharedPtr<String> schema_name,
                                 UniquePtr<PhysicalOperator> input,
                                 SharedPtr<Vector<String>> output_names,
                                 SharedPtr<Vector<SharedPtr<DataType>>> output_types,
                                 ConflictType conflict_type,
                                 u64 table_index,
                                 u64 id,
                                 SharedPtr<Vector<LoadMeta>> load_metas);

    ~PhysicalCreateTable() override = default;

    void Init(QueryContext* query_context) override;

    bool Execute(QueryContext *query_context, OperatorState *operator_state) final;

    inline SharedPtr<Vector<String>> GetOutputNames() const final { return output_names_; }

    inline SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final { return output_types_; }

    inline u64 table_index() const { return table_index_; }

    inline const SharedPtr<TableDef> &table_definition() const { return table_def_ptr_; }

    inline ConflictType conflict_type() const { return conflict_type_; }

    inline const SharedPtr<String> &schema_name() const { return schema_name_; }

private:
    SharedPtr<TableDef> table_def_ptr_{};
    SharedPtr<String> schema_name_{};
    u64 table_index_{};
    ConflictType conflict_type_{ConflictType::kInvalid};

    SharedPtr<Vector<String>> output_names_{};
    SharedPtr<Vector<SharedPtr<DataType>>> output_types_{};
};

} // namespace hybridsearch
