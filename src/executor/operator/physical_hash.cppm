

module;

export module physical_hash;

import stl;

import query_context;
import operator_state;
import physical_operator;
import physical_operator_type;
import load_meta;
import hybridsearch_exception;
import internal_types;
import data_type;
import logger;

namespace hybridsearch {

export class PhysicalHash final : public PhysicalOperator {
public:
    explicit PhysicalHash(SharedPtr<Vector<String>> output_names,
                          SharedPtr<Vector<SharedPtr<DataType>>> output_types,
                          u64 id,
                          SharedPtr<Vector<LoadMeta>> load_metas)
        : PhysicalOperator(PhysicalOperatorType::kHash, nullptr, nullptr, id, load_metas), output_names_(std::move(output_names)),
          output_types_(std::move(output_types)) {}

    ~PhysicalHash() override = default;

    void Init(QueryContext* query_context) override;

    bool Execute(QueryContext *query_context, OperatorState *operator_state) final;

    inline SharedPtr<Vector<String>> GetOutputNames() const final { return output_names_; }

    inline SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final { return output_types_; }

private:
    SharedPtr<Vector<String>> output_names_{};
    SharedPtr<Vector<SharedPtr<DataType>>> output_types_{};
};

} // namespace hybridsearch
