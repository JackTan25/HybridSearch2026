

module;

#include "peer_server_thrift/PeerService.h"

export module peer_server_thrift_types;

namespace hybridsearch_peer_server {

export using hybridsearch_peer_server::PeerServiceIf;
export using hybridsearch_peer_server::PeerServiceClient;
export using hybridsearch_peer_server::PeerServiceIfFactory;
export using hybridsearch_peer_server::PeerServiceProcessorFactory;
export using hybridsearch_peer_server::NodeStatus;
export using hybridsearch_peer_server::NodeType;
export using hybridsearch_peer_server::NodeInfo;
export using hybridsearch_peer_server::RegisterRequest;
export using hybridsearch_peer_server::RegisterResponse;
export using hybridsearch_peer_server::UnregisterRequest;
export using hybridsearch_peer_server::UnregisterResponse;
export using hybridsearch_peer_server::HeartBeatRequest;
export using hybridsearch_peer_server::HeartBeatResponse;
export using hybridsearch_peer_server::SyncLogRequest;
export using hybridsearch_peer_server::SyncLogResponse;
export using hybridsearch_peer_server::ChangeRoleRequest;
export using hybridsearch_peer_server::ChangeRoleResponse;
export using hybridsearch_peer_server::NewLeaderRequest;
export using hybridsearch_peer_server::NewLeaderResponse;
export using hybridsearch_peer_server::to_string;
} // namespace hybridsearch_peer_server
