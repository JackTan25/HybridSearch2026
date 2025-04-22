

#pragma once

#include <cstdint>
#include <numeric>
#include <string>

namespace hybridsearch {

constexpr uint32_t INVALID_SEGMENT = std::numeric_limits<uint32_t>::max();
constexpr uint32_t INVALID_SEGMENT_OFFSET = std::numeric_limits<uint32_t>::max();

struct RowID {
    RowID() : segment_offset_(INVALID_SEGMENT_OFFSET), segment_id_(INVALID_SEGMENT) {}

    inline explicit RowID(uint32_t segment_id, uint32_t segment_offset) : segment_offset_(segment_offset), segment_id_(segment_id) {}

    inline constexpr RowID(uint64_t row_id) : value_(row_id) {}

    inline uint64_t ToUint64() const { return value_; }

    union {
        struct {
#ifdef __BIG_ENDIAN__
            uint32_t segment_id_;
            uint32_t segment_offset_;
#else
            uint32_t segment_offset_;
            uint32_t segment_id_;
#endif
        };
        uint64_t value_;
    };

    inline bool operator<(const RowID &other) const { return value_ < other.value_; };

    inline bool operator<=(const RowID &other) const { return value_ <= other.value_; };

    inline bool operator==(const RowID &other) const { return value_ == other.value_; }
    inline bool operator!=(const RowID &other) const { return value_ != other.value_; }

    inline bool operator>(const RowID &other) const { return value_ > other.value_; };

    inline bool operator>=(const RowID &other) const { return value_ >= other.value_; };

    inline RowID &operator=(uint64_t value) {
        value_ = value;
        return *this;
    }

    inline RowID operator+(const uint32_t &docid) const { return RowID(value_ + docid); }

    inline uint32_t operator-(const RowID &other) const { return uint32_t(value_ - other.value_); }

    inline RowID operator-(const uint32_t &docid) const { return RowID(value_ - docid); }

    inline RowID &operator+=(const uint32_t &docid) {
        value_ += docid;
        return *this;
    }

    inline RowID &operator-=(const uint32_t &docid) {
        value_ -= docid;
        return *this;
    }

    inline RowID &operator++() {
        ++value_;
        return *this;
    }

    inline RowID operator++(int) {
        RowID temp = *this;
        ++value_;
        return temp;
    }

    [[nodiscard]] std::string ToString() const;

    static inline RowID FromUint64(uint64_t row_id) { return RowID(row_id); }
};

} // namespace hybridsearch

namespace std {
template <>
struct std::hash<hybridsearch::RowID> {
    std::size_t operator()(const hybridsearch::RowID &row_id) const noexcept { return std::hash<uint64_t>{}(row_id.ToUint64()); }
};
} // namespace std
