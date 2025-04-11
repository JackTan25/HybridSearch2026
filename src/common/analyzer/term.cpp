

module;

import stl;

module term;

namespace hybridsearch {

String PLACE_HOLDER("<PH>");

void Term::Reset() {
    text_.clear();
    word_offset_ = 0;
}

Term TermList::global_temporary_;

} // namespace hybridsearch