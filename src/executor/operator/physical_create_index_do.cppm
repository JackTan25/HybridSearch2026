

module;

export module physical_create_index_do;

import stl;

import physical_operator_type;
import physical_operator;
import query_context;
import operator_state;
import load_meta;

import index_base;
import base_table_ref;
import internal_types;
import data_type;

namespace hybridsearch {

export class PhysicalCreateIndexDo : public PhysicalOperator {
public:
    PhysicalCreateIndexDo(u64 id,
                          UniquePtr<PhysicalOperator> left,
                          SharedPtr<BaseTableRef> base_table_ref,
                          SharedPtr<String> index_name,
                          SharedPtr<Vector<String>> output_names,
                          SharedPtr<Vector<SharedPtr<DataType>>> output_types,
                          SharedPtr<Vector<LoadMeta>> load_metas);

public:
    void Init(QueryContext* query_context) override;

    bool Execute(QueryContext *query_context, OperatorState *operator_state) override;

    SizeT TaskletCount() override { return 0; }

    SharedPtr<Vector<String>> GetOutputNames() const override { return output_names_; }

    SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const override { return output_types_; }

public:
    // for create fragemnt context
    const SharedPtr<BaseTableRef> base_table_ref_{};
    const SharedPtr<String> index_name_{};

    const SharedPtr<Vector<String>> output_names_{};
    const SharedPtr<Vector<SharedPtr<DataType>>> output_types_{};
};

} // namespace hybridsearch