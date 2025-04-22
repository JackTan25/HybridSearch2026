


module;

#include <vector>

module physical_filter_scan_base;

import stl;
import common_query_filter;
import base_table_ref;
import roaring_bitmap;
import default_values;

namespace hybridsearch {

bool PhysicalFilterScanBase::CalculateFilterBitmask(SegmentID segment_id, BlockID block_id, BlockOffset row_count, Bitmask &bitmask) const {
    bitmask = Bitmask(row_count);
    if (common_query_filter_->AlwaysTrue()) {
        return true;
    }
    auto it_filter = common_query_filter_->filter_result_.find(segment_id);
    if (it_filter == common_query_filter_->filter_result_.end()) {
        bitmask.SetAllFalse();
        return false;
    }
    // not skipped after common_query_filter
    const Bitmask &filter_result = it_filter->second;
    if (filter_result.IsAllTrue()) {
        return true;
    }
    bitmask.SetAllFalse();
    const u32 block_start_offset = block_id * DEFAULT_BLOCK_CAPACITY;
    const u32 block_end_offset = block_start_offset + row_count;
    Bitmask middle_bitmask(filter_result.count());
    middle_bitmask.SetAllFalse();
    middle_bitmask.SetTrueRange(block_start_offset, block_end_offset);
    middle_bitmask.MergeAnd(filter_result);
    middle_bitmask.RoaringBitmapApplyFunc([&](const u32 offset) {
        bitmask.SetTrue(offset - block_start_offset);
        return true;
    });
    bitmask.RunOptimize();
    return true;
}

} // namespace hybridsearch
