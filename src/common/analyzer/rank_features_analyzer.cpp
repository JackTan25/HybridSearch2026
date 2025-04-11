

module;

#include <string>
module rank_features_analyzer;
import stl;
import term;
import analyzer;
import smallfloat;
import third_party;

namespace hybridsearch {

int RankFeaturesAnalyzer::AnalyzeImpl(const Term &input, void *data, HookType func) {
    nlohmann::json line_json = nlohmann::json::parse(input.text_);
    u32 offset = 0;
    for (const auto &element : line_json) {
        if (element.is_object()) {
            for (auto it = element.begin(); it != element.end(); ++it) {
                std::string key = it.key();
                float value = it.value();
                u16 weight = SmallFloat::Float122ToUInt16(value);
                func(data, key.data(), key.size(), offset++, 0, false, weight);
            }
        }
    }

    return 0;
}

} // namespace hybridsearch
