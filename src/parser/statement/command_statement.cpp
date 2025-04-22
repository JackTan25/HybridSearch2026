

#include "command_statement.h"

namespace hybridsearch {

std::string UseCmd::ToString() const { return "Use Command"; }
std::string SetCmd::ToString() const { return "Set Command"; }
std::string ExportCmd::ToString() const { return "Export Command"; }
std::string CheckTable::ToString() const { return "Check Table Command"; }
std::string CommandStatement::ToString() const { return command_info_->ToString(); }
std::string LockCmd::ToString() const { return "Lock Command"; }
std::string UnlockCmd::ToString() const { return "Unlock Command"; }
std::string CleanupCmd::ToString() const { return "Cleanup Command"; }
std::string TestCmd::ToString() const { return "Test Command: " + command_content_; }
std::string SnapshotCmd::ToString() const { return "Snapshot command"; }

} // namespace hybridsearch