

module;

export module physical_cross_product;

import stl;

import query_context;
import operator_state;
import physical_operator;
import physical_operator_type;
import data_table;
import load_meta;
import hybridsearch_exception;
import internal_types;
import data_type;
import logger;

namespace hybridsearch {

export class PhysicalCrossProduct final : public PhysicalOperator {
public:
    explicit PhysicalCrossProduct(u64 id, UniquePtr<PhysicalOperator> left, UniquePtr<PhysicalOperator> right, SharedPtr<Vector<LoadMeta>> load_metas)
        : PhysicalOperator(PhysicalOperatorType::kCrossProduct, std::move(left), std::move(right), id, load_metas) {}

    ~PhysicalCrossProduct() override = default;

    void Init(QueryContext* query_context) override;

    bool Execute(QueryContext *query_context, OperatorState *operator_state) final;

    SharedPtr<Vector<String>> GetOutputNames() const final;

    SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final;

private:
    SharedPtr<DataTable> left_table_{};
    SharedPtr<DataTable> right_table_{};
};

} // namespace hybridsearch
