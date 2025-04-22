

module;

export module physical_unnest;

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

export class PhysicalUnnest : public PhysicalOperator {
public:
    explicit PhysicalUnnest(u64 id,
                            UniquePtr<PhysicalOperator> left,
                            Vector<SharedPtr<BaseExpression>> expression_list,
                            SharedPtr<Vector<LoadMeta>> load_metas)
        : PhysicalOperator(PhysicalOperatorType::kUnnest, std::move(left), nullptr, id, load_metas), expression_list_(std::move(expression_list)) {}

    ~PhysicalUnnest() override = default;

    void Init(QueryContext* query_context) override;

    bool Execute(QueryContext *query_context, OperatorState *operator_state) final;

    SharedPtr<Vector<String>> GetOutputNames() const final;

    SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final;

    SizeT TaskletCount() override { return left_->TaskletCount(); }

    Vector<SharedPtr<BaseExpression>> expression_list() const { return expression_list_; }

private:
    SizeT GetUnnestIdx() const;

private:
    Vector<SharedPtr<BaseExpression>> expression_list_;

    SharedPtr<DataTable> input_table_{};
};

} // namespace hybridsearch
