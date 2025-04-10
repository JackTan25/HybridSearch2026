

module;

export module physical_alter;

import stl;

import query_context;
import operator_state;
import physical_operator;
import physical_operator_type;
import load_meta;
import hybridsearch_exception;
import internal_types;
import data_type;
import logger;
import meta_info;
import alter_statement;
import column_def;
import constant_expr;

namespace hybridsearch {

export class PhysicalAlter : public PhysicalOperator {
public:
    explicit PhysicalAlter(const SharedPtr<TableInfo>& table_info,
                           AlterStatementType type,
                           SharedPtr<Vector<String>> output_names,
                           SharedPtr<Vector<SharedPtr<DataType>>> output_types,
                           u64 id,
                           SharedPtr<Vector<LoadMeta>> load_metas)
        : PhysicalOperator(PhysicalOperatorType::kAlter, nullptr, nullptr, id, load_metas), table_info_(table_info),
          output_names_(std::move(output_names)), output_types_(std::move(output_types)) {}

    ~PhysicalAlter() override = default;

    inline SharedPtr<Vector<String>> GetOutputNames() const final { return output_names_; }

    inline SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final { return output_types_; }

protected:
    AlterStatementType type_;
    SharedPtr<TableInfo> table_info_{};
    SharedPtr<Vector<String>> output_names_{};
    SharedPtr<Vector<SharedPtr<DataType>>> output_types_{};
};

export class PhysicalRenameTable final : public PhysicalAlter {
public:
    PhysicalRenameTable(const SharedPtr<TableInfo>& table_info,
                        String &&new_table_name,
                        SharedPtr<Vector<String>> output_names,
                        SharedPtr<Vector<SharedPtr<DataType>>> output_types,
                        u64 id,
                        SharedPtr<Vector<LoadMeta>> load_metas)
        : PhysicalAlter(table_info, AlterStatementType::kRenameTable, std::move(output_names), std::move(output_types), id, load_metas),
          new_table_name_(std::move(new_table_name)) {}

    void Init(QueryContext* query_context) override;

    bool Execute(QueryContext *query_context, OperatorState *operator_state) override;

private:
    String new_table_name_;
};

export class PhysicalAddColumns final : public PhysicalAlter {
public:
    PhysicalAddColumns(const SharedPtr<TableInfo>& table_info,
                       const Vector<SharedPtr<ColumnDef>> &column_defs,
                       SharedPtr<Vector<String>> output_names,
                       SharedPtr<Vector<SharedPtr<DataType>>> output_types,
                       u64 id,
                       SharedPtr<Vector<LoadMeta>> load_metas)
        : PhysicalAlter(table_info, AlterStatementType::kAddColumns, std::move(output_names), std::move(output_types), id, load_metas),
          column_defs_(column_defs) {}

    void Init(QueryContext* query_context) override;

    bool Execute(QueryContext *query_context, OperatorState *operator_state) override;

private:
    const Vector<SharedPtr<ColumnDef>> &column_defs_;
};

export class PhysicalDropColumns final : public PhysicalAlter {
public:
    PhysicalDropColumns(const SharedPtr<TableInfo>& table_info,
                        const Vector<String> &column_names,
                        SharedPtr<Vector<String>> output_names,
                        SharedPtr<Vector<SharedPtr<DataType>>> output_types,
                        u64 id,
                        SharedPtr<Vector<LoadMeta>> load_metas)
        : PhysicalAlter(table_info, AlterStatementType::kDropColumns, std::move(output_names), std::move(output_types), id, load_metas),
          column_names_(column_names) {}

    void Init(QueryContext* query_context) override;

    bool Execute(QueryContext *query_context, OperatorState *operator_state) override;

private:
    const Vector<String> &column_names_;
};

} // namespace hybridsearch
