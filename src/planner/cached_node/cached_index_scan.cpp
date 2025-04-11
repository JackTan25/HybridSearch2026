

module;

module cached_index_scan;

import logical_index_scan;
import physical_index_scan;
import logical_node_type;

namespace hybridsearch {

CachedIndexScan::CachedIndexScan(TxnTimeStamp query_ts, const LogicalIndexScan *logical_index_scan)
    : CachedScanBase(logical_index_scan->operator_type(), logical_index_scan->base_table_ref_.get(), query_ts, logical_index_scan->GetOutputNames()),
      filter_expression_(logical_index_scan->index_filter_) {}

CachedIndexScan::CachedIndexScan(TxnTimeStamp query_ts, const PhysicalIndexScan *physical_index_scan)
    : CachedScanBase(LogicalNodeType::kIndexScan, physical_index_scan, query_ts), filter_expression_(physical_index_scan->FilterExpression()) {}

u64 CachedIndexScan::Hash() const {
    u64 h = 0;
    h ^= CachedScanBase::Hash();
    h ^= filter_expression_->Hash();
    return h;
}

bool CachedIndexScan::Eq(const CachedNodeBase &other_base) const {
    if (!CachedScanBase::Eq(other_base)) {
        return false;
    }
    if (type() != other_base.type()) {
        return false;
    }
    const auto &other = static_cast<const CachedIndexScan &>(other_base);
    return filter_expression_->Eq(*other.filter_expression_);
}

} // namespace hybridsearch
