

module;

#include <thrift/TApplicationException.h>
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/protocol/TCompactProtocol.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TSocket.h>

export module thrift;

export namespace apache {
namespace thrift {
using apache::thrift::TApplicationException;

namespace concurrency {
using apache::thrift::concurrency::Thread;
using apache::thrift::concurrency::ThreadFactory;
using apache::thrift::concurrency::ThreadManager;
} // namespace concurrency

namespace server {
using apache::thrift::server::TServer;
using apache::thrift::server::TThreadedServer;
using apache::thrift::server::TThreadPoolServer;
} // namespace server

namespace transport {
using apache::thrift::transport::TBufferedTransport;
using apache::thrift::transport::TBufferedTransportFactory;
using apache::thrift::transport::TServerSocket;
using apache::thrift::transport::TSocket;
using apache::thrift::transport::TTransport;
using apache::thrift::transport::TTransportException;
using TTransportExceptionType = apache::thrift::transport::TTransportException::TTransportExceptionType;
} // namespace transport

namespace protocol {
using apache::thrift::protocol::TBinaryProtocol;
using apache::thrift::protocol::TBinaryProtocolFactory;
using apache::thrift::protocol::TCompactProtocolFactory;
using apache::thrift::protocol::TProtocol;
using apache::thrift::protocol::TProtocolFactory;
} // namespace protocol
} // namespace thrift
} // namespace apache
