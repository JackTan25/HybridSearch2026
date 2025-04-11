

module;

export module parse_fulltext_options;

import stl;

namespace hybridsearch {

// parse minimum_should_match
struct MinimumShouldMatchCount {
    i32 value_ = 0;
};

struct MinimumShouldMatchPercentage {
    i8 value_ = 0;
};

using SingleMinimumShouldMatchOption = Pair<u32, std::variant<MinimumShouldMatchCount, MinimumShouldMatchPercentage>>;

export using MinimumShouldMatchOption = Vector<SingleMinimumShouldMatchOption>;

export MinimumShouldMatchOption ParseMinimumShouldMatchOption(std::string_view input_str);

export u32 GetMinimumShouldMatchParameter(const MinimumShouldMatchOption &option_vec, u32 leaf_count);

export enum class FulltextSimilarity {
    kBM25,
    kBoolean,
};

export struct BM25Params {
    float k1 = 1.2F;
    float b = 0.75F;
    float delta_term = 0.0F;
    float delta_phrase = 0.0F;
};

export struct RankFeatureOption {
    String field_;
    String feature_;
    float boost_;
};

export using RankFeaturesOption = Vector<RankFeatureOption>;

export RankFeaturesOption ParseRankFeaturesOption(std::string_view input_str);

} // namespace hybridsearch
