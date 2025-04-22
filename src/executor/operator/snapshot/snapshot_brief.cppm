

module;

export module snapshot_brief;

import stl;
import command_statement;
import status;

namespace hybridsearch {

export struct SnapshotBrief {
    String snapshot_name_; // snapshot_name_
    SnapshotScope scope_{SnapshotScope::kInvalid};  // system / db / table snapshot
    String create_time_;      // when create the snapshot
    u64 commit_ts_;        // txn ts the snapshot created.
    u64 size_;             // total snapshot size

    static Vector<SnapshotBrief> GetSnapshots(const String& dir);
};

} // namespace hybridsearch
