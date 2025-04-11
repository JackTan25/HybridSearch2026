

module;

export module physical_sort;

import stl;

import query_context;
import operator_state;
import expression_evaluator;
import physical_operator;
import physical_operator_type;
import base_expression;
import data_table;
import data_block;
import load_meta;
import hybridsearch_exception;
import physical_top;
import internal_types;
import select_statement;
import data_type;

namespace hybridsearch {

export class PhysicalSort : public PhysicalOperator {
public:
    explicit PhysicalSort(u64 id,
                          UniquePtr<PhysicalOperator> left,
                          Vector<SharedPtr<BaseExpression>> expressions,
                          Vector<OrderType> order_by_types,
                          SharedPtr<Vector<LoadMeta>> load_metas)
        : PhysicalOperator(PhysicalOperatorType::kSort, std::move(left), nullptr, id, load_metas), expressions_(std::move(expressions)),
          order_by_types_(std::move(order_by_types)) {}

    ~PhysicalSort() override = default;

    void Init(QueryContext* query_context) override;

    bool Execute(QueryContext *query_context, OperatorState *operator_state) final;

    inline SharedPtr<Vector<String>> GetOutputNames() const final { return PhysicalCommonFunctionUsingLoadMeta::GetOutputNames(*this); }

    inline SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final { return PhysicalCommonFunctionUsingLoadMeta::GetOutputTypes(*this); }

    SizeT TaskletCount() override { return left_->TaskletCount(); }

    // for OperatorState
    inline auto const &GetSortExpressions() const { return expressions_; }

    Vector<SharedPtr<BaseExpression>> expressions_;
    Vector<OrderType> order_by_types_{};

private:
    u64 input_table_index_{};
    CompareTwoRowAndPreferLeft prefer_left_function_; // compare function
};

} // namespace hybridsearch
