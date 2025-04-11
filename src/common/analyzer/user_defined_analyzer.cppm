

module;

export module user_defined_analyzer;
#if 0
#include "Python.h"
import stl;
import term;
import stemmer;
import analyzer;
import tokenizer;
import third_party;
import status;

namespace hybridsearch {
export class UserDefinedAnalyzer : public Analyzer {
public:
    UserDefinedAnalyzer(const String &analyzer_path) : analyzer_path_(analyzer_path) {}

    ~UserDefinedAnalyzer() = default;

    Tuple<Vector<String>, Status> Analyze(const String &text);

protected:
private:
    const String analyzer_path_;
};
} // namespace hybridsearch
#endif