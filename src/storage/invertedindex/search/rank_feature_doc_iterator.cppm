

module;

export module rank_feature_doc_iterator;

import stl;

import posting_iterator;
import index_defines;
import doc_iterator;
import internal_types;
import doc_iterator;
import third_party;

namespace hybridsearch {

export class RankFeatureDocIterator final : public DocIterator {
public:
    RankFeatureDocIterator(UniquePtr<PostingIterator> &&iter, u64 column_id, float boost);

    ~RankFeatureDocIterator() override;

    DocIteratorType GetType() const override { return DocIteratorType::kRankFeatureDocIterator; }

    float GetWeight() const { return boost_; }

    String Name() const override { return "RankFeatureDocIterator"; }

    void UpdateScoreThreshold(float threshold) override {}

    u32 MatchCount() const override { return 0; }

    bool Next(RowID doc_id) override;

    float Score() override;

    void PrintTree(std::ostream &os, const String &prefix, bool is_final) const override;

    void BatchDecodeTo(RowID buffer_start_doc_id, RowID buffer_end_doc_id, u16 *payload_ptr);

    const String *term_ptr_ = nullptr;
    const String *column_name_ptr_ = nullptr;

private:
    u64 column_id_;
    float boost_ = 1.0f;
    UniquePtr<PostingIterator> iter_;
};

} // namespace hybridsearch
