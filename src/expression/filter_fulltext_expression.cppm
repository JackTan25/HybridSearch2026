

module;
export module filter_fulltext_expression;
import base_expression;
import data_type;
import stl;
import function_expr;
import filter_expression_push_down;
import roaring_bitmap;
import txn;
import block_index;

namespace hybridsearch {

export class FilterFulltextExpression final : public BaseExpression {
public:
    FilterFulltextExpression(String fields, String matching_text, String options_text);

    static SharedPtr<FilterFulltextExpression> BuildFilterFulltextExpression(const FunctionExpr &expr);

    String ToString() const override;

    DataType Type() const override;

    u64 Hash() const override;

    bool Eq(const BaseExpression &other_base) const override;

    String fields_;
    String matching_text_;
    String options_text_;

    // for expression_evaluator
    Txn *txn_ = nullptr;
    SharedPtr<BlockIndex> block_index_;
    UniquePtr<IndexFilterEvaluator> filter_fulltext_evaluator_;
    Map<SegmentID, Bitmask> segment_results_;
    std::shared_mutex rw_mutex_;
};

} // namespace hybridsearch
