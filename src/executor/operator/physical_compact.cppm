

module;

export module physical_compact;

import stl;
import physical_operator;
import physical_operator_type;
import base_table_ref;
import query_context;
import operator_state;
import load_meta;
import data_type;
import segment_entry;
import compact_statement;

namespace hybridsearch {

export class PhysicalCompact : public PhysicalOperator {
public:
    PhysicalCompact(u64 id,
                    SharedPtr<BaseTableRef> base_table_ref,
                    CompactStatementType compact_type,
                    SharedPtr<Vector<String>> output_names,
                    SharedPtr<Vector<SharedPtr<DataType>>> output_types,
                    SharedPtr<Vector<LoadMeta>> load_metas)
        : PhysicalOperator(PhysicalOperatorType::kCompact, nullptr, nullptr, id, load_metas), base_table_ref_(base_table_ref),
          compact_type_(compact_type), output_names_(output_names), output_types_(output_types) {}

    ~PhysicalCompact() override = default;

    void Init(QueryContext* query_context) override;

    bool Execute(QueryContext *query_context, OperatorState *operator_state) final;

    SizeT TaskletCount() override { return compactible_segments_group_.size(); }

    Vector<Vector<Vector<SegmentEntry *>>> PlanCompact(SizeT parallel_count);

    SharedPtr<Vector<String>> GetOutputNames() const override { return output_names_; }

    SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const override { return output_types_; }

private:
    SharedPtr<BaseTableRef> base_table_ref_;
    CompactStatementType compact_type_;
    Vector<Vector<SegmentEntry *>> compactible_segments_group_;

    SharedPtr<Vector<String>> output_names_{};
    SharedPtr<Vector<SharedPtr<DataType>>> output_types_{};
};

} // namespace hybridsearch