

module;

#include <chrono>
#include <filesystem>
#include <ctime>
#include <sys/stat.h>

module snapshot_brief;

import stl;
import status;
import virtual_store;
import local_file_handle;
import hybridsearch_exception;
import third_party;
import logger;

namespace hybridsearch {

Vector<String> find_files_with_suffix(const String &path, const String &suffix) {
    Vector<String> files;
    for (const auto &entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_directory()) {
            // Don't search the directory recursively
        } else {
            if (entry.path().extension() == suffix) {
                files.emplace_back(entry.path().string());
            }
        }
    }
    return files;
}

Vector<SnapshotBrief> SnapshotBrief::GetSnapshots(const String &dir) {
    Vector<SnapshotBrief> briefs;
    Vector<String> snapshot_meta_array = find_files_with_suffix(dir, ".json");
    for (const String &snapshot_path : snapshot_meta_array) {
        try {
            auto [snapshot_file_handle, status] = VirtualStore::Open(snapshot_path, FileAccessMode::kRead);
            if (!status.ok()) {
                UnrecoverableError(status.message());
            }

            i64 file_size = snapshot_file_handle->FileSize();
            String json_str(file_size, 0);
            auto [n_bytes, status_read] = snapshot_file_handle->Read(json_str.data(), file_size);
            if (!status_read.ok()) {
                UnrecoverableError(status_read.message());
            }
            if ((SizeT)file_size != n_bytes) {
                UnrecoverableError(status_read.message());
            }

            nlohmann::json snapshot_json = nlohmann::json::parse(json_str);
            SnapshotBrief snapshot_brief;
            snapshot_brief.snapshot_name_ = snapshot_json["snapshot_name"];
            snapshot_brief.scope_ = snapshot_json["snapshot_scope"];

            snapshot_brief.commit_ts_ = snapshot_json["commit_ts"];

//            std::filesystem::path compressed_file(snapshot_path);
//            compressed_file.replace_extension("lz4");
            snapshot_brief.size_ = VirtualStore::GetDirectorySize(dir);


            struct stat statbuf;
            stat(snapshot_path.c_str(), &statbuf);
            std::time_t creationTime = statbuf.st_ctime;
            std::tm *ptm = std::localtime(&creationTime);
            char time_buffer[80];
            std::strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", ptm);
            snapshot_brief.create_time_ = time_buffer;
            briefs.emplace_back(snapshot_brief);
        } catch (const std::exception &e) {
            LOG_WARN(fmt::format("Fail to parse file: {}", snapshot_path));
        }
    }
    return briefs;
}

} // namespace hybridsearch
