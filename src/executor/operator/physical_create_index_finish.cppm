

module;

export module physical_create_index_finish;

import stl;

import physical_operator_type;
import physical_operator;
import query_context;
import operator_state;
import load_meta;
import index_base;
import internal_types;
import data_type;

namespace hybridsearch {
export class PhysicalCreateIndexFinish : public PhysicalOperator {
public:
    PhysicalCreateIndexFinish(u64 id,
                              UniquePtr<PhysicalOperator> left,
                              SharedPtr<String> db_name,
                              SharedPtr<String> table_name,
                              SharedPtr<IndexBase> index_base,
                              SharedPtr<Vector<String>> output_names,
                              SharedPtr<Vector<SharedPtr<DataType>>> output_types,
                              SharedPtr<Vector<LoadMeta>> load_metas);

public:
    void Init(QueryContext* query_context) override;

    bool Execute(QueryContext *query_context, OperatorState *operator_state) override;

    SharedPtr<Vector<String>> GetOutputNames() const override { return output_names_; }

    SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const override { return output_types_; }

public:
    const SharedPtr<String> db_name_{};
    const SharedPtr<String> table_name_{};
    const SharedPtr<IndexBase> index_base_{};

    const SharedPtr<Vector<String>> output_names_{};
    const SharedPtr<Vector<SharedPtr<DataType>>> output_types_{};
};

} // namespace hybridsearch