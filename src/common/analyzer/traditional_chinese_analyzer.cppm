

module;

export module traditional_chinese_analyzer;

import stl;
import status;
import chinese_analyzer;

class OpenCC;

namespace hybridsearch {

export class TraditionalChineseAnalyzer : public ChineseAnalyzer {
public:
    TraditionalChineseAnalyzer(const String &path);

    TraditionalChineseAnalyzer(const TraditionalChineseAnalyzer &other);

    ~TraditionalChineseAnalyzer();

    Status Load();

protected:
    void Parse(const String &input) override;

private:
    OpenCC *opencc_{nullptr};
    bool own_opencc_{false};
};
} // namespace hybridsearch
