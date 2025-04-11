

module;

#include "string_utils.h"

import stl;
import term;
import stemmer;
import analyzer;
import tokenizer;
module ngram_analyzer;

namespace hybridsearch {

bool NGramAnalyzer::NextInString(const char *data,
                                 SizeT length,
                                 SizeT *__restrict pos,
                                 SizeT *__restrict token_start,
                                 SizeT *__restrict token_length) {
    *token_start = *pos;
    *token_length = 0;
    SizeT code_points = 0;
    for (; code_points < ngram_ && *token_start + *token_length < length; ++code_points) {
        if (std::isspace(data[*token_start + *token_length])) {
            *pos += UTF8SeqLength(static_cast<u8>(data[*pos]));
            *token_start = *pos;
            *token_length = 0;
            return true;
        }
        SizeT sz = UTF8SeqLength(static_cast<u8>(data[*token_start + *token_length]));
        *token_length += sz;
    }
    *pos += UTF8SeqLength(static_cast<u8>(data[*pos]));
    return code_points == ngram_;
}

int NGramAnalyzer::AnalyzeImpl(const Term &input, void *data, HookType func) {
    SizeT len = input.text_.length();
    if (len == 0)
        return 0;

    SizeT cur = 0;
    SizeT token_start = 0;
    SizeT token_length = 0;
    SizeT offset = input.word_offset_;
    while (cur < len && NextInString(input.text_.c_str(), len, &cur, &token_start, &token_length)) {
        if (token_length == 0)
            continue;
        func(data, input.text_.c_str() + token_start, token_length, offset, offset + token_length, false, 0);
        offset++;
    }

    return offset;
}

} // namespace hybridsearch
