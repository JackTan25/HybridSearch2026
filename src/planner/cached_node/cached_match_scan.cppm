

module;

export module cached_match_scan;

import stl;
import cached_scan_base;
import logical_match_scan_base;
import physical_filter_scan_base;
import base_expression;
import logical_match_tensor_scan;

namespace hybridsearch {

class LogicalKnnScan;
class PhysicalKnnScan;
class LogicalMatchSparseScan;
class PhysicalMatchSparseScan;
class PhysicalMatchTensorScan;
class PhysicalMergeKnn;
class PhysicalMergeMatchSparse;
class PhysicalMergeMatchTensor;

export class CachedMatchScanBase : public CachedScanBase {
public:
    CachedMatchScanBase(TxnTimeStamp query_ts, const LogicalMatchScanBase *logical_match_scan);

    CachedMatchScanBase(TxnTimeStamp query_ts, const PhysicalKnnScan *physical_knn_scan);

    CachedMatchScanBase(TxnTimeStamp query_ts, const PhysicalMatchSparseScan *physical_sparse_scan);

    CachedMatchScanBase(TxnTimeStamp query_ts, const PhysicalMatchTensorScan *physical_tensor_scan);

    CachedMatchScanBase(TxnTimeStamp query_ts, const PhysicalMergeKnn *physical_merge_knn);

    CachedMatchScanBase(TxnTimeStamp query_ts, const PhysicalMergeMatchSparse *physical_merge_match_sparse);

    CachedMatchScanBase(TxnTimeStamp query_ts, const PhysicalMergeMatchTensor *physical_merge_match_tensor);

    u64 Hash() const override;

    bool Eq(const CachedNodeBase &other) const override;

    const BaseExpression *query_expression() const { return query_expression_.get(); }

private:
    SharedPtr<BaseExpression> query_expression_{};
    SharedPtr<BaseExpression> filter_expression_{};
};

export class CachedKnnScan final : public CachedMatchScanBase {
public:
    CachedKnnScan(TxnTimeStamp query_ts, const LogicalKnnScan *logical_knn_scan);

    CachedKnnScan(TxnTimeStamp query_ts, const PhysicalKnnScan *physical_knn_scan);

    CachedKnnScan(TxnTimeStamp query_ts, const PhysicalMergeKnn *physical_merge_knn);
};

export class CachedMatchSparseScan final : public CachedMatchScanBase {
public:
    CachedMatchSparseScan(TxnTimeStamp query_ts, const LogicalMatchSparseScan *logical_sparse_scan);

    CachedMatchSparseScan(TxnTimeStamp query_ts, const PhysicalMatchSparseScan *physical_sparse_scan);

    CachedMatchSparseScan(TxnTimeStamp query_ts, const PhysicalMergeMatchSparse *physical_merge_match_sparse);
};

export class CachedMatchTensorScan final : public CachedMatchScanBase {
public:
    CachedMatchTensorScan(TxnTimeStamp query_ts, const LogicalMatchTensorScan *logical_tensor_scan);

    CachedMatchTensorScan(TxnTimeStamp query_ts, const PhysicalMatchTensorScan *physical_tensor_scan);

    CachedMatchTensorScan(TxnTimeStamp query_ts, const PhysicalMergeMatchTensor *physical_merge_match_tensor);

    u64 Hash() const override;

    bool Eq(const CachedNodeBase &other) const override;

private:
    u32 topn_;
    SharedPtr<MatchTensorScanIndexOptions> index_options_;
};

} // namespace hybridsearch
