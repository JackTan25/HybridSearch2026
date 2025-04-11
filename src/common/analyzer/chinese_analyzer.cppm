

module;

export module chinese_analyzer;

import stl;
import jieba;
import term;
import analyzer;
import common_analyzer;
import third_party;
import status;

namespace hybridsearch {

export class ChineseAnalyzer : public CommonLanguageAnalyzer {
public:
    ChineseAnalyzer(const String &path);

    ChineseAnalyzer(const ChineseAnalyzer &other);

    ~ChineseAnalyzer();

    Status Load();

    void SetCutGrain(CutGrain cut_grain) { cut_grain_ = cut_grain; }

protected:
    void Parse(const String &input) override;

    bool NextToken() override;

    bool IsAlpha() override { return jieba_->IsAlpha(token_); }

    bool IsSpecialChar() override { return false; }

private:
    void LoadStopwordsDict(const String &stopwords_path);

    bool Accept_token(const String &term) { return !stopwords_->contains(term); }

    bool DoNext();

protected:
    cppjieba::Jieba *jieba_{nullptr};
    String dict_path_;
    bool own_jieba_{};
    Vector<cppjieba::Word> cut_words_;
    i32 cursor_{};
    i32 cut_size_{};
    SharedPtr<FlatHashSet<String>> stopwords_{};
    CutGrain cut_grain_{CutGrain::kCoarse};
};
} // namespace hybridsearch
