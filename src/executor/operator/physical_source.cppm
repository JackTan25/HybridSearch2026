

module;

export module physical_source;

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

export enum class SourceType {
    kInvalid,
    kTable,
    kLocalQueue,
    kEmpty,
    kRemote,
};

export String ToString(SourceType source_type);

export class PhysicalSource final : public PhysicalOperator {
public:
    explicit PhysicalSource(u64 id,
                            SourceType source_type,
                            SharedPtr<Vector<String>> names,
                            SharedPtr<Vector<SharedPtr<DataType>>> types,
                            SharedPtr<Vector<LoadMeta>> load_metas)
        : PhysicalOperator(PhysicalOperatorType::kSource, nullptr, nullptr, id, load_metas), output_names_(std::move(names)),
          output_types_(std::move(types)), type_(source_type) {}

    ~PhysicalSource() override = default;

    void Init(QueryContext* query_context) override;

    bool Execute(QueryContext *query_context, OperatorState *operator_state) final;

    bool Execute(QueryContext *query_context, SourceState *source_state);

    inline SharedPtr<Vector<String>> GetOutputNames() const final { return output_names_; }

    inline SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final { return output_types_; }

    inline SourceType source_type() const { return type_; }

private:
    SharedPtr<Vector<String>> output_names_{};
    SharedPtr<Vector<SharedPtr<DataType>>> output_types_{};
    SourceType type_{SourceType::kInvalid};
};

} // namespace hybridsearch
