

module;

export module whitespace_analyzer;
import stl;
import term;
import analyzer;

namespace hybridsearch {

export class WhitespaceAnalyzer : public Analyzer {
    String delimiters_{};

public:
    WhitespaceAnalyzer() = default;
    explicit WhitespaceAnalyzer(std::string_view delimiters);
    ~WhitespaceAnalyzer() override = default;

protected:
    int AnalyzeImpl(const Term &input, void *data, HookType func) override;
};

} // namespace hybridsearch
