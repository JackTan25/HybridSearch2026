

#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#ifdef ENABLE_JEMALLOC_PROF
#include <jemalloc/jemalloc.h>
#endif

import compilation_config;
import stl;
import third_party;
import pg_server;
import hybridsearch_exception;
import hybridsearch_context;
import thrift_server;
import peer_thrift_server;
import http_server;
import logger;
import simd_init;

namespace {

#define THRIFT_SERVER_TYPE 0

#if THRIFT_SERVER_TYPE == 0

hybridsearch::Thread pool_thrift_thread;
hybridsearch::PoolThriftServer pool_thrift_server;

#elif THRIFT_SERVER_TYPE == 1

hybridsearch::Thread non_block_pool_thrift_thread;
hybridsearch::NonBlockPoolThriftServer non_block_pool_thrift_server;

#else

hybridsearch::Thread threaded_thrift_thread;
hybridsearch::ThreadedThriftServer threaded_thrift_server;

#endif

hybridsearch::Thread pool_peer_thrift_thread;
hybridsearch::PoolPeerThriftServer pool_peer_thrift_server;

hybridsearch::Thread http_server_thread;
hybridsearch::HTTPServer http_server;

hybridsearch::Thread pg_thread;
hybridsearch::PGServer pg_server;

// Used for server shutdown
std::mutex server_mutex;
std::condition_variable server_cv;

bool server_running = false;

hybridsearch::Thread shutdown_thread;

void StartThriftServer() {
    using namespace hybridsearch;
    u32 thrift_server_port = hybridsearchContext::instance().config()->ClientPort();

#if THRIFT_SERVER_TYPE == 0

    i32 thrift_server_pool_size = hybridsearchContext::instance().config()->ConnectionPoolSize();
    pool_thrift_server.Init(hybridsearchContext::instance().config()->ServerAddress(), thrift_server_port, thrift_server_pool_size);
    pool_thrift_thread = pool_thrift_server.Start();

#elif THRIFT_SERVER_TYPE == 1

    i32 thrift_server_pool_size = hybridsearchContext::instance().config()->ConnectionPoolSize();
    non_block_pool_thrift_server.Init(hybridsearchContext::instance().config()->ServerAddress(), thrift_server_port, thrift_server_pool_size);
    non_block_pool_thrift_thread = hybridsearch::Thread([&]() { non_block_pool_thrift_server.Start(); });

#else

    threaded_thrift_server.Init(hybridsearchContext::instance().config()->ServerAddress(), thrift_server_port);
    threaded_thrift_thread = hybridsearch::Thread([&]() { threaded_thrift_server.Start(); });

#endif
    LOG_INFO("Thrift server is started.");
}

void StopThriftServer() {
    using namespace hybridsearch;
#if THRIFT_SERVER_TYPE == 0
    pool_thrift_server.Shutdown();
    pool_thrift_thread.join();
#elif THRIFT_SERVER_TYPE == 1
    non_block_pool_thrift_server.Shutdown();
    non_block_pool_thrift_server.join();
#else
    threaded_thrift_server.Shutdown();
    threaded_thrift_thread.join();
#endif
    LOG_INFO("Thrift server is shutdown.");
}

void StartPeerServer() {
    using namespace hybridsearch;
    u32 peer_server_port = hybridsearchContext::instance().config()->PeerServerPort();
    i32 peer_server_connection_pool_size = hybridsearchContext::instance().config()->PeerServerConnectionPoolSize();
    pool_peer_thrift_server.Init(hybridsearchContext::instance().config()->PeerServerIP(), peer_server_port, peer_server_connection_pool_size);
    pool_peer_thrift_thread = pool_peer_thrift_server.Start();
    hybridsearch::LOG_INFO("Peer server is started.");
}

void StopPeerServer() {
    using namespace hybridsearch;
    pool_peer_thrift_server.Shutdown();
    pool_peer_thrift_thread.join();
    LOG_INFO("Peer server is shutdown.");
}

void ShutdownServer() {
    {
        std::unique_lock<std::mutex> lock(server_mutex);
        server_running = true;
        server_cv.wait(lock, [&] { return !server_running; });
    }

    http_server.Shutdown();

    hybridsearch::LOG_INFO("HTTP server is shutdown.");

    pg_server.Shutdown();

    hybridsearch::LOG_INFO("PG server is shutdown.");

    hybridsearch::hybridsearchContext::instance().UnInit();

    fmt::print("Shutdown hybridsearch server successfully\n");
}

void SignalHandler(int signal_number, siginfo_t *, void *) {
    switch (signal_number) {
        case SIGUSR1:
        case SIGINT:
        case SIGQUIT:
        case SIGTERM: {
            fmt::print("Shutdown hybridsearch server ...\n");

            std::unique_lock<std::mutex> lock(server_mutex);
            server_running = false;
            server_cv.notify_one();

            break;
        }
        case SIGSEGV: {
            // Print back strace
            hybridsearch::PrintTransactionHistory();
            hybridsearch::PrintStacktrace("SEGMENT FAULTS");
            exit(-1);
            break;
        }
#ifdef ENABLE_JEMALLOC_PROF
        case SIGUSR2: {
            // http://jemalloc.net/jemalloc.3.html
            int rc = mallctl("prof.dump", NULL, NULL, NULL, 0);
            printf("Dump memory profile %d\n", rc);
            break;
        }
#endif
        default: {
            // Ignore
            printf("Other type of signal: %d\n", signal_number);
        }
    }
    //    exit(0);
}

void RegisterSignal() {
    struct sigaction sig_action;
    sig_action.sa_flags = SA_SIGINFO;
    sig_action.sa_sigaction = SignalHandler;
    sigemptyset(&sig_action.sa_mask);
    sigaction(SIGUSR1, &sig_action, NULL);
#ifdef ENABLE_JEMALLOC_PROF
    sigaction(SIGUSR2, &sig_action, NULL);
#endif
    sigaction(SIGINT, &sig_action, NULL);
    sigaction(SIGQUIT, &sig_action, NULL);
    sigaction(SIGTERM, &sig_action, NULL);
    sigaction(SIGSEGV, &sig_action, NULL);
}

void TerminateHandler() {
    hybridsearch::String message = "TerminateHandler: ";
    try {
        std::exception_ptr eptr{std::current_exception()};
        if (eptr) {
            std::rethrow_exception(eptr);
        } else {
            message += "Exiting without exception";
        }
    } catch (const std::exception &ex) {
        message += "Unhandled Exception: ";
        message += ex.what();
    } catch (...) {
        message += "Unknown Unhandled Exception";
    }
    hybridsearch::PrintTransactionHistory();
    hybridsearch::PrintStacktrace(message);
    std::abort();
}

} // namespace

