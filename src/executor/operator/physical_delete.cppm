

module;

export module physical_delete;

import stl;

import query_context;
import operator_state;
import physical_operator;
import physical_operator_type;
import meta_info;
import load_meta;
import hybridsearch_exception;
import internal_types;
import data_type;
import logger;

namespace hybridsearch {

export class PhysicalDelete final : public PhysicalOperator {
public:
    explicit PhysicalDelete(u64 id, UniquePtr<PhysicalOperator> left, SharedPtr<TableInfo> table_info, SharedPtr<Vector<LoadMeta>> load_metas)
        : PhysicalOperator(PhysicalOperatorType::kDelete, std::move(left), nullptr, id, load_metas), table_info_(std::move(table_info)) {}

    ~PhysicalDelete() override = default;

    void Init(QueryContext *query_context) override;

    bool Execute(QueryContext *query_context, OperatorState *operator_state) final;

    inline SharedPtr<Vector<String>> GetOutputNames() const final { return output_names_; }

    inline SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final { return output_types_; }

    SharedPtr<TableInfo> table_info_{};

private:
    SharedPtr<Vector<String>> output_names_{};
    SharedPtr<Vector<SharedPtr<DataType>>> output_types_{};
};

} // namespace hybridsearch
