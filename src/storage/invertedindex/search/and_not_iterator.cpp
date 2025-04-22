

module;

module and_not_iterator;

import stl;
import index_defines;
import multi_doc_iterator;
import doc_iterator;
import internal_types;

namespace hybridsearch {

AndNotIterator::AndNotIterator(Vector<UniquePtr<DocIterator>> iterators) : MultiDocIterator(std::move(iterators)) {
    std::sort(children_.begin() + 1, children_.end(), [](const auto &lhs, const auto &rhs) {
        return lhs->GetEstimateIterateCost() < rhs->GetEstimateIterateCost();
    });
    bm25_score_upper_bound_ = children_[0]->BM25ScoreUpperBound();
    estimate_iterate_cost_ = children_[0]->GetEstimateIterateCost();
}

bool AndNotIterator::Next(RowID doc_id) {
    bool next_loop = false;
    do {
        children_[0]->Next(doc_id);
        doc_id = children_[0]->DocID();
        if (doc_id == INVALID_ROWID) [[unlikely]] {
            break;
        }
        // now doc_id < INVALID_ROWID
        next_loop = false;
        for (u32 i = 1; i < children_.size(); ++i) {
            children_[i]->Next(doc_id);
            if (RowID doc = children_[i]->DocID(); doc == doc_id) {
                ++doc_id;
                next_loop = true;
                break;
            }
        }
    } while (next_loop);
    doc_id_ = doc_id;
    return doc_id != INVALID_ROWID;
}

float AndNotIterator::Score() { return children_[0]->Score(); }

void AndNotIterator::UpdateScoreThreshold(float threshold) { children_[0]->UpdateScoreThreshold(threshold); }

u32 AndNotIterator::MatchCount() const { return children_[0]->MatchCount(); }

} // namespace hybridsearch