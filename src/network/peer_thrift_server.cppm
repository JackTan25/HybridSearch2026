

module;

export module peer_thrift_server;

import stl;
import thrift;

namespace hybridsearch {

enum class PeerThriftServerStatus : u8 {
    kStopped,
    kRunning,
    kStopping,
};

export class PoolPeerThriftServer {
public:
    void Init(const String &server_address, i32 port_no, i32 pool_size);
    Thread Start();
    void Shutdown();

private:
    UniquePtr<apache::thrift::server::TServer> server{nullptr};

    bool initialized_{false};
    Atomic<PeerThriftServerStatus> status_{PeerThriftServerStatus::kStopped};
};

} // namespace hybridsearch
