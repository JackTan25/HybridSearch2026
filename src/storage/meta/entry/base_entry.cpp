

module;

module base_entry;

import txn_manager;

namespace hybridsearch {

BaseEntry::BaseEntry(const BaseEntry &other) : deleted_(other.deleted_), entry_type_(other.entry_type_), encode_(other.encode_) {
    txn_id_ = other.txn_id_;
    begin_ts_ = other.begin_ts_;
    commit_ts_ = other.commit_ts_.load();

#ifdef hybridsearch_DEBUG
    GlobalResourceUsage::IncrObjectCount("BaseEntry");
#endif
}

bool BaseEntry::CheckVisible(Txn *txn) const {
    TxnTimeStamp begin_ts = txn->BeginTS();
    if (txn_id_ == 0) {
        // could not check if the entry is visible accurately. log a warning and return true
        LOG_WARN(fmt::format("Entry {} txn id is not set, commit_ts: {}", *encode_, commit_ts_));
        return begin_ts >= commit_ts_;
    }
    if (begin_ts >= commit_ts_ || txn_id_ == txn->TxnID()) {
        return true;
    }
    TxnManager *txn_mgr = txn->txn_mgr();
    if (txn_mgr == nullptr) { // when replay
        String error_message = fmt::format("Replay should not reach here. begin_ts: {}, commit_ts_: {} txn_id: {}, txn_id_: {}",
                                           begin_ts,
                                           commit_ts_,
                                           txn->TxnID(),
                                           txn_id_);
        UnrecoverableError(error_message);
    }
    // Check if the entry is in committing process, because commit_ts of the base_entry is set in the Txn::CommitBottom
    return txn_mgr->CheckIfCommitting(txn_id_, begin_ts);
}

} // namespace hybridsearch
