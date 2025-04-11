

module;

export module physical_index_join;

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

namespace hybridsearch {

export class PhysicalIndexJoin : public PhysicalOperator {
public:
    explicit PhysicalIndexJoin(u64 id, SharedPtr<Vector<LoadMeta>> load_metas)
        : PhysicalOperator(PhysicalOperatorType::kJoinIndex, nullptr, nullptr, id, load_metas) {}

    ~PhysicalIndexJoin() override = default;

    void Init(QueryContext* query_context) override;

    bool Execute(QueryContext *query_context, OperatorState *operator_state) final;

    SharedPtr<Vector<String>> GetOutputNames() const final;

    SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final;
};

} // namespace hybridsearch
