

module;

export module doc_iterator;

import stl;

import index_defines;
import internal_types;

namespace hybridsearch {

export enum class EarlyTermAlgo {
    kAuto,    // choose between kNaive, kBatch, kBMW
    kNaive,   // naive or
    kBatch,   // use batch_or if (sum_of_df > total_doc_num / 4) and term nodes under or node achieve a certain number
    kBMW,     // use bmw if it is "or iterator" on the top level and has only term children
    kCompare, // compare bmw, batch, naive
};

export enum class DocIteratorType : u8 {
    kTermDocIterator,
    kPhraseIterator,
    kAndIterator,
    kAndNotIterator,
    kOrIterator,
    kMinimumShouldMatchIterator,
    kBatchOrIterator,
    kBMWIterator,
    kFilterIterator,
    kScoreThresholdIterator,
    kKeywordIterator,
    kMustFirstIterator,
    kRankFeatureDocIterator,
    kRankFeaturesDocIterator,
};

export struct DocIteratorEstimateIterateCost {
    u32 priority_ = 0;
    u32 estimate_cost_ = 0;
    friend auto operator<=>(const DocIteratorEstimateIterateCost &, const DocIteratorEstimateIterateCost &) = default;
    auto &operator+=(DocIteratorEstimateIterateCost rhs) {
        if (priority_ < rhs.priority_) {
            std::swap(*this, rhs);
        }
        estimate_cost_ += (rhs.estimate_cost_ >> 3 * (priority_ - rhs.priority_));
        return *this;
    }
};

export class DocIterator {
public:
    DocIterator() {}

    virtual ~DocIterator();

    RowID DocID() const { return doc_id_; }

    inline DocIteratorEstimateIterateCost GetEstimateIterateCost() const { return estimate_iterate_cost_; }

    // Update doc_id_ to one larger than previous one.
    // If has_blockmax is true, it ensures its BM25 score be larger than current threshold.
    // Note that this routine decode skip_list and doc_list, and doesn't decode tf_list.
    // Caller may invoke BlockMaxBM25Score() and BM25Score() after this routine.
    inline bool Next() {
        RowID target_doc_id = (doc_id_ == INVALID_ROWID) ? 0 : (doc_id_ + 1);
        return Next(target_doc_id);
    }

    inline float BM25ScoreUpperBound() const { return bm25_score_upper_bound_; }

    inline float Threshold() const { return threshold_; }

    /* virtual methods */

    virtual DocIteratorType GetType() const = 0;
    virtual String Name() const = 0;

    // Update doc_id_ to one no less than given doc_id.
    // If has_blockmax is true, it ensures its BM25 score be larger than current threshold.
    virtual bool Next(RowID doc_id) = 0;

    virtual float Score() = 0;

    virtual void UpdateScoreThreshold(float threshold) = 0;

    // for minimum_should_match parameter
    virtual u32 MatchCount() const = 0;

    // print the query tree, for debugging
    virtual void PrintTree(std::ostream &os, const String &prefix = "", bool is_final = true) const = 0;

protected:
    RowID doc_id_{INVALID_ROWID};
    float threshold_ = 0.0f;
    float bm25_score_upper_bound_ = std::numeric_limits<float>::max();
    DocIteratorEstimateIterateCost estimate_iterate_cost_ = {};
};
} // namespace hybridsearch
