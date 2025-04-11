

module;

import stl;

import table_ref;
import bound_statement;
import table_reference;

export module subquery_table_ref;

namespace hybridsearch {

export class SubqueryTableRef : public TableRef {
public:
    explicit SubqueryTableRef(UniquePtr<BoundStatement> subquery_node, u64 table_index, String alias)
        : TableRef(TableRefType::kSubquery, std::move(alias)), subquery_node_(std::move(subquery_node)), table_index_(table_index) {}

    UniquePtr<BoundStatement> subquery_node_{nullptr};
    u64 table_index_{};
};

} // namespace hybridsearch