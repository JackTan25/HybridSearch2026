

module;

#include <vector>

module cached_scan_base;

import base_table_ref;
import physical_scan_base;

namespace hybridsearch {

CachedScanBase::CachedScanBase(LogicalNodeType type,
                               const BaseTableRef *base_table_ref,
                               TxnTimeStamp query_ts,
                               SharedPtr<Vector<String>> output_names)
    : CachedNodeBase(type, output_names), schema_name_(base_table_ref->db_name()), table_name_(base_table_ref->table_name()),
      column_ids_(base_table_ref->column_ids_), query_ts_(query_ts) {}

CachedScanBase::CachedScanBase(LogicalNodeType type, const PhysicalScanBase *physical_scan_base, TxnTimeStamp query_ts)
    : CachedNodeBase(type, physical_scan_base->GetOutputNames()), schema_name_(physical_scan_base->base_table_ref_->db_name()),
      table_name_(physical_scan_base->base_table_ref_->table_name()), column_ids_(physical_scan_base->base_table_ref_->column_ids_),
      query_ts_(query_ts) {}

u64 CachedScanBase::Hash() const {
    u64 h = CachedNodeBase::Hash();
    h = h ^ std::hash<String>{}(*schema_name_);
    h = h ^ std::hash<String>{}(*table_name_);
    h = h ^ std::hash<TxnTimeStamp>{}(query_ts_);
    return h;
}

bool CachedScanBase::Eq(const CachedNodeBase &other_base) const {
    if (!CachedNodeBase::Eq(other_base)) {
        return false;
    }
    const auto &other = static_cast<const CachedScanBase &>(other_base);
    return *schema_name_ == *other.schema_name_ && *table_name_ == *other.table_name_ && query_ts_ == other.query_ts_;
}

} // namespace hybridsearch