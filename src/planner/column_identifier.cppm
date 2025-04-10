

module;

import stl;

import query_context;
import column_expr;

export module column_identifer;

namespace hybridsearch {

export struct ColumnIdentifier {
public:
    static ColumnIdentifier MakeColumnIdentifier(QueryContext *query_context, const ColumnExpr &expr);

public:
    explicit ColumnIdentifier(SharedPtr<String> db_name,
                              SharedPtr<String> schema_name,
                              SharedPtr<String> table_name,
                              SharedPtr<String> column_name,
                              SharedPtr<String> alias_name);

    [[nodiscard]] String ToString() const;

    [[nodiscard]] bool operator==(const ColumnIdentifier &other) const;

    SharedPtr<String> db_name_ptr_;
    SharedPtr<String> schema_name_ptr_;
    SharedPtr<String> column_name_ptr_;
    SharedPtr<String> table_name_ptr_;
    SharedPtr<String> alias_name_ptr_;
};

} // namespace hybridsearch
