//
// Created by Dogs-Cute on 4/15/2023.
//

#ifndef HOOT_HOVER_EXTMATH_HPP
#define HOOT_HOVER_EXTMATH_HPP

#include "ExtBase.h"

NS_BEGIN
#define SIGNED(ret_tp, spec...) template<typename Arith> spec typename std::enable_if<std::is_signed<Arith>::value,ret_tp>::type
#define UNSIGNED(ret_tp, spec...) template<typename Arith> spec typename std::enable_if<std::is_unsigned<Arith>::value,ret_tp>::type
#define NUMBER(ret_tp, spec...) template<typename Arith> spec typename std::enable_if<std::is_arithmetic<Arith>::value,ret_tp>::type
#define sign(arith) sign_of(arith)
#define nsign(arith) (-sign_of(arith))
#define p1 (signed char)1
#define n1 (signed char)(-1)

    template<typename NumType>
    constexpr typename std::enable_if<std::is_floating_point<NumType>::value, bool>::type
    operator==(NumType x, NumType y) {
        return abs(x - y) <= epsilon_of(NumType);
    }

    template<typename NumType>
    constexpr typename std::enable_if<std::is_integral<NumType>::value, bool>::type
    eq0(NumType x) {
        return x == 0;
    }

    template<typename NumType>
    constexpr typename std::enable_if<std::is_floating_point<NumType>::value, bool>::type
    eq0(NumType x) {
        return abs(x) <= epsilon_of(NumType);
    }

    template<typename NumType>
    inline constexpr typename std::enable_if<std::is_integral<NumType>::value, NumType>::type
    to_times(NumType x, NumType base) noexcept {
        return x - x % base;
    }

    template<typename NumType>
    inline constexpr typename std::enable_if<std::is_floating_point<NumType>::value, NumType>::type
    to_times(NumType x, NumType base) noexcept {
        return static_cast<long long>(x / base) * base;
    }

    template<typename NumType>
    inline constexpr typename std::enable_if<std::is_integral<NumType>::value, NumType>::type
    to_times(NumType x, NumType add, NumType base) noexcept {
        return x - x % base + add * base;
    }

    template<typename NumType>
    inline constexpr typename std::enable_if<std::is_floating_point<NumType>::value, NumType>::type
    to_times(NumType x, long long add, NumType base) noexcept {
        return (static_cast<long long>(x / base) + add) * base;
    }
