

module;

export module peer_server_thrift_service;

import peer_server_thrift_types;
import stl;
import global_resource_usage;

namespace hybridsearch {

export class PeerServerThriftService final : public hybridsearch_peer_server::PeerServiceIf {

public:
    PeerServerThriftService() {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::IncrObjectCount("PeerServerThriftService");
#endif
    }
    virtual ~PeerServerThriftService() {
#ifdef hybridsearch_DEBUG
        GlobalResourceUsage::DecrObjectCount("PeerServerThriftService");
#endif
    }

    void Register(hybridsearch_peer_server::RegisterResponse &response, const hybridsearch_peer_server::RegisterRequest &request) final;
    void Unregister(hybridsearch_peer_server::UnregisterResponse &response, const hybridsearch_peer_server::UnregisterRequest &request) final;
    void HeartBeat(hybridsearch_peer_server::HeartBeatResponse &response, const hybridsearch_peer_server::HeartBeatRequest &request) final;
    void SyncLog(hybridsearch_peer_server::SyncLogResponse &response, const hybridsearch_peer_server::SyncLogRequest &request) final;
    void ChangeRole(hybridsearch_peer_server::ChangeRoleResponse &response, const hybridsearch_peer_server::ChangeRoleRequest &request) final;
    void NewLeader(hybridsearch_peer_server::NewLeaderResponse &response, const hybridsearch_peer_server::NewLeaderRequest &request) final;

private:
};

} // namespace hybridsearch
