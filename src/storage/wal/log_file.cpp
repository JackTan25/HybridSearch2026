

module;

#include <vector>

module log_file;

import stl;
import virtual_store;
import third_party;
import hybridsearch_exception;
import logger;
import default_values;
import hybridsearch_context;
import status;
import admin_statement;

namespace hybridsearch {

Optional<Pair<FullCatalogFileInfo, Vector<DeltaCatalogFileInfo>>> CatalogFile::ParseValidCheckpointFilenames(const String &catalog_dir,
                                                                                                             TxnTimeStamp max_checkpoint_ts) {
    auto [full_infos, delta_infos, temp_full_infos, temp_delta_infos] = ParseCheckpointFilenames(catalog_dir);
    std::sort(full_infos.begin(), full_infos.end(), [](const auto &lhs, const auto &rhs) { return lhs.max_commit_ts_ < rhs.max_commit_ts_; });
    std::sort(delta_infos.begin(), delta_infos.end(), [](const auto &lhs, const auto &rhs) { return lhs.max_commit_ts_ < rhs.max_commit_ts_; });

    while (!full_infos.empty() && full_infos.back().max_commit_ts_ > max_checkpoint_ts) {
        LOG_WARN(fmt::format("Full catalog file {} is newer than the max checkpoint ts {}", full_infos.back().path_, max_checkpoint_ts));
        full_infos.pop_back();
    }
    if (full_infos.empty()) {
        return None;
    }
    if (full_infos.size() > 1) {
        LOG_WARN(fmt::format("Multiple full catalog files found in the catalog directory: {}", catalog_dir));
    }
    FullCatalogFileInfo last_full_info = full_infos.back();

    while (!delta_infos.empty() && delta_infos.back().max_commit_ts_ > max_checkpoint_ts) {
        LOG_WARN(fmt::format("Delta catalog file {} is newer than the max checkpoint ts {}", delta_infos.back().path_, max_checkpoint_ts));
        delta_infos.pop_back();
    }
    while (!delta_infos.empty() && delta_infos.front().max_commit_ts_ <= last_full_info.max_commit_ts_) {
        LOG_WARN(fmt::format("Delta catalog file {} is older than the full catalog file {}", delta_infos.front().path_, last_full_info.path_));
        delta_infos.erase(delta_infos.begin());
    }

    auto res = Pair<FullCatalogFileInfo, Vector<DeltaCatalogFileInfo>>{last_full_info, std::move(delta_infos)};
    return res;
}

String CatalogFile::FullCheckpointFilename(TxnTimeStamp max_commit_ts) { return fmt::format("FULL.{}.json", max_commit_ts); }

String CatalogFile::TempFullCheckpointFilename(TxnTimeStamp max_commit_ts) { return fmt::format("_FULL.{}.json", max_commit_ts); }

String CatalogFile::DeltaCheckpointFilename(TxnTimeStamp max_commit_ts) { return fmt::format("DELTA.{}", max_commit_ts); }

String CatalogFile::TempDeltaCheckpointFilename(TxnTimeStamp max_commit_ts) { return fmt::format("_DELTA.{}", max_commit_ts); }

void CatalogFile::RecycleCatalogFile(TxnTimeStamp max_commit_ts, const String &catalog_dir) {
    bool upload_catalog =
        hybridsearchContext::instance().GetServerRole() == NodeRole::kLeader or hybridsearchContext::instance().GetServerRole() == NodeRole::kStandalone;

    auto [full_infos, delta_infos, temp_full_infos, temp_delta_infos] = ParseCheckpointFilenames(catalog_dir);
    bool found = false;
    for (const auto &full_info : full_infos) {
        if (full_info.max_commit_ts_ < max_commit_ts) {
            LOG_INFO(fmt::format("WalManager::Checkpoint delete full catalog file: {}", full_info.path_));
            VirtualStore::DeleteFile(full_info.path_);
            if (upload_catalog) {
                const String filename = Path(full_info.path_).filename();
                VirtualStore::RemoveObject(filename);
            }
        } else if (full_info.max_commit_ts_ == max_commit_ts) {
            found = true;
        }
    }
    if (!found) {
        String error_message = fmt::format("Full catalog file {} not found in the catalog directory: {}", max_commit_ts, catalog_dir);
        UnrecoverableError(error_message);
    }
    for (const auto &delta_info : delta_infos) {
        if (delta_info.max_commit_ts_ <= max_commit_ts) {
            LOG_INFO(fmt::format("WalManager::Checkpoint delete delta catalog file: {}", delta_info.path_));
            VirtualStore::DeleteFile(delta_info.path_);
            if (upload_catalog) {
                const String filename = Path(delta_info.path_).filename();
                VirtualStore::RemoveObject(filename);
            }
        }
    }
}

CatalogFilesInfo CatalogFile::ParseCheckpointFilenames(const String &catalog_dir) {
    auto [entries, status] = VirtualStore::ListDirectory(Path(hybridsearchContext::instance().config()->DataDir()) / catalog_dir);
    if (!status.ok()) {
        String error_message = fmt::format("Can't list directory: {}/{}", hybridsearchContext::instance().config()->DataDir(), catalog_dir);
        UnrecoverableError(error_message);
    }

    if (entries.empty()) {
        return {Vector<FullCatalogFileInfo>{}, Vector<DeltaCatalogFileInfo>{}, Vector<TempFullCatalogFileInfo>{}, Vector<TempDeltaCatalogFileInfo>{}};
    }

    Vector<FullCatalogFileInfo> full_infos;
    Vector<DeltaCatalogFileInfo> delta_infos;
    Vector<TempFullCatalogFileInfo> temp_full_infos;
    Vector<TempDeltaCatalogFileInfo> temp_delta_infos;
    for (const auto &entry : entries) {
        if (!entry->is_regular_file()) {
            LOG_WARN(fmt::format("Catalog file {} is not a regular file", entry->path().string()));
            continue;
        }
        const auto &filename = entry->path().filename().string();
        auto dot_pos = filename.rfind('.');
        if (dot_pos == String::npos) {
            LOG_WARN(fmt::format("Catalog file {} has wrong file name", entry->path().string()));
            continue;
        }
        auto suffix = filename.substr(dot_pos + 1);
        if (IsEqual(suffix, String("json"))) {
            if (dot_pos == 0) {
                LOG_WARN(fmt::format("Catalog file {} has wrong file name", entry->path().string()));
                continue;
            }
            auto dot2_pos = filename.rfind('.', dot_pos - 1);
            TxnTimeStamp checkpoint_ts;
            try {
                checkpoint_ts = std::stoll(filename.substr(dot2_pos + 1, dot_pos - dot2_pos - 1));
            } catch (...) {
                LOG_WARN(fmt::format("Catalog file {} has wrong file name", entry->path().string()));
                continue;
            }
            auto file_prefix = filename.substr(0, dot2_pos);
            if (IsEqual(file_prefix, String("_FULL"))) {
                temp_full_infos.emplace_back(TempFullCatalogFileInfo{entry->path().string(), checkpoint_ts});
                continue;
            }
            if (!IsEqual(file_prefix, String("FULL"))) {
                LOG_WARN(fmt::format("Catalog file {} has wrong file name", entry->path().string()));
                continue;
            }
            full_infos.emplace_back(FullCatalogFileInfo{entry->path().string(), checkpoint_ts});
        } else {
            TxnTimeStamp checkpoint_ts;
            try {
                checkpoint_ts = std::stoll(filename.substr(dot_pos + 1));
            } catch (...) {
                LOG_WARN(fmt::format("Catalog file {} has wrong file name", entry->path().string()));
                continue;
            }
            auto file_prefix = filename.substr(0, dot_pos);
            if (IsEqual(file_prefix, String("_DELTA"))) {
                temp_delta_infos.emplace_back(TempDeltaCatalogFileInfo{entry->path().string(), checkpoint_ts});
                continue;
            }
            if (!IsEqual(file_prefix, String("DELTA"))) {
                LOG_WARN(fmt::format("Catalog file {} has wrong file name", entry->path().string()));
                continue;
            }
            delta_infos.push_back({entry->path().string(), checkpoint_ts});
        }
    }
    return {std::move(full_infos), std::move(delta_infos), std::move(temp_full_infos), std::move(temp_delta_infos)};
}

Pair<Optional<TempWalFileInfo>, Vector<WalFileInfo>> WalFile::ParseWalFilenames(const String &wal_dir) {

    if (!VirtualStore::Exists(wal_dir)) {
        return {None, Vector<WalFileInfo>{}};
    }

    auto [entries, status] = VirtualStore::ListDirectory(wal_dir);
    if (!status.ok()) {
        UnrecoverableError(status.message());
    }

    if (entries.empty()) {
        return {None, Vector<WalFileInfo>{}};
    }
    Optional<TempWalFileInfo> cur_wal_info;
    Vector<WalFileInfo> wal_infos;
    for (const auto &entry : entries) {
        if (!entry->is_regular_file()) {
            LOG_WARN(fmt::format("Wal file {} is not a regular file", entry->path().string()));
            continue;
        }
        const auto &filename = entry->path().filename().string();
        if (IsEqual(filename, WalFile::TempWalFilename())) {
            if (cur_wal_info.has_value()) {
                String error_message = fmt::format("Multiple current wal files found in the wal directory: {}", wal_dir);
                UnrecoverableError(error_message);
            }
            cur_wal_info = TempWalFileInfo{entry->path().string()};
        } else {
            auto dot_pos = filename.rfind('.');
            if (dot_pos == String::npos) {
                LOG_WARN(fmt::format("Wal file {} has wrong file name", entry->path().string()));
                continue;
            }
            TxnTimeStamp checkpoint_ts;
            try {
                checkpoint_ts = std::stoll(filename.substr(dot_pos + 1));
            } catch (...) {
                LOG_WARN(fmt::format("Wal file {} has wrong file name", entry->path().string()));
                continue;
            }
            auto file_prefix = filename.substr(0, dot_pos);
            if (!IsEqual(file_prefix, String(WAL_FILE_PREFIX))) {
                LOG_WARN(fmt::format("Wal file {} has wrong file name", entry->path().string()));
                continue;
            }
            wal_infos.push_back({entry->path().string(), checkpoint_ts});
        }
    }
    if (!cur_wal_info.has_value()) {
        // this happens when temp wal file is swapped and new wal is not created yet
        LOG_INFO(fmt::format("Current wal file not found in the wal directory: {}", wal_dir));
    }
    return {cur_wal_info, wal_infos};
}

String WalFile::WalFilename(TxnTimeStamp max_commit_ts) { return fmt::format("{}.{}", String(WAL_FILE_PREFIX), max_commit_ts); }

String WalFile::TempWalFilename() { return String(WAL_FILE_TEMP_FILE); }

// /**
//  * @brief Gc the old wal files.
//  * Only delete the wal.log.* files. the wal.log file is current wal file.
//  * Check if the wal.log.* files are too old.
//  * if * is less than the max_commit_ts, we will delete it.
//  */
void WalFile::RecycleWalFile(TxnTimeStamp max_commit_ts, const String &wal_dir) {
    auto [cur_wal_info, wal_infos] = ParseWalFilenames(wal_dir);
    for (const auto &wal_info : wal_infos) {
        if (wal_info.max_commit_ts_ <= max_commit_ts) {
            LOG_INFO(fmt::format("WalManager::Checkpoint delete wal file: {}", wal_info.path_));
            VirtualStore::DeleteFile(wal_info.path_);
        }
    }
}

} // namespace hybridsearch