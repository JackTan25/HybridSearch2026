

module;

export module logical_alter;

import stl;
import logical_node_type;
import column_binding;
import logical_node;
import meta_info;
import internal_types;
import data_type;
import alter_statement;
import column_def;

namespace hybridsearch {

export class LogicalAlter : public LogicalNode {
public:
    LogicalAlter(u64 node_id, const SharedPtr<TableInfo> &table_info, AlterStatementType type)
        : LogicalNode(node_id, LogicalNodeType::kAlter), type_(type), table_info_(table_info) {}

    Vector<ColumnBinding> GetColumnBindings() const final;

    SharedPtr<Vector<String>> GetOutputNames() const final;

    SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final;

public:
    AlterStatementType type_;
    SharedPtr<TableInfo> table_info_;
};

export class LogicalRenameTable : public LogicalAlter {
public:
    LogicalRenameTable(u64 node_id, const SharedPtr<TableInfo> &table_info, String new_table_name)
        : LogicalAlter(node_id, table_info, AlterStatementType::kRenameTable), new_table_name_(std::move(new_table_name)) {}

    String ToString(i64 &space) const final;

    String name() final { return "LogicalRenameTable"; }

public:
    String new_table_name_;
};

export class LogicalAddColumns : public LogicalAlter {
public:
    LogicalAddColumns(u64 node_id, const SharedPtr<TableInfo> &table_info, Vector<SharedPtr<ColumnDef>> column_defs)
        : LogicalAlter(node_id, table_info, AlterStatementType::kAddColumns), column_defs_(std::move(column_defs)) {}

    String ToString(i64 &space) const final;

    String name() final { return "LogicalAddColumns"; }

public:
    Vector<SharedPtr<ColumnDef>> column_defs_;
};

export class LogicalDropColumns : public LogicalAlter {
public:
    LogicalDropColumns(u64 node_id, const SharedPtr<TableInfo> &table_info, Vector<String> column_name)
        : LogicalAlter(node_id, table_info, AlterStatementType::kDropColumns), column_names_(std::move(column_name)) {}

    String ToString(i64 &space) const final;

    String name() final { return "LogicalDropColumns"; }

public:
    Vector<String> column_names_;
};

} // namespace hybridsearch