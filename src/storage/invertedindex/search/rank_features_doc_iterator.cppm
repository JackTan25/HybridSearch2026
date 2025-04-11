

module;

export module rank_features_doc_iterator;

import stl;
import index_defines;
import doc_iterator;
import multi_doc_iterator;
import internal_types;

namespace hybridsearch {

export class RankFeaturesDocIterator : public MultiDocIterator {
public:
    explicit RankFeaturesDocIterator(Vector<UniquePtr<DocIterator>> &&iterators);

    ~RankFeaturesDocIterator() override;

    DocIteratorType GetType() const override { return DocIteratorType::kRankFeaturesDocIterator; }

    String Name() const override { return "RankFeaturesDocIterator"; }

    void UpdateScoreThreshold(float threshold) override {}

    bool Next(RowID doc_id) override;

    float Score() override;

    u32 MatchCount() const override;

    void DecodeFrom(RowID buffer_start_doc_id);

private:
    RowID buffer_start_doc_id_ = INVALID_ROWID;
    u32 memset_bytes_ = 0;
    void *aligned_buffer_ = nullptr;
    u32 *match_cnt_ptr_ = nullptr;
    u16 *payload_ptr_ = nullptr;
    f32 *score_sum_ptr_ = nullptr;
};

} // namespace hybridsearch
