

module;

export module cached_scan_base;

import stl;
import cached_node_base;
import logical_node_type;

namespace hybridsearch {

class BaseTableRef;
class PhysicalScanBase;

export class CachedScanBase : public CachedNodeBase {
public:
    CachedScanBase(LogicalNodeType type, const BaseTableRef *base_table_ref, TxnTimeStamp query_ts, SharedPtr<Vector<String>> output_names);

    CachedScanBase(LogicalNodeType type, const PhysicalScanBase *physical_scan_base, TxnTimeStamp query_ts);

    u64 Hash() const override;

    bool Eq(const CachedNodeBase &other) const override;

    const String &schema_name() const { return *schema_name_; }
    const String &table_name() const { return *table_name_; }

protected:
    SharedPtr<String> schema_name_{};
    SharedPtr<String> table_name_{};
    Vector<ColumnID> column_ids_{};
    TxnTimeStamp query_ts_{};
};

} // namespace hybridsearch