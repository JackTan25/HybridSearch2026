

module;

#include <cstdlib>
#include <execinfo.h>
#include <iostream>

module hybridsearch_exception;

import stl;
import logger;
import third_party;
import hybridsearch_context;
import cleanup_scanner;
import txn_manager;
import txn_context;

namespace hybridsearch {

void PrintTransactionHistory() {
    TxnManager *txn_manager = hybridsearchContext::instance().storage()->txn_manager();

    Vector<SharedPtr<TxnContext>> txn_contexts = txn_manager->GetTxnContextHistories();

    SizeT history_count = txn_contexts.size();
    for (SizeT idx = 0; idx < history_count; ++idx) {
        SharedPtr<TxnContext> txn_history = txn_contexts[idx];
        LOG_CRITICAL(txn_history->ToString());
    }
}

void PrintStacktrace(const String &err_msg) {
    int trace_stack_depth = 256;
    void *array[256];
    int stack_num = backtrace(array, trace_stack_depth);
    char **stacktrace = backtrace_symbols(array, stack_num);

    LOG_CRITICAL(fmt::format("Error: {}", err_msg));
    for (int i = 0; i < stack_num; ++i) {
        String info = stacktrace[i];
        LOG_CRITICAL(fmt::format("{}, {}", i, info));
    }
    free(stacktrace);
}

#define ADD_LOG_INFO

#if defined(hybridsearch_DEBUG) || defined(ADD_LOG_INFO)

void RecoverableError(Status status, const char *file_name, u32 line) {
    status.AppendMessage(fmt::format("@{}:{}", hybridsearch::TrimPath(file_name), line));
    if (IS_LOGGER_INITIALIZED()) {
        LOG_ERROR(status.message());
    }
    throw RecoverableException(status);
}

std::string_view GetErrorMsg(const String &message) {
    auto pos = message.find_first_of('@', 0);
    return {message.data(), pos};
}

void UnrecoverableError(const String &message, const char *file_name, u32 line) {
    auto *storage = hybridsearchContext::instance().storage();
    if (storage != nullptr) {
        if (storage->txn_manager() != nullptr) {
            hybridsearch::PrintTransactionHistory();
        }
    }
    // if (storage != nullptr) {
    //     CleanupInfoTracer *cleanup_tracer = storage->cleanup_info_tracer();
    //     String error_msg = cleanup_tracer->GetCleanupInfo();
    //     LOG_ERROR(std::move(error_msg));
    // }
    String location_message = fmt::format("{}@{}:{}", message, hybridsearch::TrimPath(file_name), line);
    if (IS_LOGGER_INITIALIZED()) {

        PrintStacktrace(location_message);
    }
    Logger::Flush();
    throw UnrecoverableException(location_message);
}

#else

void RecoverableError(Status status) {
    if (IS_LOGGER_INITIALIZED()) {
        LOG_ERROR(status.message());
    }
    throw RecoverableException(status);
}

void UnrecoverableError(const String &message) {
    if (IS_LOGGER_INITIALIZED()) {
        LOG_CRITICAL(message);
    }
    Logger::Flush();
    throw UnrecoverableException(message);
}

std::string_view GetErrorMsg(const String &message) { return message; }

#endif

} // namespace hybridsearch
