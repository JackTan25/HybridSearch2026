

module;

#include <sstream>
#include <string>
module whitespace_analyzer;
import stl;
import term;
import analyzer;

namespace hybridsearch {

WhitespaceAnalyzer::WhitespaceAnalyzer(const std::string_view delimiters) {
    delimiters_ = delimiters;
    std::sort(delimiters_.begin(), delimiters_.end());
    const auto last_unique = std::unique(delimiters_.begin(), delimiters_.end());
    delimiters_.erase(last_unique, delimiters_.end());
}

int WhitespaceAnalyzer::AnalyzeImpl(const Term &input, void *data, HookType func) {
    if (delimiters_.empty()) {
        // split by std::isspace()
        std::istringstream is(input.text_);
        std::string t;
        u32 offset = 0;
        while (is >> t) {
            func(data, t.data(), t.size(), offset++, 0, false, 0);
        }
        return 0;
    } else {
        // split by delimiters
        const std::string_view delimiters = delimiters_;
        const std::string_view input_text = input.text_;
        u32 search_start = 0;
        u32 offset = 0;
        while (search_start < input_text.size()) {
            const auto found = input_text.find_first_of(delimiters, search_start);
            if (found == std::string_view::npos) {
                func(data, input_text.data() + search_start, input_text.size() - search_start, offset++, 0, false, 0);
                break;
            }
            if (found > search_start) {
                func(data, input_text.data() + search_start, found - search_start, offset++, 0, false, 0);
            }
            search_start = found + 1;
        }
        return 0;
    }
}

} // namespace hybridsearch
