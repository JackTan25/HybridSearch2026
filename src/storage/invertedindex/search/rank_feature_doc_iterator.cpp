

module;

#include <cassert>
#include <iostream>
module rank_feature_doc_iterator;

import stl;
import logger;
import smallfloat;

namespace hybridsearch {

RankFeatureDocIterator::RankFeatureDocIterator(UniquePtr<PostingIterator> &&iter, const u64 column_id, float boost)
    : column_id_(column_id), boost_(boost), iter_(std::move(iter)) {}

RankFeatureDocIterator::~RankFeatureDocIterator() {}

bool RankFeatureDocIterator::Next(RowID doc_id) {
    assert(doc_id != INVALID_ROWID);
    if (doc_id_ != INVALID_ROWID && doc_id_ >= doc_id)
        return true;
    doc_id_ = iter_->SeekDoc(doc_id);
    if (doc_id_ == INVALID_ROWID)
        return false;
    return true;
}

float RankFeatureDocIterator::Score() {
    u16 payload = iter_->GetCurrentDocPayload();
    float weight = SmallFloat::UInt16ToFloat122(payload);
    return weight * boost_;
}

void RankFeatureDocIterator::PrintTree(std::ostream &os, const String &prefix, bool is_final) const {
    os << prefix;
    os << (is_final ? "└──" : "├──");
    os << "RankFeatureDocIterator";
    os << " (column: " << *column_name_ptr_ << ")";
    os << " (term: " << *term_ptr_ << ")";
    os << '\n';
}

void RankFeatureDocIterator::BatchDecodeTo(const RowID buffer_start_doc_id, const RowID buffer_end_doc_id, u16 *payload_ptr) {
    auto iter_doc_id = iter_->DocID();
    assert((buffer_start_doc_id <= iter_doc_id && iter_doc_id < buffer_end_doc_id));
    while (iter_doc_id < buffer_end_doc_id) {
        const auto pos = iter_doc_id - buffer_start_doc_id;
        const auto payload = iter_->GetCurrentDocPayload();
        payload_ptr[pos] = payload;
        iter_doc_id = iter_->SeekDoc(iter_doc_id + 1);
    }
    doc_id_ = iter_doc_id;
}

} // namespace hybridsearch
