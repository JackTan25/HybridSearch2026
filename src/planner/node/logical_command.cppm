

module;

export module logical_command;

import stl;
import logical_node_type;
import column_binding;
import logical_node;

import internal_types;
import command_statement;
import data_type;

namespace hybridsearch {

struct TableEntry;

export class LogicalCommand : public LogicalNode {
public:
    LogicalCommand(u64 node_id, SharedPtr<CommandInfo> command_info)
        : LogicalNode(node_id, LogicalNodeType::kCommand), command_info_(std::move(command_info)) {}

    [[nodiscard]] Vector<ColumnBinding> GetColumnBindings() const final;

    [[nodiscard]] SharedPtr<CommandInfo> command_info() const { return command_info_; }

    [[nodiscard]] SharedPtr<Vector<String>> GetOutputNames() const final;

    [[nodiscard]] SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final;

    String ToString(i64 &space) const final;

    inline String name() final { return "LogicalCommand"; }

private:
    SharedPtr<CommandInfo> command_info_{};
};

} // namespace hybridsearch
