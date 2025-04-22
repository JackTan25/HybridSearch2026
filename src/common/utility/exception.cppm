
module;

export module hybridsearch_exception;

import stl;
import status;

namespace hybridsearch {

export void PrintStacktrace(const String &err_msg);
export void PrintTransactionHistory();

export class RecoverableException : public std::exception {
public:
    explicit RecoverableException(Status status) : status_(std::move(status)) {}
    [[nodiscard]] inline const char *what() const noexcept override { return status_.message(); }

    RecoverableException(const RecoverableException &other) : status_(other.status_.clone()) {}

    inline ErrorCode ErrorCode() const { return status_.code(); }

    inline std::string_view ErrorMessage() const { return status_.message(); }

private:
    Status status_;
};

export class UnrecoverableException : public std::exception {
public:
    explicit UnrecoverableException(String message) : message_(std::move(message)) {}
    [[nodiscard]] inline const char *what() const noexcept override { return message_.c_str(); }

private:
    String message_;
};

#define ADD_LOG_INFO

#if defined(hybridsearch_DEBUG) || defined(ADD_LOG_INFO)

export void RecoverableError(Status status,
                             const char *file_name = std::source_location::current().file_name(),
                             u32 line = std::source_location::current().line());

export void UnrecoverableError(const String &message,
                               const char *file_name = std::source_location::current().file_name(),
                               u32 line = std::source_location::current().line());

export std::string_view GetErrorMsg(const String &message);

#else

export void RecoverableError(Status status);

export void UnrecoverableError(const String &message);

export std::string_view GetErrorMsg(const String &message);

#endif

} // namespace hybridsearch
