

module;

#include <filesystem>

module snapshot;

import stl;
import txn;
import query_context;
import table_entry;
import status;
import third_party;
import config;
import virtual_store;
import logger;

namespace hybridsearch {

Status Snapshot::DropSnapshot(QueryContext *query_context, const String &snapshot_name) {

    String snapshot_dir = query_context->global_config()->SnapshotDir();
    bool found = false;
    for (const auto &entry : std::filesystem::directory_iterator(snapshot_dir)) {
        if (entry.is_directory()) {
            // Don't search the directory recursively
        } else {
            // Just the file base name
            if (entry.path().stem() == snapshot_name) {
                String extension = entry.path().extension();
                if (extension == ".json" or extension == ".lz4") {
                    LOG_INFO(fmt::format("Delete file: {}", entry.path().string()));
                    VirtualStore::DeleteFile(entry.path().string());
                    found = true;
                }
            } else {
                String filename = entry.path().filename();
                LOG_WARN(fmt::format("Invalid snapshot file name: {}", filename));
            }
        }
    }

    if (!found) {
        return Status::NotFound(fmt::format("Snapshot: {} not found", snapshot_name));
    }

    return Status::OK();
}

} // namespace hybridsearch
