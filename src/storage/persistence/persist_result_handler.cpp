

module;

#include <vector>

module persist_result_handler;

import hybridsearch_exception;
import third_party;
import virtual_store;
import hybridsearch_context;
import peer_task;
import logger;
import admin_statement;
import obj_status;

namespace hybridsearch {

void PersistResultHandler::HandleWriteResult(const PersistWriteResult &result) {
    for (const String &persist_key : result.persist_keys_) {
        String persist_path = pm_->GetObjPath(persist_key);
        if (hybridsearchContext::instance().GetServerRole() == NodeRole::kLeader or
            hybridsearchContext::instance().GetServerRole() == NodeRole::kStandalone) {
            VirtualStore::UploadObject(persist_path, persist_key);
        }
    }
    for (const String &drop_key : result.drop_keys_) {
        String drop_path = pm_->GetObjPath(drop_key);
        VirtualStore::DeleteFileBG(drop_path);
    }
    for (const String &drop_key : result.drop_from_remote_keys_) {
        String drop_path = pm_->GetObjPath(drop_key);
        VirtualStore::DeleteFileBG(drop_path);
        if (hybridsearchContext::instance().GetServerRole() == NodeRole::kLeader or
            hybridsearchContext::instance().GetServerRole() == NodeRole::kStandalone) {
            VirtualStore::RemoveObject(drop_key);
        }
    }
}

ObjAddr PersistResultHandler::HandleReadResult(const PersistReadResult &result) {
    if (result.obj_stat_ != nullptr) {
        ObjCached expect = ObjCached::kNotCached;
        Atomic<ObjCached> &cached = result.obj_stat_->cached_;
        if (cached.compare_exchange_strong(expect, ObjCached::kDownloading)) {
            VirtualStore::AddRequestCount();
            String read_path = hybridsearchContext::instance().persistence_manager()->GetObjPath(result.obj_addr_.obj_key_);
            LOG_TRACE(fmt::format("GetObjCache download object {}.", read_path));
            VirtualStore::DownloadObject(read_path, result.obj_addr_.obj_key_);
            LOG_TRACE(fmt::format("GetObjCache download object {} done.", read_path));
            cached.store(ObjCached::kCached);
            cached.notify_all();
            VirtualStore::AddCacheMissCount();
        } else if (expect == ObjCached::kDownloading) {
            LOG_TRACE(fmt::format("GetObjCache waiting downloading object {}", result.obj_addr_.obj_key_));
            cached.wait(ObjCached::kDownloading);
            LOG_TRACE(fmt::format("GetObjCache finish waiting object {}", result.obj_addr_.obj_key_));
        } else {
            VirtualStore::AddRequestCount();
        }
    }
    return result.obj_addr_;
}

} // namespace hybridsearch
