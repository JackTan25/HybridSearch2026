

module;

#include <iostream>

module score_threshold_iterator;
import stl;
import doc_iterator;
import internal_types;
import index_defines;

namespace hybridsearch {

bool ScoreThresholdIterator::Next(RowID doc_id) {
    while (true) {
        if (!(query_iterator_->Next(doc_id))) {
            doc_id_ = INVALID_ROWID;
            return false;
        }
        doc_id = query_iterator_->DocID();
        // check score
        if (Score() >= score_threshold_) {
            doc_id_ = doc_id;
            return true;
        }
        ++doc_id;
    }
}

void ScoreThresholdIterator::PrintTree(std::ostream &os, const String &prefix, const bool is_final) const {
    os << prefix;
    os << (is_final ? "└──" : "├──");
    os << "ScoreThresholdIterator (score_threshold: " << score_threshold_ << ")\n";
    const String next_prefix = prefix + (is_final ? "    " : "│   ");
    query_iterator_->PrintTree(os, next_prefix, true);
}

} // namespace hybridsearch
