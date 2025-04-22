

#pragma once

#include "parser_assert.h"
#include "type/logical_type.h"

namespace hybridsearch {

struct DecimalType {
public:
    DecimalType() = default;

    DecimalType(int64_t val1, int64_t val2) : upper(val1), lower(val2) {}

    bool operator==(const DecimalType &other) const {
        if (this == &other)
            return true;
        return other.upper == upper && other.lower == lower;
    }

    inline void Reset() {
        upper = 0;
        lower = 0;
    }

    [[nodiscard]] inline std::string ToString() const {
        ParserError("ToString() isn't implemented");
        return std::string();
    }

    [[nodiscard]] auto GetHash() const { return std::hash<int64_t>{}(upper) ^ std::hash<int64_t>{}(lower); }

    int64_t upper{0};
    int64_t lower{0};
};

} // namespace hybridsearch