auto main(int argc, char **argv) -> int {
    using namespace hybridsearch;

    fmt::print(" __  .__   __.  _______  __  .__   __.  __  .___________.____    ____ \n"
               "|  | |  \\ |  | |   ____||  | |  \\ |  | |  | |           |\\   \\  /   / \n"
               "|  | |   \\|  | |  |__   |  | |   \\|  | |  | `---|  |----` \\   \\/   /  \n"
               "|  | |  . `  | |   __|  |  | |  . `  | |  |     |  |       \\_    _/   \n"
               "|  | |  |\\   | |  |     |  | |  |\\   | |  |     |  |         |  |     \n"
               "|__| |__| \\__| |__|     |__| |__| \\__| |__|     |__|         |__|     \n");

    fmt::print("Release: {}.{}.{} build on {} with {} mode from branch: {}, commit-id: {}\n",
               version_major(),
               version_minor(),
               version_patch(),
               system_build_time(),
               build_type(),
               git_branch_name(),
               git_commit_id());

    fmt::print("Currently enabled SIMD support: {}\n", fmt::join(GetSupportedSimdTypesList(), ", "));

    CLI::App app{"hybridsearch_main"};

    SharedPtr<String> config_path = MakeShared<String>();
    bool m_flag{false};
    app.add_option("-f,--config", *config_path, "Specify the config file path. No default config file");
    app.add_flag("-m,--maintenance", m_flag, "Start hybridsearch in maintenance mode");
    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        return app.exit(e);
    }

    hybridsearchContext::instance().InitPhase1(config_path, nullptr);

    auto start_thrift_servers = [&]() {
        StartThriftServer();
        StartPeerServer();
    };

    auto stop_thrift_servers = [&]() {
        StopPeerServer();
        StopThriftServer();
    };

    hybridsearchContext::instance().AddThriftServerFn(start_thrift_servers, stop_thrift_servers);
    hybridsearchContext::instance().StartThriftServers();

    pg_thread = pg_server.Run();

    http_server_thread = http_server.Start(hybridsearchContext::instance().config()->ServerAddress(), hybridsearchContext::instance().config()->HTTPPort());

    shutdown_thread = hybridsearch::Thread([&]() { ShutdownServer(); });

    RegisterSignal();

    std::set_terminate(TerminateHandler);

    hybridsearchContext::instance().InitPhase2(m_flag);

    shutdown_thread.join();

    http_server_thread.join();

    pg_thread.join();

    fmt::print("Server is shutdown\n");
    return 0;
}
