

module;

export module korean_analyzer;

import stl;
import ijma;
import term;
import common_analyzer;
import status;

namespace hybridsearch {

export class KoreanAnalyzer : public CommonLanguageAnalyzer {
public:
    KoreanAnalyzer(const String &path);

    KoreanAnalyzer(const KoreanAnalyzer &other);

    ~KoreanAnalyzer();

    Status Load();

protected:
    void Parse(const String &input) override {
        mecab_->SetSentence(input);
        local_offset_ = -1;

        ResetToken();
    }

    bool NextToken() override {
        while (DoNext()) {
            mecab_->GetToken(token_str_);
            token_ = token_str_.c_str();
            len_ = token_str_.size();
            offset_ = local_offset_;
            is_index_ = true;
            return true;
        }
        ResetToken();
        return false;
    }

    bool IsAlpha() override { return mecab_->IsAlpha(); }

    bool IsSpecialChar() override { return false; }

private:
    bool DoNext() {
        while (!mecab_->IsEnd()) {
            mecab_->Next();
            ++local_offset_;
            return true;
        }
        return false;
    }

    String knowledge_path_;

    jma::MeCab *mecab_{nullptr};

    bool own_mecab_;

    String token_str_;
};
} // namespace hybridsearch
