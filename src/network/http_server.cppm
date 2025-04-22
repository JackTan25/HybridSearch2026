

module;

export module http_server;

import stl;
import third_party;

namespace hybridsearch {

enum struct HTTPServerStatus : u8 {
    kStopped = 0,
    kStarting,
    kRunning,
    kStopping,
};

export class HTTPServer {
public:
    Thread Start(const String &server_address, u16 port);
    void Shutdown();

private:
    Atomic<HTTPServerStatus> status_{HTTPServerStatus::kStopped};
    SharedPtr<HttpRouter> router_{};
    SharedPtr<WebServer> server_{};

    SharedPtr<HttpConnectionProvider> connection_provider_{};
    SharedPtr<HttpConnectionHandler> connection_handler_{};
};

} // namespace hybridsearch
