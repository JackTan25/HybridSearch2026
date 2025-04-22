

module;

export module user_defined_term_weight;
#if 0
import stl;
import term;
import stemmer;
import analyzer;
import tokenizer;
import third_party;
import status;

namespace hybridsearch {
export class UserDefinedTermWeight {
public:
    UserDefinedTermWeight(const String &tw_path) : tw_path_(tw_path) {}

    ~UserDefinedTermWeight() = default;

    Tuple<HashMap<String, double>, Status> Run(const Vector<String> &text);

protected:
private:
    const String tw_path_;


};
} // namespace hybridsearchz
#endif
