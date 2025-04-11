

module;

export module rank_features_analyzer;
import stl;
import term;
import analyzer;

namespace hybridsearch {

export class RankFeaturesAnalyzer : public Analyzer {
    String delimiters_{};

public:
    RankFeaturesAnalyzer() = default;
    ~RankFeaturesAnalyzer() override = default;

protected:
    int AnalyzeImpl(const Term &input, void *data, HookType func) override;
};

} // namespace hybridsearch
