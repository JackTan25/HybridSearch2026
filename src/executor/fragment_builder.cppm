

module;

import stl;
import query_context;
import plan_fragment;
import physical_operator;
import global_resource_usage;

export module fragment_builder;

namespace hybridsearch {

export class FragmentBuilder {
public:
    explicit FragmentBuilder(QueryContext *query_context_ptr) : query_context_ptr_(query_context_ptr) {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::IncrObjectCount("FragmentBuilder");
#endif
    }

    ~FragmentBuilder() {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::DecrObjectCount("FragmentBuilder");
#endif
    }

    SharedPtr<PlanFragment> BuildFragment(const Vector<PhysicalOperator *> &physical_plans);

private:
    void BuildFragments(PhysicalOperator *phys_op, PlanFragment *current_fragment_ptr);

    void BuildExplain(PhysicalOperator *phys_op, PlanFragment *current_fragment_ptr);

    idx_t GetFragmentId() { return fragment_id_++; }

private:
    QueryContext *query_context_ptr_{};
    idx_t fragment_id_{};
};

} // namespace hybridsearch