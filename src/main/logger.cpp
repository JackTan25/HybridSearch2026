

module;

module logger;

import stl;
import config;
import third_party;
import status;

namespace hybridsearch {

static SharedPtr<spdlog::sinks::stdout_color_sink_mt> stdout_sinker = nullptr;
static SharedPtr<spdlog::sinks::rotating_file_sink_mt> rotating_file_sinker = nullptr;

SharedPtr<spdlog::logger> hybridsearch_logger = nullptr;

Status Logger::Initialize(Config *config_ptr) {

    SizeT log_file_max_size = config_ptr->LogFileMaxSize();
    SizeT log_file_rotate_count = config_ptr->LogFileRotateCount();
    bool log_stdout = config_ptr->LogToStdout();

    if (rotating_file_sinker.get() == nullptr) {
        try {
            rotating_file_sinker = MakeShared<spdlog::sinks::rotating_file_sink_mt>(config_ptr->LogFilePath(),
                                                                                    log_file_max_size,
                                                                                    log_file_rotate_count); // NOLINT
        } catch (const std::exception &e) {
            String error_message = fmt::format("Error to create log sinker, cause: {}", e.what());
            fmt::print("{}", error_message);
            return Status::UnexpectedError(error_message);
        }
    }

    if (log_stdout) {
        if (stdout_sinker.get() == nullptr) {
            stdout_sinker = MakeShared<spdlog::sinks::stdout_color_sink_mt>(); // NOLINT
        }
        Vector<spdlog::sink_ptr> sinks{stdout_sinker, rotating_file_sinker};

        hybridsearch_logger = MakeShared<spdlog::logger>("hybridsearch", sinks.begin(), sinks.end()); // NOLINT
        hybridsearch_logger->set_pattern("[%H:%M:%S.%e] [%t] [%^%l%$] %v");
        hybridsearch_logger->flush_on(spdlog::level::warn);
        spdlog::details::registry::instance().register_logger(hybridsearch_logger);
    } else {
        Vector<spdlog::sink_ptr> sinks{rotating_file_sinker};
        hybridsearch_logger = MakeShared<spdlog::logger>("hybridsearch", sinks.begin(), sinks.end()); // NOLINT
        hybridsearch_logger->set_pattern("[%H:%M:%S.%e] [%t] [%^%l%$] %v");
        hybridsearch_logger->flush_on(spdlog::level::warn);
        spdlog::details::registry::instance().register_logger(hybridsearch_logger);
    }

    SetLogLevel(config_ptr->GetLogLevel());

    LOG_TRACE("Logger is initialized.");

    return Status::OK();
}

void Logger::Initialize(const LoggerConfig &config) {
    bool log_stdout = config.log_to_stdout_;
    if (rotating_file_sinker.get() == nullptr) {
        rotating_file_sinker = MakeShared<spdlog::sinks::rotating_file_sink_mt>(config.log_file_path_,
                                                                                config.log_file_max_size_,
                                                                                config.log_file_rotate_count_); // NOLINT
    }
    if (log_stdout) {
        if (stdout_sinker.get() == nullptr) {
            stdout_sinker = MakeShared<spdlog::sinks::stdout_color_sink_mt>(); // NOLINT
        }
        Vector<spdlog::sink_ptr> sinks{stdout_sinker, rotating_file_sinker};

        hybridsearch_logger = MakeShared<spdlog::logger>("hybridsearch", sinks.begin(), sinks.end()); // NOLINT
        hybridsearch_logger->set_pattern("[%H:%M:%S.%e] [%t] [%^%l%$] %v");
        spdlog::details::registry::instance().register_logger(hybridsearch_logger);
    } else {
        Vector<spdlog::sink_ptr> sinks{rotating_file_sinker};
        hybridsearch_logger = MakeShared<spdlog::logger>("hybridsearch", sinks.begin(), sinks.end()); // NOLINT
        hybridsearch_logger->set_pattern("[%H:%M:%S.%e] [%t] [%^%l%$] %v");
        spdlog::details::registry::instance().register_logger(hybridsearch_logger);
    }
    SetLogLevel(config.log_level_);
}

void Logger::Flush() {
    if (IS_LOGGER_INITIALIZED()) {
        hybridsearch_logger->flush();
    }
}

void Logger::Shutdown() {
    if (stdout_sinker.get() != nullptr or rotating_file_sinker.get() != nullptr) {
        spdlog::shutdown();
        stdout_sinker = nullptr;
        rotating_file_sinker = nullptr;
        hybridsearch_logger = nullptr;
    }
}

} // namespace hybridsearch
