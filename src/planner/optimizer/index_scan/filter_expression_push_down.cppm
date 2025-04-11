

module;

export module filter_expression_push_down;

import stl;
import query_context;
import base_expression;
import base_table_ref;
import index_base;
import fast_rough_filter;
import roaring_bitmap;
import txn;

namespace hybridsearch {

// TODO: equivalent expression rewrite optimization

// TODO: now do not support "not" expression in index scan

// 1. secondary index
// 2. filter_fulltext
// 3. AND, OR
export struct IndexFilterEvaluator {
    enum class Type {
        kAllTrue,
        kAllFalse,
        kSecondaryIndex,
        kFulltextIndex,
        kAnd,
        kOr,
        kInvalid,
    };

    Type type() const { return type_; }
    virtual ~IndexFilterEvaluator() = default;
    [[nodiscard]] virtual Bitmask Evaluate(SegmentID segment_id, SegmentOffset segment_row_count, Txn *txn) const = 0;

protected:
    Type type_;
    explicit IndexFilterEvaluator(const Type type) : type_(type) {}
};

export struct IndexScanFilterExpressionPushDownResult {
    SharedPtr<BaseExpression> index_filter_;
    SharedPtr<BaseExpression> leftover_filter_;

    UniquePtr<IndexFilterEvaluator> index_filter_evaluator_;
};

export class FilterExpressionPushDown {
public:
    static IndexScanFilterExpressionPushDownResult
    PushDownToIndexScan(QueryContext *query_context, const BaseTableRef *base_table_ref_ptr, const SharedPtr<BaseExpression> &expression);

    static void BuildFilterFulltextExpression(QueryContext *query_context,
                                              const BaseTableRef *base_table_ref_ptr,
                                              const Vector<SharedPtr<BaseExpression>> &expressions);

    static UniquePtr<FastRoughFilterEvaluator> PushDownToFastRoughFilter(SharedPtr<BaseExpression> &expression);
};

} // namespace hybridsearch