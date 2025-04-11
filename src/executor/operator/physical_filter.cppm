

module;

export module physical_filter;

import stl;

import query_context;
import operator_state;
import physical_operator;
import physical_operator_type;
import base_expression;
import data_table;
import load_meta;
import hybridsearch_exception;
import internal_types;
import data_type;

namespace hybridsearch {

export class PhysicalFilter : public PhysicalOperator {
public:
    explicit PhysicalFilter(u64 id, UniquePtr<PhysicalOperator> left, SharedPtr<BaseExpression> condition, SharedPtr<Vector<LoadMeta>> load_metas)
        : PhysicalOperator(PhysicalOperatorType::kFilter, std::move(left), nullptr, id, load_metas), condition_(std::move(condition)) {}

    ~PhysicalFilter() override = default;

    void Init(QueryContext* query_context) override;

    bool Execute(QueryContext *query_context, OperatorState *operator_state) final;

    inline SharedPtr<Vector<String>> GetOutputNames() const final { return PhysicalCommonFunctionUsingLoadMeta::GetOutputNames(*this); }

    inline SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final { return PhysicalCommonFunctionUsingLoadMeta::GetOutputTypes(*this); }

    SizeT TaskletCount() override { return left_->TaskletCount(); }

    inline const SharedPtr<BaseExpression> &condition() const { return condition_; }

private:
    SharedPtr<BaseExpression> condition_;

    SharedPtr<DataTable> input_table_{};
};

} // namespace hybridsearch
