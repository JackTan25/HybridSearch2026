

module;

export module physical_merge_top;

import stl;

import query_context;
import operator_state;
import expression_state;
import physical_operator;
import physical_operator_type;
import base_expression;
import load_meta;
import hybridsearch_exception;
import base_table_ref;
import physical_top;
import internal_types;
import select_statement;
import data_type;

namespace hybridsearch {

export class PhysicalMergeTop final : public PhysicalOperator {
public:
    explicit PhysicalMergeTop(u64 id,
                              SharedPtr<BaseTableRef> base_table_ref,
                              UniquePtr<PhysicalOperator> left,
                              u32 limit,
                              u32 offset,
                              Vector<SharedPtr<BaseExpression>> sort_expressions,
                              Vector<OrderType> order_by_types,
                              SharedPtr<Vector<LoadMeta>> load_metas)
        : PhysicalOperator(PhysicalOperatorType::kMergeTop, std::move(left), nullptr, id, load_metas), base_table_ref_(std::move(base_table_ref)),
          limit_(limit), offset_(offset), order_by_types_(std::move(order_by_types)), sort_expressions_(std::move(sort_expressions)) {}

    ~PhysicalMergeTop() override = default;

    void Init(QueryContext* query_context) override;

    bool Execute(QueryContext *query_context, OperatorState *operator_state) final;

    inline SharedPtr<Vector<String>> GetOutputNames() const final { return PhysicalCommonFunctionUsingLoadMeta::GetOutputNames(*this); }

    inline SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final { return PhysicalCommonFunctionUsingLoadMeta::GetOutputTypes(*this); }

    SizeT TaskletCount() override { return left_->TaskletCount(); }

    // for OperatorState and Explain
    inline auto const &GetSortExpressions() const { return sort_expressions_; }

    // for Explain
    inline auto const &GetOrderbyTypes() const { return order_by_types_; }

    // for Explain
    inline auto GetLimit() const { return limit_; }

    // for Explain
    inline auto GetOffset() const { return offset_; }

    // for InputLoad
    // necessary because MergeTop may be the first operator in a pipeline
    void FillingTableRefs(HashMap<SizeT, SharedPtr<BaseTableRef>> &table_refs) override {
        table_refs.insert({base_table_ref_->table_index_, base_table_ref_});
    }

private:
    SharedPtr<BaseTableRef> base_table_ref_;             // necessary for InputLoad
    u32 limit_{};                                        // limit value
    u32 offset_{};                                       // offset value
    u32 sort_expr_count_{};                              // number of expressions to sort
    Vector<OrderType> order_by_types_;                   // ASC or DESC
    Vector<SharedPtr<BaseExpression>> sort_expressions_; // expressions to sort
    CompareTwoRowAndPreferLeft prefer_left_function_;    // compare function
};

} // namespace hybridsearch
