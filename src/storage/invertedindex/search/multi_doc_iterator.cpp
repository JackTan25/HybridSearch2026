

module;

#include <iostream>

module multi_doc_iterator;

import stl;
import hybridsearch_exception;
import logger;

namespace hybridsearch {

void MultiDocIterator::PrintTree(std::ostream &os, const String &prefix, bool is_final) const {
    os << prefix;
    os << (is_final ? "└──" : "├──");
    os << Name();
    {
        auto [level, cost] = GetEstimateIterateCost();
        os << " (estimate_iterate_cost: " << level << ", " << cost << ")";
    }
    os << " (bm25_score_upper_bound: " << BM25ScoreUpperBound() << ")";
    os << " (threshold: " << Threshold() << ")";
    os << " (children count: " << children_.size() << ")";
    os << '\n';
    std::string next_prefix = prefix + (is_final ? "    " : "│   ");
    for (u32 i = 0; i + 1 < children_.size(); ++i) {
        children_[i]->PrintTree(os, next_prefix, false);
    }
    children_.back()->PrintTree(os, next_prefix, true);
}

} // namespace hybridsearch