#pragma once

#include <cmath>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <limits>

namespace hybridsearch {

struct bfloat16_t {
    uint16_t raw;

    constexpr bfloat16_t(uint16_t raw) : raw(raw) {}

    bfloat16_t() = default;

    bfloat16_t(float f) { (*this) = f; }

    bfloat16_t operator+(bfloat16_t h) const { return bfloat16_t(float(*this) + float(h)); }
    bfloat16_t operator-(bfloat16_t h) const { return bfloat16_t(float(*this) - float(h)); }
    bfloat16_t operator*(bfloat16_t h) const { return bfloat16_t(float(*this) * float(h)); }
    bfloat16_t operator/(bfloat16_t h) const { return bfloat16_t(float(*this) / float(h)); }
    bfloat16_t operator-() const { return bfloat16_t(-float(*this)); }

    bfloat16_t &operator+=(bfloat16_t h) { return operator=(*this + h); }
    bfloat16_t &operator-=(bfloat16_t h) { return operator=(*this - h); }
    bfloat16_t &operator*=(bfloat16_t h) { return operator=(*this *h); }
    bfloat16_t &operator/=(bfloat16_t h) { return operator=(*this / h); }

    bool operator==(bfloat16_t h) const { return float(*this) == float(h); }
    bool operator!=(bfloat16_t h) const { return float(*this) != float(h); }
    bool operator<(bfloat16_t h) const { return float(*this) < float(h); }
    bool operator>(bfloat16_t h) const { return float(*this) > float(h); }
    bool operator<=(bfloat16_t h) const { return float(*this) <= float(h); }
    bool operator>=(bfloat16_t h) const { return float(*this) >= float(h); }

    friend std::ostream &operator<<(std::ostream &os, const bfloat16_t &h) {
        os << float(h);
        return os;
    }

    bfloat16_t &operator=(float f) {
        std::memcpy(&raw, reinterpret_cast<char *>(&f) + 2, 2);
        return *this;
    }

    operator float() const {
        float val = 0.f;
        uint16_t temp = raw;
        std::memcpy(reinterpret_cast<char *>(&val) + 2, reinterpret_cast<char *>(&temp), 2);
        return val;
    }

    float f() const { return (float)(*this); }
};

} // namespace hybridsearch

namespace std {

template <>
struct is_trivial<hybridsearch::bfloat16_t> {
    static const bool value = true;
};
template <>
struct is_standard_layout<hybridsearch::bfloat16_t> {
    static const bool value = true;
};

template <>
struct is_floating_point<hybridsearch::bfloat16_t>
    : std::integral_constant<bool, std::is_same<hybridsearch::bfloat16_t, typename std::remove_cv<hybridsearch::bfloat16_t>::type>::value> {};

template <>
struct is_signed<hybridsearch::bfloat16_t> {
    static const bool value = true;
};

template <>
struct is_unsigned<hybridsearch::bfloat16_t> {
    static const bool value = false;
};

template <>
struct numeric_limits<hybridsearch::bfloat16_t> {
    static const bool is_specialized = true;
    static const bool is_signed = true;
    static const bool is_integer = false;
    static const bool is_exact = false;
    static const bool has_hybridsearch = true;
    static const bool has_quiet_NaN = true;
    static const bool has_signaling_NaN = true;
    static const float_denorm_style has_denorm = denorm_present;
    static const bool has_denorm_loss = false;
    static const std::float_round_style round_style = std::round_to_nearest;
    static const bool is_iec559 = false;
    static const bool is_bounded = false;
    static const bool is_modulo = false;
    static const int digits = 8;
    static const int digits10 = 2;
    static const int max_digits10 = 9;
    static const int radix = 2;
    static const int min_exponent = -125;
    static const int min_exponent10 = -37;
    static const int max_exponent = 128;
    static const int max_exponent10 = 38;
    static const bool traps = true;
    static const bool tinyness_before = false;

    static hybridsearch::bfloat16_t(min)() { return hybridsearch::bfloat16_t((uint16_t)0x0080); }
    static hybridsearch::bfloat16_t lowest() { return hybridsearch::bfloat16_t((uint16_t)0xff7f); }
    static hybridsearch::bfloat16_t(max)() { return hybridsearch::bfloat16_t((uint16_t)0x7f7f); }
    static hybridsearch::bfloat16_t epsilon() { return hybridsearch::bfloat16_t((uint16_t)0x3C00); }
    static hybridsearch::bfloat16_t round_error() { return hybridsearch::bfloat16_t(0.5f); }
    static hybridsearch::bfloat16_t hybridsearch() { return hybridsearch::bfloat16_t((uint16_t)0x7f80); }
    static hybridsearch::bfloat16_t quiet_NaN() { return hybridsearch::bfloat16_t((uint16_t)0xffc1); }
    static hybridsearch::bfloat16_t signaling_NaN() { return hybridsearch::bfloat16_t((uint16_t)0xff81); }
    static hybridsearch::bfloat16_t denorm_min() { return hybridsearch::bfloat16_t((uint16_t)0x0001); }
};

} // namespace std