/*
    class Sign final {
        bool pos, nonzero;

        Sign(bool pos, bool nonzero) noexcept: pos(pos), nonzero(nonzero) {}

        SIGNED(Sign, friend) sign_of(Arith arith) noexcept;

        UNSIGNED(Sign, friend) sign_of(Arith arith) noexcept;

        friend Sign sign_if(bool cond) noexcept;

    public:
        void operator-() noexcept {
            pos = !pos;
        }

        SIGNED(Arith) operator*(Arith arith) const noexcept {
            if (nonzero) {
                if (pos) return arith;
                else return -arith;
            } else return 0;
        }

        UNSIGNED(Arith) operator*(Arith arith) const noexcept {
            if (nonzero) return arith;
            return 0;
        }

        constexpr bool operator==(const Sign &s) const noexcept {
            return pos == s.pos || !(nonzero && s.nonzero);
        }

        constexpr bool operator!=(const Sign &s) const noexcept {
            return pos != s.pos && (nonzero || s.nonzero);
        }

        bool operator<(const Sign &s) const noexcept {
            if (nonzero) {
                if (s.nonzero)
                    return pos < s.pos;
                else
                    return !pos;
            } else {
                if (s.nonzero)
                    return s.pos;
                else
                    return false;
            }
        }

        bool operator<=(const Sign &s) const noexcept {
            if (nonzero) {
                if (s.nonzero)
                    return !pos || s.pos;
                else
                    return !pos;
            } else {
                if (s.nonzero)
                    return s.pos;
                else
                    return true;
            }
        }

        bool operator>(const Sign &s) const noexcept {
            if (nonzero) {
                if (s.nonzero)
                    return pos > s.pos;
                else
                    return pos;
            } else {
                if (s.nonzero)
                    return !s.pos;
                else
                    return false;
            }
        }

        bool operator>=(const Sign &s) const noexcept {
            if (nonzero) {
                if (s.nonzero)
                    return pos || !s.pos;
                else
                    return pos;
            } else {
                if (s.nonzero)
                    return !s.pos;
                else
                    return true;
            }
        }

        Sign operator*(const Sign &s) const noexcept {
            return {!(pos ^ s.pos), nonzero && s.nonzero};
        }

        Sign &operator*=(const Sign &s) noexcept {
            pos = !(pos ^ s.pos);
            nonzero &= s.nonzero;
            return *this;
        }

        template<typename Arith>
        constexpr explicit operator Arith() const noexcept {
            static_assert(std::is_signed<Arith>::value,
                          "Sign conversions must be a signed type");
            return nonzero ? (pos ? 1 : -1) : 0;
        }

        [[nodiscard]] constexpr

        bool is_nonzero() const noexcept {
            return nonzero;
        }

        [[nodiscard]] constexpr bool is_zero() const noexcept {
            return !nonzero;
        }

        [[nodiscard]] constexpr bool is_positive() const noexcept {
            return pos;
        }

        [[nodiscard]] constexpr bool is_not_positive() const noexcept {
            return !(nonzero && pos);
        }

        [[nodiscard]] constexpr bool is_not_negative() const noexcept {
            return !nonzero || pos;
        }

        [[nodiscard]] constexpr bool is_negative() const noexcept {
            return !pos;
        }

        void invert() noexcept {
            pos = !pos;
        }
    };

    SIGNED(Arith) operator*(Arith arith, Sign s) noexcept {
        return s * arith;
    }

    UNSIGNED(Arith) operator*(Arith arith, Sign s) noexcept {
        return s * arith;
    }

    SIGNED(Sign) sign_of(Arith arith) noexcept {
        return Sign{arith < 0, arith == 0};
    }

    UNSIGNED(Sign) sign_of(Arith arith) noexcept {
        return Sign{false, arith == 0};
    }

    Sign sign_if(bool cond) noexcept {
        return {cond, false};
    }
*/
    SIGNED(Arith, constexpr) sign_of(Arith arith) {
        return arith > 0 ? 1 : arith < 0 ? -1 : 0;
    }

    UNSIGNED(Arith, constexpr) sign_of(Arith arith) {
        return arith > 0 ? 1 : 0;
    }

    NUMBER(Arith) sign_if(bool positive) {
        return positive ? 1 : -1;
    }

    NUMBER(Arith) sign_if(bool positive, bool is_0) {
        return is_0 ? 0 : positive ? 1 : -1;
    }

    SIGNED(signed char, constexpr) sign_of_sc(Arith arith) {
        return arith > 0 ? 1 : arith < 0 ? -1 : 0;
    }

    UNSIGNED(signed char, constexpr) sign_of_sc(Arith arith) {
        return arith > 0 ? 1 : 0;
    }

    inline signed char sign_if_sc(bool positive) {
        return positive ? 1 : -1;
    }

    SIGNED(Arith, constexpr) sign_mov(Arith sign, Arith value) {
        return sign > 0 ? value : sign < 0 ? -value : 0;
    }

    UNSIGNED(Arith, constexpr) sign_mov(Arith sign, Arith value) {
        return sign > 0 ? value : 0;
    }

    SIGNED(Arith) sign_mov(std::initializer_list<Arith> signs, Arith value) {
        bool neg = false;
        for (auto sign: signs)
            switch (sign_of<Arith>(sign)) {
                case 1:
                    break;
                case 0:
                    return 0;
                case -1:
                    neg = !neg;
                    break;
            }
        return neg ? -value : value;
    }

    UNSIGNED(Arith, constexpr) sign_mov(std::initializer_list<Arith> signs, Arith value) {
        return std::any_of(signs.begin(), signs.end(), eq0) ? 0 : value;
    }

    NUMBER(Arith, constexpr) sign_if_mov(bool positive, Arith arith) {
        return positive ? arith : -arith;
    }

    NUMBER(Arith, constexpr) sign_if_mov(bool positive, bool is_0, Arith arith) {
        return is_0 ? 0 : positive ? arith : -arith;
    }


