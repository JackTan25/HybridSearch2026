

#pragma once

#include "parser_assert.h"

namespace hybridsearch {

struct HugeInt {
public:
    HugeInt() = default;

    explicit constexpr HugeInt(int64_t upper, int64_t lower) : upper(upper), lower(lower) {}

    inline bool operator==(const HugeInt &other) const {
        if (this == &other)
            return true;
        return other.upper == upper && other.lower == lower;
    }

    inline bool operator!=(const HugeInt &other) const { return !(operator==(other)); }

    inline bool operator>(const HugeInt &other) const {
        if (this->upper > other.upper)
            return true;
        if (this->upper == other.upper && this->lower > other.lower)
            return true;
        return false;
    }

    inline bool operator<(const HugeInt &other) const {
        if (this->upper < other.upper)
            return true;
        if (this->upper == other.upper && this->lower < other.lower)
            return true;
        return false;
    }

    inline bool operator<=(const HugeInt &other) const { return !(operator>(other)); }

    inline bool operator>=(const HugeInt &other) const { return !(operator<(other)); }

    int64_t upper{};
    int64_t lower{};

public:
    [[nodiscard]] inline std::string ToString() const;

    [[nodiscard]] uint64_t GetHash() const { return std::hash<int64_t>{}(upper) ^ std::hash<int64_t>{}(lower); }

    inline void Reset() {
        upper = 0;
        lower = 0;
    }
};

std::string HugeInt::ToString() const {
    ParserError("ToString() isn't implemented");
    return std::string();
}

} // namespace hybridsearch
