

module;

export module and_iterator;

import stl;
import index_defines;
import doc_iterator;
import multi_doc_iterator;
import internal_types;

namespace hybridsearch {

export class AndIterator : public MultiDocIterator {
public:
    AndIterator(Vector<UniquePtr<DocIterator>> iterators);

    DocIteratorType GetType() const override { return DocIteratorType::kAndIterator; }

    String Name() const override { return "AndIterator"; }

    /* pure virtual methods implementation */
    bool Next(RowID doc_id) override;

    float Score() override;

    void UpdateScoreThreshold(float threshold) override;

    u32 MatchCount() const override;

private:
    // score cache
    RowID score_cache_docid_ = INVALID_ROWID;
    float score_cache_ = 0.0f;
    // for minimum_should_match
    u32 fixed_match_count_ = 0;
    Vector<u32> dyn_match_ids_{};
};

} // namespace hybridsearch