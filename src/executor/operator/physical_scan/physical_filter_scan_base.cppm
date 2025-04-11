

module;

export module physical_filter_scan_base;

import stl;
import physical_scan_base;
import common_query_filter;
import physical_operator_type;
import base_table_ref;
import load_meta;
import roaring_bitmap;

namespace hybridsearch {

export class PhysicalFilterScanBase : public PhysicalScanBase {
public:
    PhysicalFilterScanBase(const u64 id,
                           const PhysicalOperatorType type,
                           UniquePtr<PhysicalOperator> left,
                           UniquePtr<PhysicalOperator> right,
                           const u64 table_index,
                           SharedPtr<BaseTableRef> base_table_ref,
                           SharedPtr<CommonQueryFilter> common_query_filter,
                           SharedPtr<Vector<LoadMeta>> load_metas)
        : PhysicalScanBase(id, type, std::move(left), std::move(right), table_index, std::move(base_table_ref), std::move(load_metas)),
          common_query_filter_(std::move(common_query_filter)) {}

    bool CalculateFilterBitmask(SegmentID segment_id, BlockID block_id, BlockOffset row_count, Bitmask &bitmask) const;

public:
    // for filter
    SharedPtr<CommonQueryFilter> common_query_filter_;
};

} // namespace hybridsearch