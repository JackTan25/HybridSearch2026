

module;

export module standard_analyzer;

import stl;
import term;
import stemmer;
import analyzer;
import tokenizer;
import common_analyzer;

namespace hybridsearch {
export class StandardAnalyzer : public CommonLanguageAnalyzer {
public:
    StandardAnalyzer() : CommonLanguageAnalyzer() {}

    ~StandardAnalyzer() {}

    void InitStemmer(Language language) { CommonLanguageAnalyzer::InitStemmer(language); }

protected:
    inline void Parse(const String &input) override {
        tokenizer_.Tokenize(input);
        local_offset_ = 0;
        ResetToken();
    }

    inline bool NextToken() override {
        if (DoNext()) {
            token_ = tokenizer_.GetToken();
            len_ = tokenizer_.GetLength();
            offset_ = get_char_offset_ ? tokenizer_.GetTokenStartCursor() : local_offset_;
            end_offset_ = tokenizer_.GetInputCursor();
            local_offset_++;
            is_index_ = true;
            return true;
        } else {
            ResetToken();
            return false;
        }
    }

    bool DoNext() {
        while (tokenizer_.NextToken()) {
            if (!IsSpecialChar()) {
                return true;
            } else {
                continue;
            }
        }
        return false;
    }
    inline bool IsAlpha() override { return true; }

    inline bool IsSpecialChar() override { return tokenizer_.IsDelimiter(); }
};
} // namespace hybridsearch