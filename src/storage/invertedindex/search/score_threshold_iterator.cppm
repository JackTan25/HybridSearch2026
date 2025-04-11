

module;

export module score_threshold_iterator;
import stl;
import doc_iterator;
import internal_types;

namespace hybridsearch {

export class ScoreThresholdIterator final : public DocIterator {
    UniquePtr<DocIterator> query_iterator_{};
    f32 score_threshold_ = {};

public:
    ScoreThresholdIterator(UniquePtr<DocIterator> &&iter, const f32 score_threshold)
        : query_iterator_(std::move(iter)), score_threshold_(score_threshold) {}
    ~ScoreThresholdIterator() override = default;
    DocIteratorType GetType() const override { return DocIteratorType::kScoreThresholdIterator; }
    String Name() const override { return "ScoreThresholdIterator"; };
    bool Next(RowID doc_id) override;
    float Score() override { return query_iterator_->Score(); }
    void UpdateScoreThreshold(const float threshold) override { query_iterator_->UpdateScoreThreshold(threshold); }
    u32 MatchCount() const override { return query_iterator_->MatchCount(); }
    void PrintTree(std::ostream &os, const String &prefix, bool is_final) const override;
};

} // namespace hybridsearch
