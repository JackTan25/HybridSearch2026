

module;

export module cached_index_scan;

import stl;
import cached_scan_base;
import base_expression;

namespace hybridsearch {

class LogicalIndexScan;
class PhysicalIndexScan;

export class CachedIndexScan final : public CachedScanBase {
public:
    CachedIndexScan(TxnTimeStamp query_ts, const LogicalIndexScan *logical_index_scan);

    CachedIndexScan(TxnTimeStamp query_ts, const PhysicalIndexScan *physical_index_scan);

    u64 Hash() const override;

    bool Eq(const CachedNodeBase &other) const override;

private:
    SharedPtr<BaseExpression> filter_expression_;
};

} // namespace hybridsearch
