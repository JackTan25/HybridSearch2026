

module;

export module physical_command;

import stl;
import internal_types;
import physical_operator;
import physical_operator_type;
import query_context;

import operator_state;
import load_meta;
import hybridsearch_exception;
import table_entry;
import command_statement;
import data_type;
import logger;

namespace hybridsearch {

export class PhysicalCommand final : public PhysicalOperator {
public:
    explicit PhysicalCommand(u64 id,
                             SharedPtr<CommandInfo> command_info,
                             SharedPtr<Vector<String>> output_names,
                             SharedPtr<Vector<SharedPtr<DataType>>> output_types,
                             SharedPtr<Vector<LoadMeta>> load_metas)
        : PhysicalOperator(PhysicalOperatorType::kCommand, nullptr, nullptr, id, load_metas), command_info_(std::move(command_info)),
          output_names_(std::move(output_names)), output_types_(std::move(output_types)) {}

    ~PhysicalCommand() override = default;

    void Init(QueryContext* query_context) override;

    bool Execute(QueryContext *query_context, OperatorState *operator_state) override;

    inline SharedPtr<Vector<String>> GetOutputNames() const override { return output_names_; }

    inline SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const override { return output_types_; }

public:
    const SharedPtr<CommandInfo> command_info_{};

    const SharedPtr<Vector<String>> output_names_{};
    const SharedPtr<Vector<SharedPtr<DataType>>> output_types_{};
};
} // namespace hybridsearch