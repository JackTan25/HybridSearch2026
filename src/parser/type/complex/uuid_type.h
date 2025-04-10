

#pragma once

#include <cstring>
#include <string>

namespace hybridsearch {

struct UuidType {
public:
    static constexpr int64_t LENGTH = 16;

public:
    inline UuidType() = default;

    explicit UuidType(const char *input) { Set(input); }

    UuidType(const UuidType &other);

    UuidType(UuidType &&other) noexcept;

    UuidType &operator=(const UuidType &other);

    UuidType &operator=(UuidType &&other) noexcept;

    bool operator==(const UuidType &other) const;

    inline bool operator!=(const UuidType &other) const { return !operator==(other); }

    inline void Set(const char *input) { memcpy(body, input, UuidType::LENGTH); }

    [[nodiscard]] inline std::string ToString() const { return std::string(body, UuidType::LENGTH); }

    inline void Reset() { memset(body, 0, UuidType::LENGTH); }

public:
    char body[UuidType::LENGTH];
};

} // namespace hybridsearch
