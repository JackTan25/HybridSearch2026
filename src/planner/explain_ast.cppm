

module;

import stl;
import base_statement;
import create_statement;
import insert_statement;
import copy_statement;
import drop_statement;
import select_statement;
import show_statement;
import flush_statement;
import optimize_statement;
import base_table_reference;
import status;

export module explain_ast;

namespace hybridsearch {

export class ExplainAST {
public:
    static Status Explain(const BaseStatement *statement, SharedPtr<Vector<SharedPtr<String>>> &stmt_string, i64 intent_size = 0);

    static Status BuildCreate(const CreateStatement *statement, SharedPtr<Vector<SharedPtr<String>>> &stmt_string, i64 intent_size);

    static Status BuildInsert(const InsertStatement *statement, SharedPtr<Vector<SharedPtr<String>>> &stmt_string, i64 intent_size);

    static Status BuildCopy(const CopyStatement *statement, SharedPtr<Vector<SharedPtr<String>>> &stmt_string, i64 intent_size);

    static Status BuildDrop(const DropStatement *statement, SharedPtr<Vector<SharedPtr<String>>> &stmt_string, i64 intent_size);

    static Status BuildSelect(const SelectStatement *statement,
                              SharedPtr<Vector<SharedPtr<String>>> &stmt_string,
                              i64 intent_size,
                              SharedPtr<String> alias_ptr = nullptr);

    static Status BuildBaseTableRef(const BaseTableReference *table_ref, SharedPtr<Vector<SharedPtr<String>>> &stmt_string, i64 intent_size);

    static Status BuildShow(const ShowStatement *statement, SharedPtr<Vector<SharedPtr<String>>> &stmt_string, i64 intent_size);

    static Status BuildFlush(const FlushStatement *flush_statement, SharedPtr<Vector<SharedPtr<String>>> &result, i64 intent_size);

    static Status BuildOptimize(const OptimizeStatement *optimize_statement, SharedPtr<Vector<SharedPtr<String>>> &result, i64);
};

} // namespace hybridsearch