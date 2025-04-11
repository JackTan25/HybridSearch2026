

module;

export module physical_optimize;

import stl;

import query_context;
import operator_state;
import physical_operator;
import physical_operator_type;
import base_expression;
import load_meta;
import hybridsearch_exception;
import internal_types;
import optimize_statement;
import data_type;
import logger;
import statement_common;

namespace hybridsearch {

export class PhysicalOptimize final : public PhysicalOperator {
public:
    explicit PhysicalOptimize(u64 id,
                              String db_name,
                              String table_name,
                              String index_name,
                              Vector<UniquePtr<InitParameter>> opt_params,
                              SharedPtr<Vector<LoadMeta>> load_metas)
        : PhysicalOperator(PhysicalOperatorType::kOptimize, nullptr, nullptr, id, load_metas), db_name_(std::move(db_name)),
          table_name_(std::move(table_name)), index_name_(std::move(index_name)), opt_params_(std::move(opt_params)) {}

    ~PhysicalOptimize() override = default;

    void Init(QueryContext* query_context) override;

    bool Execute(QueryContext *query_context, OperatorState *operator_state) final;

    inline SharedPtr<Vector<String>> GetOutputNames() const final { return output_names_; }

    inline SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final { return output_types_; }

private:
    void OptimizeIndex(QueryContext *query_context, OperatorState *operator_state);

    void OptIndex(QueryContext *query_context, OperatorState *operator_state);

private:
    String db_name_{};
    String table_name_{};
    String index_name_{};
    Vector<UniquePtr<InitParameter>> opt_params_;

    SharedPtr<Vector<String>> output_names_{};
    SharedPtr<Vector<SharedPtr<DataType>>> output_types_{};
};

} // namespace hybridsearch
