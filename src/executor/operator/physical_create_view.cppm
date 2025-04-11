

module;

export module physical_create_view;

import stl;

import query_context;
import operator_state;
import physical_operator;
import physical_operator_type;
import index_base;
import load_meta;
import hybridsearch_exception;
import internal_types;
import create_view_info;
import data_type;
import logger;

namespace hybridsearch {

export class PhysicalCreateView final : public PhysicalOperator {
public:
    explicit inline PhysicalCreateView(u64 id,
                                       SharedPtr<Vector<String>> names_ptr,
                                       SharedPtr<Vector<SharedPtr<DataType>>> types_ptr,
                                       SharedPtr<CreateViewInfo> create_view_info,
                                       SharedPtr<Vector<LoadMeta>> load_metas)
        : PhysicalOperator(PhysicalOperatorType::kCreateView, nullptr, nullptr, id, load_metas), create_view_info_(std::move(create_view_info)),
          output_names_(std::move(names_ptr)), output_types_(std::move(types_ptr)) {}

    ~PhysicalCreateView() override = default;

    void Init(QueryContext* query_context) override;

    bool Execute(QueryContext *query_context, OperatorState *operator_state) final;

    inline const SharedPtr<CreateViewInfo> &bound_select_statement() const { return create_view_info_; };

    inline SharedPtr<Vector<String>> GetOutputNames() const final { return output_names_; }

    inline SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final { return output_types_; }

    inline const SharedPtr<CreateViewInfo> &create_view_info() const { return create_view_info_; }

private:
    SharedPtr<CreateViewInfo> create_view_info_{nullptr};

    SharedPtr<Vector<String>> output_names_{};
    SharedPtr<Vector<SharedPtr<DataType>>> output_types_{};
};

} // namespace hybridsearch
