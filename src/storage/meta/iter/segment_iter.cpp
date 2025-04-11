

module;

module segment_iter;

import stl;
import catalog;

namespace hybridsearch {

BlockEntry *BlockEntryIter::Next() {
    std::shared_lock lock(segment_->rw_locker_);
    if (cur_block_idx_ >= segment_->block_entries_.size()) {
        return nullptr;
    }
    return segment_->block_entries_[cur_block_idx_++].get();
}

} // namespace hybridsearch