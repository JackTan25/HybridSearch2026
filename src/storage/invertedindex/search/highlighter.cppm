module;

module;

export module highlighter;

import stl;
import singleton;
import aho_corasick;
import analyzer;

namespace hybridsearch {

export struct HighlightInfo {
    Vector<String> query_terms_;
    String analyzer_;
};

export class Highlighter : public Singleton<Highlighter> {
public:
    Highlighter();

    void GetHighlightWithoutStemmer(const Vector<String> &query, const String &raw_text, String &output);

    void GetHighlightWithStemmer(const Vector<String> &query, const String &raw_text, String &output, Analyzer *analyzer);

private:
    AhoCorasick sentence_delimiter_;
};
}; // namespace hybridsearch
