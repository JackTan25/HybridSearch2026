

module;

export module batch_or_iterator;

import stl;
import index_defines;
import doc_iterator;
import multi_doc_iterator;
import internal_types;

namespace hybridsearch {

export class BatchOrIterator : public MultiDocIterator {
public:
    explicit BatchOrIterator(Vector<UniquePtr<DocIterator>> &&iterators);

    ~BatchOrIterator() override;

    DocIteratorType GetType() const override { return DocIteratorType::kBatchOrIterator; }

    String Name() const override { return "BatchOrIterator"; }

    void UpdateScoreThreshold(float threshold) override {}

    bool Next(RowID doc_id) override;

    float BM25Score() const;

    float Score() override { return BM25Score(); }

    u32 MatchCount() const override;

    void DecodeFrom(RowID buffer_start_doc_id);

private:
    Vector<f32> f1_;
    Vector<f32> f2_;
    Vector<f32> bm25_common_score_;
    RowID buffer_start_doc_id_ = INVALID_ROWID;
    u32 memset_bytes_ = 0;
    void *aligned_buffer_ = nullptr;
    u32 *tf_ptr_ = nullptr;
    u32 *doc_len_ptr_ = nullptr;
    u32 *match_cnt_ptr_ = nullptr;
    f32 *bm25_score_sum_ptr_ = nullptr;
};

} // namespace hybridsearch
