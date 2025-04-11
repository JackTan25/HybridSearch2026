

module;

import plan_fragment;
import stl;

export module explain_fragment;

namespace hybridsearch {

export class ExplainFragment {
public:
    static void Explain(PlanFragment *op, SharedPtr<Vector<SharedPtr<String>>> &result, i64 intent_size = 0);
};

} // namespace hybridsearch
