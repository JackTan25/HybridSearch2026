

module;

export module secondary_index_in_mem;

import stl;
import roaring_bitmap;
import internal_types;
import column_def;
import table_index_entry;
import base_memindex;
import memindex_tracer;

namespace hybridsearch {

struct BlockColumnEntry;
class BufferManager;
struct ChunkIndexEntry;
struct SegmentIndexEntry;

export class SecondaryIndexInMem : public BaseMemIndex {
protected:
    SegmentIndexEntry *segment_index_entry_ = nullptr;

    explicit SecondaryIndexInMem(SegmentIndexEntry *segment_index_entry) : segment_index_entry_(segment_index_entry) {}

    virtual u32 GetRowCountNoLock() const = 0;

    virtual u32 MemoryCostOfEachRow() const = 0;

    virtual u32 MemoryCostOfThis() const = 0;

public:
    virtual ~SecondaryIndexInMem() = default;

    MemIndexTracerInfo GetInfo() const override;

    TableIndexEntry *table_index_entry() const override;

    virtual u32 GetRowCount() const = 0;

    virtual void InsertBlockData(SegmentOffset block_offset,
                                 BlockColumnEntry *block_column_entry,
                                 BufferManager *buffer_manager,
                                 u32 row_offset,
                                 u32 row_count) = 0;

    virtual SharedPtr<ChunkIndexEntry> Dump(SegmentIndexEntry *segment_index_entry, BufferManager *buffer_mgr) const = 0;

    virtual Pair<u32, Bitmask> RangeQuery(const void *input) const = 0;

    static SharedPtr<SecondaryIndexInMem>
    NewSecondaryIndexInMem(const SharedPtr<ColumnDef> &column_def, SegmentIndexEntry *segment_index_entry, RowID begin_row_id);
};

} // namespace hybridsearch