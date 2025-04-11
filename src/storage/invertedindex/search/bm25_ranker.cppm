

module;

export module bm25_ranker;

import stl;
import index_defines;

namespace hybridsearch {
export class BM25Ranker {
public:
    BM25Ranker(u64 total_df);
    ~BM25Ranker() = default;

    void AddTermParam(u64 tf, u64 df, float avg_column_len, u32 column_len, float weight);

    void AddPhraseParam(float tf, u64 df, float avg_colum_len, u32 column_len, float weight);

    float GetScore() { return score_; }

private:
    float score_{0};
    u64 total_df_{0};
};
} // namespace hybridsearch