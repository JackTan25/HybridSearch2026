

module;

export module keyword_iterator;

import stl;
import index_defines;
import doc_iterator;
import multi_doc_iterator;
import or_iterator;
import internal_types;

namespace hybridsearch {

export class KeywordIterator final : public MultiDocIterator {
public:
    KeywordIterator(Vector<UniquePtr<DocIterator>> iterators, const float weight) : MultiDocIterator(std::move(iterators)), weight_(weight) {
        bm25_score_upper_bound_ = weight;
        estimate_iterate_cost_ = {};
        for (u32 i = 0; i < children_.size(); ++i) {
            estimate_iterate_cost_ += children_[i]->GetEstimateIterateCost();
        }
    }

    DocIteratorType GetType() const override { return DocIteratorType::kKeywordIterator; }

    String Name() const override { return "KeywordIterator"; }

    /* pure virtual methods implementation */
    bool Next(const RowID doc_id) override {
        if (doc_id_ == INVALID_ROWID) {
            for (u32 i = 0; i < children_.size(); ++i) {
                children_[i]->Next();
                DocIteratorEntry entry = {children_[i]->DocID(), i};
                heap_.AddEntry(entry);
            }
            heap_.BuildHeap();
            doc_id_ = heap_.TopEntry().doc_id_;
        }
        if (doc_id_ != INVALID_ROWID && doc_id_ >= doc_id) {
            return true;
        }
        while (doc_id > heap_.TopEntry().doc_id_) {
            DocIterator *top = children_[heap_.TopEntry().entry_id_].get();
            top->Next(doc_id);
            heap_.TopEntry().doc_id_ = top->DocID();
            heap_.AdjustDown(1);
        }
        doc_id_ = heap_.TopEntry().doc_id_;
        return doc_id_ != INVALID_ROWID;
    }

    float Score() override { return weight_; }

    void UpdateScoreThreshold(float threshold) override { /* do nothing */ }

    u32 MatchCount() const override { return 0; }

private:
    const float weight_ = 1.0f;
    DocIteratorHeap heap_{};
};

} // namespace hybridsearch
