

module;

#include <vector>

module cluster_manager;

import stl;
import status;

namespace hybridsearch {

void ClusterManager::InitAsStandalone() {
    std::unique_lock<std::mutex> cluster_lock(cluster_mutex_);
    current_node_role_ = NodeRole::kStandalone;
}

Status ClusterManager::UnInitFromStandaloneNoLock() {
    current_node_role_ = NodeRole::kUnInitialized;
    return Status::OK();
}

} // namespace hybridsearch