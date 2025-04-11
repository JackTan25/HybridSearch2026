

module;

import stl;
import term;
import stemmer;
import analyzer;
export module common_analyzer;

namespace hybridsearch {
export class CommonLanguageAnalyzer : public Analyzer {
public:
    CommonLanguageAnalyzer();
    CommonLanguageAnalyzer(const CommonLanguageAnalyzer &) = delete;
    virtual ~CommonLanguageAnalyzer();

    void InitStemmer(Language language);

    void SetCaseSensitive(bool case_sensitive = true, bool contain_lower = true) {
        case_sensitive_ = case_sensitive;
        contain_lower_ = contain_lower;
    }

    void SetExtractEngStem(bool extract_eng_stem = true) { extract_eng_stem_ = extract_eng_stem; }

    void SetStemOnly(bool stem_only = true) { stem_only_ = stem_only; }

    void SetExtractSynonym(bool extract_synonym = true) { extract_synonym_ = extract_synonym; }

    void SetRemoveStopwords(bool remove_stopwords = true) { remove_stopwords_ = remove_stopwords; }

    bool IsRemoveStopwords() { return remove_stopwords_; }

protected:
    int AnalyzeImpl(const Term &input, void *data, HookType func) override;
    /// Parse given input
    virtual void Parse(const String &input) = 0;

    /// Fill token_, len_, offset_
    virtual bool NextToken() = 0;

    /// whether morpheme_ indicates foreign language
    virtual bool IsAlpha() = 0;

    /// whether morpheme_ indicates special character, e.g. punctuations
    virtual bool IsSpecialChar() = 0;

    /// whether current token is stopword
    virtual bool IsStopword() { return false; }

    inline void ResetToken() {
        token_ = nullptr;
        len_ = 0;
        offset_ = 0;
        end_offset_ = 0;
        is_index_ = false;
        is_raw_ = false;
    }

protected:
    static const SizeT term_string_buffer_limit_ = 4096 * 3;

    Vector<char> lowercase_string_buffer_;
    UniquePtr<Stemmer> stemmer_{nullptr};
    const char *token_{nullptr};
    SizeT len_{0};
    u32 offset_{0};
    u32 end_offset_{0};
    u32 local_offset_{0};
    bool is_index_{false};
    bool is_raw_{false};
    bool case_sensitive_{false};
    bool contain_lower_{false};
    bool extract_eng_stem_{true};
    bool extract_synonym_{false};
    bool cjk_{false};
    bool remove_stopwords_{false};
    bool stem_only_{false};
};

} // namespace hybridsearch