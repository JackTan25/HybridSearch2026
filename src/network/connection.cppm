

module;

export module connection;

import boost;
import stl;
import session;
import pg_protocol_handler;
import query_context;
import data_table;
import query_result;

namespace hybridsearch {

export class Connection {
public:
    explicit Connection(boost::asio::io_service &io_service);

    ~Connection();

    void Run();

    inline SharedPtr<boost::asio::ip::tcp::socket> socket() { return socket_; }

    inline void GetClientInfo(String &ip_address, u16 &port) {
        if (session_.get() != nullptr) {
            session_->GetClientInfo(ip_address, port);
        } else {
            ip_address = "";
            port = 0;
        }
    }

private:
    void HandleConnection();

    void HandleRequest();

    void HandlerSimpleQuery(QueryContext *query_context);

    void SendTableDescription(const SharedPtr<DataTable> &result_table);

    void SendQueryResponse(const QueryResult &query_result);

    void HandleError(const char *error_message);

private:
    const SharedPtr<boost::asio::ip::tcp::socket> socket_{};

    const SharedPtr<PGProtocolHandler> pg_handler_{};

    bool terminate_connection_ = false;

    SharedPtr<RemoteSession> session_{};
};

} // namespace hybridsearch
