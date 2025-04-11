
module;

export module logger;

import stl;
import third_party;
import status;

namespace hybridsearch {

export extern SharedPtr<spdlog::logger> hybridsearch_logger;

class Config;

export struct LoggerConfig {
    bool log_to_stdout_ = true;
    String log_file_path_ = "tmp.log";
    SizeT log_file_max_size_ = 1024 * 1024 * 10;
    SizeT log_file_rotate_count_ = 5;
    LogLevel log_level_ = LogLevel::kInfo;
};

export class Logger {
public:
    static Status Initialize(Config *config_ptr);

    static void Initialize(const LoggerConfig &config);
    static void Flush();

    static void Shutdown();
};

export inline bool IS_LOGGER_INITIALIZED() { return hybridsearch_logger.get() != nullptr; }

export inline bool SHOULD_LOG_TRACE() { return IS_LOGGER_INITIALIZED() && hybridsearch_logger->should_log(spdlog::level::level_enum::trace); }

export inline bool SHOULD_LOG_DEBUG() { return IS_LOGGER_INITIALIZED() && hybridsearch_logger->should_log(spdlog::level::level_enum::debug); }

export inline bool SHOULD_LOG_INFO() { return IS_LOGGER_INITIALIZED() && hybridsearch_logger->should_log(spdlog::level::level_enum::info); }

export inline void LOG_TRACE(const String &msg) {
    if (IS_LOGGER_INITIALIZED()) {
        hybridsearch_logger->trace(msg);
    } else {
        fmt::print("[trace] {}\n", msg);
    }
}

export inline void LOG_DEBUG(const String &msg) {
    if (IS_LOGGER_INITIALIZED()) {
        hybridsearch_logger->debug(msg);
    } else {
        fmt::print("[debug] {}\n", msg);
    }
}

export inline void LOG_INFO(const String &msg) {
    if (IS_LOGGER_INITIALIZED()) {
        hybridsearch_logger->info(msg);
    } else {
        fmt::print("[info] {}\n", msg);
    }
}

export inline void LOG_WARN(const String &msg) {
    if (IS_LOGGER_INITIALIZED()) {
        hybridsearch_logger->warn(msg);
    } else {
        fmt::print("[warn] {}\n", msg);
    }
}

export inline void LOG_ERROR(const String &msg) {
    if (IS_LOGGER_INITIALIZED()) {
        hybridsearch_logger->error(msg);
    } else {
        fmt::print("[error] {}\n", msg);
    }
}

export inline void LOG_CRITICAL(const String &msg) {
    if (IS_LOGGER_INITIALIZED()) {
        hybridsearch_logger->critical(msg);
    } else {
        fmt::print("[critical] {}\n", msg);
    }
}

} // namespace hybridsearch
