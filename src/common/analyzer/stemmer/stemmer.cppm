

module;

export module stemmer;

import stl;

namespace hybridsearch {

export enum Language {
    STEM_LANG_UNKNOWN = 0,
    STEM_LANG_DANISH = 1,
    STEM_LANG_DUTCH = 2,
    STEM_LANG_ENGLISH,
    STEM_LANG_FINNISH,
    STEM_LANG_FRENCH,
    STEM_LANG_GERMAN,
    STEM_LANG_HUNGARIAN,
    STEM_LANG_ITALIAN,
    STEM_LANG_NORWEGIAN,
    STEM_LANG_PORT,
    STEM_LANG_PORTUGUESE,
    STEM_LANG_ROMANIAN,
    STEM_LANG_RUSSIAN,
    STEM_LANG_SPANISH,
    STEM_LANG_SWEDISH,
    STEM_LANG_TURKISH,
    STEM_LANG_EOS,
};

export class Stemmer {
public:
    Stemmer();

    virtual ~Stemmer();

    bool Init(Language language);

    void DeInit();

    bool Stem(const String &term, String &resultWord);

private:
    // int stemLang_; ///< language for stemming

    void *stem_function_; ///< stemming function
};

} // namespace hybridsearch
