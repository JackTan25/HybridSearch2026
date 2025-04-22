

#pragma once

#include "datetime_type.h"
#include "parser_assert.h"
#include <string>

namespace hybridsearch {
struct TimestampType : public DateTimeType {
    TimestampType() = default;
    explicit constexpr TimestampType(int32_t date_value, int32_t time_value) : DateTimeType(date_value, time_value) {};
    explicit constexpr TimestampType(int64_t epoch_time) : DateTimeType(epoch_time) {};
};
/*
struct TimestampType {
    TimestampType() = default;

    explicit TimestampType(int32_t date_value, int32_t time_value) : date(date_value), time(time_value) {}

    inline void Reset() {
        date = 0;
        time = 0;
    }

    [[nodiscard]] inline std::string ToString() const {
        ParserError("ToString() isn't implemented");
        return std::string();
    }

    int32_t date{};
    int32_t time{};
};
*/
} // namespace hybridsearch
