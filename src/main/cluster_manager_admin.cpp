

module;

#include <vector>

module cluster_manager;

import stl;
import status;

namespace hybridsearch {

void ClusterManager::InitAsAdmin() {
    std::unique_lock<std::mutex> cluster_lock(cluster_mutex_);
    current_node_role_ = NodeRole::kAdmin;
}

Status ClusterManager::UnInitFromAdminNoLock() {
    current_node_role_ = NodeRole::kUnInitialized;
    return Status::OK();
}

} // namespace hybridsearch