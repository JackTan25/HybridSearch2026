

module;

#include <string>

import stl;
import plan_fragment;
import explain_physical_plan;
import physical_operator;

module explain_fragment;

namespace hybridsearch {

static String FragmentTitle(u64 fragment_id, bool is_head) {
    if (!is_head) {
        return "FRAGMENT #" + std::to_string(fragment_id);
    }

    return "FRAGMENT (" + std::to_string(fragment_id) + ")";
}

void ExplainFragment::Explain(PlanFragment *plan_fragment_ptr, SharedPtr<Vector<SharedPtr<String>>> &result, i64 intent_size) {
    result->emplace_back(MakeShared<String>(FragmentTitle(plan_fragment_ptr->FragmentID(), true)));

    if (plan_fragment_ptr->GetSinkNode()) {
        ExplainPhysicalPlan::Explain(plan_fragment_ptr->GetSinkNode(), result, false, 2);
    }

    Vector<PhysicalOperator *> &fragment_operators = plan_fragment_ptr->GetOperators();
    for (auto &fragment_operator : fragment_operators) {
        ExplainPhysicalPlan::Explain(fragment_operator, result, false, 2);
    }

    if (plan_fragment_ptr->GetSourceNode()) {
        ExplainPhysicalPlan::Explain(plan_fragment_ptr->GetSourceNode(), result, false, 2);

        if (!plan_fragment_ptr->Children().empty()) {
            String fragment_footer = *result->back().get();

            fragment_footer += ": ";
            for (SizeT i = 0; i < plan_fragment_ptr->Children().size(); ++i) {
                if (i > 0) {
                    fragment_footer += ", ";
                }
                fragment_footer += FragmentTitle(plan_fragment_ptr->Children()[i]->FragmentID(), false);
            }
            result->back() = MakeShared<String>(fragment_footer);
        }
    }
    // NOTE: Insert blank elements after each Fragment for alignment
    result->emplace_back(MakeShared<String>());

    // NOTE: recursive call this function to explain child fragment
    if (!plan_fragment_ptr->Children().empty()) {
        for (auto &child : plan_fragment_ptr->Children()) {
            ExplainFragment::Explain(child.get(), result, 2);

            // NOTE: Insert blank elements after each Fragment for alignment
            result->emplace_back(MakeShared<String>());
        }
    }
}

} // namespace hybridsearch
