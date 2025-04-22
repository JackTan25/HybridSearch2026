

#pragma once

#include <cstdint>
#include <exception>
#include <source_location>
#include <string>

namespace hybridsearch {

class ParserException : public std::exception {
public:
    explicit ParserException(std::string message) : message_(std::move(message)) {}
    [[nodiscard]] inline const char *what() const noexcept override { return message_.c_str(); }

private:
    std::string message_;
};

#ifdef hybridsearch_DEBUG
inline void ParserAssert(bool is_true,
                         const std::string &message,
                         const char *file_name = std::source_location::current().file_name(),
                         uint32_t line = std::source_location::current().line()) {
    if (!(is_true)) {
        std::string path = file_name;
        const auto pos = path.find("/src/");
        if (pos != std::string::npos) {
            path = path.substr(pos + 1);
        }

        std::string errmsg = std::string(message) + " @" + path + ":" + std::to_string(line);
        throw ParserException(errmsg);
    }
}
#else

inline void ParserAssert(bool is_true, const std::string &message) {
    if (!(is_true)) {
        throw ParserException(message);
    }
}
#endif

#define ParserError(message) ParserAssert(false, message)

} // namespace hybridsearch
