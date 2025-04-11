

module;

export module pg_server;

import stl;
import singleton;
import boost;
import connection;

namespace hybridsearch {

enum struct PGServerStatus : u8 {
    kStopped = 0,
    kStarting,
    kRunning,
    kStopping,
};

export class PGServer {
public:
    Thread Run();

    void Shutdown();

private:
    void CreateConnection();

    void StartConnection(SharedPtr<Connection> &connection);

    Atomic<PGServerStatus> status_{PGServerStatus::kStopped};
    atomic_u64 running_connection_count_{0};
    UniquePtr<boost::asio::io_service> io_service_ptr_{};
    UniquePtr<boost::asio::ip::tcp::acceptor> acceptor_ptr_{};
};

} // namespace hybridsearch
