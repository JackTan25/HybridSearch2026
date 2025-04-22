

module;

export module physical_flush;

import stl;

import query_context;
import operator_state;
import physical_operator;
import physical_operator_type;
import base_expression;
import load_meta;
import hybridsearch_exception;
import internal_types;
import flush_statement;
import data_type;
import logger;

namespace hybridsearch {

export class PhysicalFlush final : public PhysicalOperator {
public:
    explicit PhysicalFlush(FlushType flush_type, u64 id, SharedPtr<Vector<LoadMeta>> load_metas)
        : PhysicalOperator(PhysicalOperatorType::kFlush, nullptr, nullptr, id, load_metas), flush_type_(flush_type) {}

    ~PhysicalFlush() override = default;

    void Init(QueryContext* query_context) override;

    bool Execute(QueryContext *query_context, OperatorState *operator_state) final;

    inline SharedPtr<Vector<String>> GetOutputNames() const final { return output_names_; }

    inline SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final { return output_types_; }

    inline FlushType flush_type() const { return flush_type_; }

private:
    void FlushData(QueryContext *query_context, OperatorState *operator_state);

    void FlushLog(QueryContext *query_context, OperatorState *operator_state);

    void FlushBuffer(QueryContext *query_context, OperatorState *operator_state);

private:
    FlushType flush_type_;

    SharedPtr<Vector<String>> output_names_{};
    SharedPtr<Vector<SharedPtr<DataType>>> output_types_{};
};

} // namespace hybridsearch
