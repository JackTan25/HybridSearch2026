

module;

export module bound_statement;

import stl;
import logical_node;
import query_context;

import internal_types;
import data_type;

namespace hybridsearch {

export struct BoundStatement {
public:
    SharedPtr<Vector<String>> names_ptr_{};
    SharedPtr<Vector<SharedPtr<DataType>>> types_ptr_{};

    virtual SharedPtr<LogicalNode> BuildPlan(QueryContext *query_context) = 0;
    virtual ~BoundStatement() = default;
};

} // namespace hybridsearch
