

module;

#include <cmath>

module bm25_ranker;

import stl;
import index_defines;
import third_party;

namespace hybridsearch {

constexpr float k1 = 1.2F;
constexpr float b = 0.75F;

BM25Ranker::BM25Ranker(u64 total_df) : total_df_(std::max(total_df, (u64)1)) {}

void BM25Ranker::AddTermParam(u64 tf, u64 df, float avg_column_len, u32 column_len, float weight) {
    float smooth_idf = std::log(1.0F + (total_df_ - df + 0.5F) / (df + 0.5F));
    float smooth_tf = (k1 + 1.0F) * tf / (tf + k1 * (1.0F - b + b * column_len / avg_column_len));
    score_ += smooth_idf * smooth_tf * weight;
}

void BM25Ranker::AddPhraseParam(float tf, u64 df, float avg_column_len, u32 column_len, float weight) {
    float smooth_idf = std::log(1.0F + (total_df_ - df + 0.5F) / (df + 0.5F));
    float smooth_tf = (k1 + 1.0F) * tf / (tf + k1 * (1.0F - b + b * column_len / avg_column_len));
    score_ += smooth_idf * smooth_tf * weight;
}

} // namespace hybridsearch
