

module;

export module cached_match;

import stl;
import cached_node_base;
import cached_scan_base;
import logical_node_type;

namespace hybridsearch {

class BaseTableRef;
class MatchExpression;
class BaseExpression;
class LogicalMatch;
class PhysicalMatch;

export class CachedMatch final : public CachedScanBase {
public:
    CachedMatch(TxnTimeStamp query_ts, LogicalMatch *logical_match);

    CachedMatch(TxnTimeStamp query_ts, PhysicalMatch *physical_match);

    u64 Hash() const override;

    bool Eq(const CachedNodeBase &other) const override;

private:
    SharedPtr<MatchExpression> match_expr_{};
    SharedPtr<BaseExpression> filter_expression_{};
    u32 topn_;
};

} // namespace hybridsearch
