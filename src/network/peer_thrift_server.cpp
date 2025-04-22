

module;

#include <memory>
#include <thrift/TToString.h>
#include <thrift/concurrency/ThreadFactory.h>
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TCompactProtocol.h>
#include <thrift/server/TNonblockingServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TNonblockingServerSocket.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

module peer_thrift_server;

import peer_server_thrift_service;
import peer_server_thrift_types;
import logger;
import third_party;
import stl;
import hybridsearch_exception;

using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

namespace hybridsearch {

class PeerServiceCloneFactory final : public hybridsearch_peer_server::PeerServiceIfFactory {
public:
    ~PeerServiceCloneFactory() final = default;

    hybridsearch_peer_server::PeerServiceIf *getHandler(const ::apache::thrift::TConnectionInfo &connInfo) final { return new PeerServerThriftService; }

    void releaseHandler(hybridsearch_peer_server::PeerServiceIf *handler) final { delete handler; }
};

void PoolPeerThriftServer::Init(const String &server_address, i32 port_no, i32 pool_size) {

    SharedPtr<TServerSocket> server_socket = MakeShared<TServerSocket>(server_address, port_no);

    SharedPtr<TBinaryProtocolFactory> protocol_factory = MakeShared<TBinaryProtocolFactory>();

    SharedPtr<ThreadFactory> threadFactory = MakeShared<ThreadFactory>();

    SharedPtr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(pool_size);
    threadManager->threadFactory(threadFactory);
    threadManager->start();

    fmt::print("Peer server listen on {}: {}, connection limit: {}\n", server_address, port_no, pool_size);

    server = MakeUnique<TThreadPoolServer>(MakeShared<hybridsearch_peer_server::PeerServiceProcessorFactory>(MakeShared<PeerServiceCloneFactory>()),
                                           server_socket,
                                           MakeShared<TBufferedTransportFactory>(),
                                           protocol_factory,
                                           threadManager);
    initialized_ = true;
}

Thread PoolPeerThriftServer::Start() {
    if (!initialized_) {
        UnrecoverableError("Peer server is not initialized.");
    }
    {
        auto expect = PeerThriftServerStatus::kStopped;
        if (!status_.compare_exchange_strong(expect, PeerThriftServerStatus::kRunning)) {
            UnrecoverableError(fmt::format("Peer server in unexpected state: {}", u8(expect)));
        }
    }
    return Thread([this] {
        server->serve();

        status_.store(PeerThriftServerStatus::kStopped);
        status_.notify_one();
    });
}

void PoolPeerThriftServer::Shutdown() {
    {
        auto expected = PeerThriftServerStatus::kRunning;
        if (!status_.compare_exchange_strong(expected, PeerThriftServerStatus::kStopping)) {
            if (status_ == PeerThriftServerStatus::kStopped) {
                return;
            } else {
                UnrecoverableError(fmt::format("Peer server in unexpected state: {}", u8(expected)));
            }
        }
    }
    server->stop();

    status_.wait(PeerThriftServerStatus::kStopping);
}

} // namespace hybridsearch
