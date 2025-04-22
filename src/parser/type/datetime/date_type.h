

#pragma once

#include "interval_type.h"
#include <string>

namespace hybridsearch {

struct DateType {
    friend struct DateTimeType;

    DateType() = default;

    explicit constexpr DateType(int32_t date_value) : value(date_value) {};

    inline int32_t GetValue() const { return value; }

    operator int32_t() const { return value; }

    inline void FromString(const std::string_view &date_str) { FromString(date_str.data(), date_str.size()); }

    void FromString(const char *date, size_t length);

    void FromString(const char *date, size_t length, size_t &end_length);

    [[nodiscard]] std::string ToString() const;

    int32_t value{0};

    int64_t GetEpochTime() const;

private:
    static bool ConvertFromString(const char *date_ptr, size_t length, DateType &date, size_t &end_length);

    static bool YMD2Date(int32_t year, int32_t month, int32_t day, DateType &date);

    static bool Date2YMD(int32_t days, int32_t &year, int32_t &month, int32_t &day);

    inline static bool IsLeapYear(int32_t year) { return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0); }

    static bool IsDateValid(int32_t year, int32_t month, int32_t day);

public:
    inline bool operator==(const DateType &other) const { return this->value == other.value; }

    inline bool operator>=(const DateType &other) const { return this->value >= other.value; }

    inline bool operator>(const DateType &other) const { return this->value > other.value; }

    inline bool operator<=(const DateType &other) const { return this->value <= other.value; }

    inline bool operator<(const DateType &other) const { return this->value < other.value; }

public:
    // Operation
    static bool Add(DateType input, IntervalType interval, DateType &output);

    static bool Subtract(DateType input, IntervalType interval, DateType &output);

    static int64_t GetDatePart(DateType input, TimeUnit unit);

    static bool OuterDate2YMD(DateType input, int32_t &year, int32_t &month, int32_t &day);
};

} // namespace hybridsearch