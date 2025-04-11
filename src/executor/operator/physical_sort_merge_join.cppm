

module;

export module physical_sort_merge_join;

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

export class PhysicalSortMergeJoin : public PhysicalOperator {
public:
    explicit PhysicalSortMergeJoin(u64 id, SharedPtr<Vector<LoadMeta>> load_metas)
        : PhysicalOperator(PhysicalOperatorType::kJoinMerge, nullptr, nullptr, id, load_metas) {}

    ~PhysicalSortMergeJoin() override = default;

    void Init(QueryContext* query_context) override;

    bool Execute(QueryContext *query_context, OperatorState *operator_state) final;

    inline SharedPtr<Vector<String>> GetOutputNames() const final { return output_names_; }

    inline SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final { return output_types_; }

private:
    SharedPtr<Vector<String>> output_names_{};
    SharedPtr<Vector<SharedPtr<DataType>>> output_types_{};
};

} // namespace hybridsearch
