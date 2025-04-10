

module;

#include <vector>

import stl;

import physical_source;
import physical_sink;
import data_table;
import query_context;

module plan_fragment;

namespace hybridsearch {

void PlanFragment::SetSourceNode(QueryContext *query_context,
                                 SourceType source_type,
                                 const SharedPtr<Vector<String>> &names,
                                 const SharedPtr<Vector<SharedPtr<DataType>>> &types) {
    source_ = MakeUnique<PhysicalSource>(query_context->GetNextNodeID(), source_type, names, types, nullptr);
}

void PlanFragment::SetSinkNode(QueryContext *query_context,
                               SinkType sink_type,
                               const SharedPtr<Vector<String>> &names,
                               const SharedPtr<Vector<SharedPtr<DataType>>> &types) {
    sink_ = MakeUnique<PhysicalSink>(query_context->GetNextNodeID(), sink_type, names, types, nullptr);
}

SharedPtr<Vector<String>> PlanFragment::ToString() {
    auto result = MakeShared<Vector<String>>();
    if (source_.get() != nullptr) {
        result->push_back(source_->GetName());
    }
    for (auto &op : operators_) {
        result->push_back(op->GetName());
    }
    if (sink_.get() != nullptr) {
        result->push_back(sink_->GetName());
    }
    return result;
}

SharedPtr<DataTable> PlanFragment::GetResult() { return context_->GetResult(); }

void PlanFragment::AddNext(SharedPtr<PlanFragment> root, PlanFragment *next) {
    Vector<PlanFragment *> next_leaves;
    next->GetStartFragments(next_leaves);
    for (auto &leaf : next_leaves) {
        leaf->AddChild(root);
    }
}

SizeT PlanFragment::GetStartFragments(Vector<PlanFragment *> &leaf_fragments) {
    SizeT all_fragment_n = 0;
    HashSet<PlanFragment *> visited;
    std::function<void(PlanFragment *)> TraversePlanFragmentGraph = [&](PlanFragment *fragment) {
        if (visited.find(fragment) != visited.end()) {
            return;
        }
        visited.insert(fragment);
        if (fragment->GetContext()) {
            all_fragment_n += fragment->GetContext()->Tasks().size();
        }
        if (!fragment->HasChild()) {
            leaf_fragments.emplace_back(fragment);
            return;
        }
        for (auto &child : fragment->Children()) {
            TraversePlanFragmentGraph(child.get());
        }
    };
    TraversePlanFragmentGraph(this);
    return all_fragment_n;
}

} // namespace hybridsearch
