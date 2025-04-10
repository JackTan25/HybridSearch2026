

module;

export module data_access_state;

import stl;
import data_block;
import column_vector;

import internal_types;

namespace hybridsearch {

struct SegmentEntry;

export struct AppendRange {
    inline explicit AppendRange(u32 segment_id, u16 block_id, u16 start_offset, u16 row_count)
        : segment_id_(segment_id), block_id_(block_id), start_offset_(start_offset), row_count_(row_count) {}

    SegmentID segment_id_;
    BlockID block_id_;
    u16 start_offset_;
    u16 row_count_;
};

export struct AppendState {
    explicit AppendState(const Vector<SharedPtr<DataBlock>> &blocks) : blocks_(blocks), current_count_(0) {
        SizeT block_count = blocks.size();
        for (SizeT idx = 0; idx < block_count; ++idx) {
            total_count_ += blocks[idx]->row_count();
        }
    }

    const Vector<SharedPtr<DataBlock>> &blocks_{};
    SizeT total_count_{};
    SizeT current_count_{};

    u64 current_block_{}; // block count in append state may larger than u16::max, since these blocks may not be in one segment.
    u16 current_block_offset_{};

    Vector<AppendRange> append_ranges_{};

    [[nodiscard]] inline bool Finished() const { return current_count_ == total_count_; }
};

export struct ImportState {
    // Vector<void *> segments_ptr_;
    Vector<SharedPtr<SegmentEntry>> segment_entries_{};
};

export struct DeleteState {
    //    HashMap<u64, Vector<RowID>> rows_; // key is pair<segment_id, block_id>
    // HashMap<<segment, block_id>, block_offset>
    HashMap<SegmentID, HashMap<BlockID, Vector<BlockOffset>>> rows_; // use segment id, as the first level key, block id as the second level key
};

export struct GetState {};

export enum class ScanStateType {
    kTableScan,
    kIndexScan,
    kAnnScan,
};

export enum class ScanLocation {
    kLocal,
    kGlobal,
};

} // namespace hybridsearch
