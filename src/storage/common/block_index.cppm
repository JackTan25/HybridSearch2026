

module;

import stl;
import global_block_id;

export module block_index;

namespace hybridsearch {

struct SegmentEntry;
struct BlockEntry;
struct TableIndexEntry;
struct SegmentIndexEntry;
class Txn;

export struct SegmentSnapshot {
    SegmentEntry *segment_entry_{};

    Vector<BlockEntry *> block_map_;

    SegmentOffset segment_offset_ = 0;
};

export struct BlockIndex {
public:
    void Insert(SegmentEntry *segment_entry, Txn *txn);

    inline SizeT BlockCount() const {
        SizeT count = 0;
        for (const auto &[_, segment_info] : segment_block_index_) {
            count += segment_info.block_map_.size();
        }
        return count;
    }

    inline SizeT SegmentCount() const { return segment_block_index_.size(); }

    BlockEntry *GetBlockEntry(u32 segment_id, u16 block_id) const;

    SegmentOffset GetSegmentOffset(SegmentID segment_id) const;

    BlockOffset GetBlockOffset(SegmentID segment_id, BlockID block_id) const;

    bool IsEmpty() const { return segment_block_index_.empty(); }

public:
    Map<SegmentID, SegmentSnapshot> segment_block_index_;
};

export struct IndexSnapshot {
    TableIndexEntry *table_index_entry_;

    Map<SegmentID, SegmentIndexEntry *> segment_index_entries_;
};

export struct IndexIndex {
public:
    SharedPtr<IndexSnapshot> Insert(TableIndexEntry *table_index_entry, Txn *txn);

    void Insert(String index_name, SharedPtr<IndexSnapshot> index_snapshot);

    void Insert(TableIndexEntry *table_index_entry, SegmentIndexEntry *segment_index_entry);

    bool IsEmpty() const { return index_snapshots_.empty(); }

public:
    HashMap<String, SharedPtr<IndexSnapshot>> index_snapshots_;
    Vector<IndexSnapshot *> index_snapshots_vec_;
};

} // namespace hybridsearch
