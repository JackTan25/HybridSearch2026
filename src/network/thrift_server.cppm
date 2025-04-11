

module;

#include <sstream>
#include <stdexcept>

export module thrift_server;

import stl;
import hybridsearch;
import hybridsearch_thrift_service;
import query_options;
import thrift;

using namespace std;

namespace hybridsearch {

enum class ThriftServerStatus : u8 {
    kStopped,
    kRunning,
    kStopping,
};

#define THRIFT_SERVER_TYPE 0

#if THRIFT_SERVER_TYPE == 2

export class ThreadedThriftServer {
public:
    void Init(const String &server_address, i32 port_no);
    void Start();
    void Shutdown();

private:
    UniquePtr<apache::thrift::server::TThreadedServer> server{nullptr};

    atomic_bool started_{false};
};

#elif THRIFT_SERVER_TYPE == 0

export class PoolThriftServer {
public:
    void Init(const String &server_address, i32 port_no, i32 pool_size);
    Thread Start();

    void Shutdown();

private:
    UniquePtr<apache::thrift::server::TServer> server{nullptr};

    bool initialized_{false};
    Atomic<ThriftServerStatus> status_ = ThriftServerStatus::kStopped;
};

#elif THRIFT_SERVER_TYPE == 1

export class NonBlockPoolThriftServer {
public:
    void Init(const String &server_address, i32 port_no, i32 pool_size);
    void Start();
    void Shutdown();

private:
    //    UniquePtr<TServer> server{nullptr};
    SharedPtr<hybridsearchThriftService> service_handler_{};
    SharedPtr<apache::thrift::concurrency::Thread> server_thread_{};

    atomic_bool started_{false};
};

#endif

} // namespace hybridsearch
