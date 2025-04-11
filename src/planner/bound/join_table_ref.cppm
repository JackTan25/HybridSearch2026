

module;

export module join_table_ref;

import stl;

import table_ref;
import base_expression;
import table_reference;
import join_reference;

namespace hybridsearch {

class BindContext;

export class JoinTableRef : public TableRef {
public:
    explicit JoinTableRef(String alias) : TableRef(TableRefType::kJoin, std::move(alias)) {}

    SharedPtr<BindContext> left_bind_context_;
    SharedPtr<BindContext> right_bind_context_;

    SharedPtr<TableRef> left_table_ref_;
    SharedPtr<TableRef> right_table_ref_;

    // Condition ???
    Vector<SharedPtr<BaseExpression>> on_conditions_;

    // Join Type ???
    JoinType join_type_;
};

} // namespace hybridsearch