#undef SIGNED
#undef UNSIGNED
#undef p1
#undef n1

    template<bool _canBeNeg = true>
    typename std::enable_if<_canBeNeg, double>::type
    round(double rad) {
        return rad - to_times(rad, 1, M_PI);
    }

    template<bool _canBeNeg = true>
    typename std::enable_if<!_canBeNeg, double>::type
    round(double rad) {
        return rad - to_times(rad, M_PI);
    }

    inline constexpr double radians(double x) noexcept {
        return x * M_PI / 180;
    }

    inline constexpr double degrees(double x) noexcept {
        return x * 180 / M_PI;
    }

    // Convert radian to slope or slope to radian(clockwise to a coordinate system)
    class RadSlope {
        double _rad;
        ArithPoint<double> _slope;

        void cal_slope() noexcept {
            if (_rad == M_PI_2)
                _slope = {0, 1};
            else if (_rad == -M_PI_2)
                _slope = {0, -1};
            else
                _slope = {sign_if<double>(abs(_rad) < 90), tan(_rad)};
        }

        [[nodiscard]] constexpr double get_rad() const noexcept {
            return atan2(_slope.y, _slope.x);
        }

        void cal_rad() noexcept {
            _rad = get_rad();
        }

    public:
        RadSlope() : _rad(0), _slope(1, 0) {}

        explicit RadSlope(double rad) noexcept: _rad(rad) {
            cal_slope();
        }

        explicit RadSlope(const ArithPoint<double> &slope) noexcept: _rad(get_rad()), _slope(slope) {}

        void set_slope(const ArithPoint<double> &slope) {
            _slope = slope;
            cal_rad();
        }

        void set_rad(double rad) {
            _rad = rad;
            cal_slope();
        }

        void rot(double rad) {
            _rad = round(_rad + rad);
            cal_slope();
        }

        [[nodiscard]] constexpr const ArithPoint<double> &slope() const noexcept {
            return _slope;
        }

        [[nodiscard]] constexpr const double &rad() const noexcept {
            return _rad;
        }

        [[nodiscard]] constexpr double angle() const noexcept {
            return degrees(_rad);
        }

        template<typename NumType>
        [[nodiscard]] typename std::enable_if<std::is_arithmetic<NumType>::value, ArithPoint<double>>::type
        operator*(NumType num) const noexcept {
            return {cos(_rad) * num, sin(_rad) * num};
        }
    };

    template<typename NumType>
    struct Ok {
        NumType value;
        bool ok;

        constexpr operator bool() const noexcept { // NOLINT(google-explicit-constructor)
            return ok;
        }
    };

    template<typename NumType>
    typename std::enable_if<std::is_unsigned<NumType>::value, Ok<NumType>>::type
    add_ok(NumType a, NumType b) noexcept {
        NumType sum = a + b;
        return {sum, (sum > a) && (sum > b)};
    }

    template<typename NumType>
    typename std::enable_if<std::is_signed<NumType>::value, Ok<NumType>>::type
    add_ok(NumType a, NumType b) noexcept {
        NumType sign_a = sign_of(a), sign_b = sign_of(b);
        NumType sum = a + b;
        return {sum, (sign_a != sign_b) || (sign_a == sign_of(sum))};
    }

    template<typename NumType>
    typename std::enable_if<std::is_unsigned<NumType>::value, Ok<NumType>>::type
    sub_ok(NumType a, NumType b) noexcept {
        return {NumType(a - b), a > b};
    }

    template<typename NumType>
    typename std::enable_if<std::is_signed<NumType>::value, Ok<NumType>>::type
    sub_ok(NumType a, NumType b) noexcept {
        return add_ok(a, -b);
    }
NS_END

#endif //HOOT_HOVER_EXTMATH_HPP
