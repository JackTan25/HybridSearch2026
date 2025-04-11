

module;

import stl;
import term;
import stemmer;
import analyzer;
import tokenizer;
export module ngram_analyzer;

namespace hybridsearch {
export class NGramAnalyzer : public Analyzer {
public:
    NGramAnalyzer(u32 ngram) : ngram_(ngram) {}

    ~NGramAnalyzer() = default;

protected:
    int AnalyzeImpl(const Term &input, void *data, HookType func) override;

    bool NextInString(const char *data, SizeT length, SizeT *__restrict pos, SizeT *__restrict token_start, SizeT *__restrict token_length);

private:
    u32 ngram_;
};
} // namespace hybridsearch
