

module;

export module update_segment_bloom_filter_task;

import stl;
import bg_task;
import segment_entry;
import table_entry;
import txn;
import txn_manager;

namespace hybridsearch {

export class UpdateSegmentBloomFilterTask final : public BGTask {
public:
    static void CreateAndSubmitTask(SegmentEntry *segment_entry, TableEntry *table_entry, TxnManager *txn_mgr);

    explicit UpdateSegmentBloomFilterTask(SegmentEntry *segment, TableEntry *table_entry, TxnManager *txn_mgr)
        : BGTask(BGTaskType::kUpdateSegmentBloomFilterData, false), segment_(segment), table_entry_(table_entry), txn_mgr_(txn_mgr) {}

    String ToString() const override { return "UpdateSegmentBloomFilterTask"; }

    void Execute();

    static void ExecuteInner(SegmentEntry *segment, TxnManager *txn_mgr);

private:
    SegmentEntry *segment_{};
    TableEntry *table_entry_{};
    TxnManager *txn_mgr_{};
};

} // namespace hybridsearch
