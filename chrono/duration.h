/**
 * @file
 *
 * @brief Chrono support for Network Simulation Frameworks.
 *
 * @version 1.1
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2015-02-16
 *
 * @copyright Copyright (c) 2015.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef DURATION_H__E90A15BC_96A8_4FA8_99A1_E3DC5B5A2A66
#define DURATION_H__E90A15BC_96A8_4FA8_99A1_E3DC5B5A2A66


#include <nsfx/chrono/config.h>
#include <boost/functional/hash.hpp>
#include <boost/core/swap.hpp>
#include <sstream>
#include <iomanip>
#include <limits>


NSFX_CHRONO_OPEN_NAMESPACE


/**
 * @ingroup Chrono
 * @brief Time duration.
 *
 * Represents a time duration.<br/>
 * The time resolution is 1 nanosecond.<br/>
 * Internally, a 64-bit signed integer is used to represent the time duration.<br/>
 * Technically, the duration in nanoseconds represented by a 64-bit signed
 * integer is about -106,751 to +106,751 days, or -292 to 292 years.
 */
class Duration
{
    // Typedefs./*{{{*/
public:
    typedef int64_t  ValueType;

    BOOST_STATIC_CONSTANT(ValueType,  NANO_SECOND =                1    );
    BOOST_STATIC_CONSTANT(ValueType, MICRO_SECOND =  NANO_SECOND * 1000 );
    BOOST_STATIC_CONSTANT(ValueType, MILLI_SECOND = MICRO_SECOND * 1000 );
    BOOST_STATIC_CONSTANT(ValueType,       SECOND = MILLI_SECOND * 1000 );
    BOOST_STATIC_CONSTANT(ValueType,       MINUTE =       SECOND * 60   );
    BOOST_STATIC_CONSTANT(ValueType,         HOUR =       MINUTE * 60   );
    BOOST_STATIC_CONSTANT(ValueType,          DAY =         HOUR * 24   );

    /*}}}*/

    // Constructors. /*{{{*/
public:
    BOOST_CONSTEXPR Duration(void) BOOST_NOEXCEPT :
        value_(0) {}

    explicit BOOST_CONSTEXPR Duration(const ValueType& ns) BOOST_NOEXCEPT :
        value_(ns) {}

    BOOST_CONSTEXPR Duration(const ValueType& us, const ValueType& ns) BOOST_NOEXCEPT :
        value_(us * MICRO_SECOND + ns) {}

    BOOST_CONSTEXPR Duration(const ValueType& ms, const ValueType& us, const ValueType& ns) BOOST_NOEXCEPT :
        value_(ms * MILLI_SECOND + us * MICRO_SECOND + ns) {}

    BOOST_CONSTEXPR Duration(const ValueType& sc,
                             const ValueType& ms, const ValueType& us, const ValueType& ns) BOOST_NOEXCEPT :
        value_(sc * SECOND +
             ms * MILLI_SECOND + us * MICRO_SECOND + ns) {}

    BOOST_CONSTEXPR Duration(const ValueType& mn, const ValueType& sc,
                             const ValueType& ms, const ValueType& us, const ValueType& ns) BOOST_NOEXCEPT :
        value_(mn * MINUTE + sc * SECOND +
             ms * MILLI_SECOND + us * MICRO_SECOND + ns) {}

    BOOST_CONSTEXPR Duration(const ValueType& hr, const ValueType& mn, const ValueType& sc,
                             const ValueType& ms, const ValueType& us, const ValueType& ns) BOOST_NOEXCEPT :
        value_(hr * HOUR + mn * MINUTE + sc * SECOND +
             ms * MILLI_SECOND + us * MICRO_SECOND + ns) {}

    BOOST_CONSTEXPR Duration(const ValueType& dy,
                             const ValueType& hr, const ValueType& mn, const ValueType& sc,
                             const ValueType& ms, const ValueType& us, const ValueType& ns) BOOST_NOEXCEPT :
        value_(dy * DAY +
             hr * HOUR + mn * MINUTE + sc * SECOND +
             ms * MILLI_SECOND + us * MICRO_SECOND + ns) {}

    /*}}}*/

    // Comparisons./*{{{*/
public:
    friend BOOST_CONSTEXPR bool operator< (const Duration& lhs, const Duration& rhs) BOOST_NOEXCEPT;
    friend BOOST_CONSTEXPR bool operator<=(const Duration& lhs, const Duration& rhs) BOOST_NOEXCEPT;
    friend BOOST_CONSTEXPR bool operator==(const Duration& lhs, const Duration& rhs) BOOST_NOEXCEPT;
    friend BOOST_CONSTEXPR bool operator!=(const Duration& lhs, const Duration& rhs) BOOST_NOEXCEPT;
    friend BOOST_CONSTEXPR bool operator> (const Duration& lhs, const Duration& rhs) BOOST_NOEXCEPT;
    friend BOOST_CONSTEXPR bool operator>=(const Duration& lhs, const Duration& rhs) BOOST_NOEXCEPT;

    /*}}}*/

    // Algorithms./*{{{*/
public:
    /**
     * @brief Return the days part, and keep the remaining duration.
     *
     * Performance a division operation by the duration of a day, the number
     * of days are returned, and the duration is changed to the modulus.
     * The remaining time duration is always non-negative, and less than
     * the duration of a day.
     *
     * Time is usually kept in a uniform format:
     * D HH:MM:SS.mmm,uuu,nnn, where
     * HH, MM, SS, mmu, uuu, nnn are non-negative values,
     * and represent a duration less than a day.
     *
     * The remaining value the duration represents HH:MM:SS.mmm,uuu,nnn,
     * and D is returned.
     */
    BOOST_CONSTEXPR ValueType ModuloDay(void) BOOST_NOEXCEPT;

    friend BOOST_CONSTEXPR Duration  operator+(const Duration& lhs, const Duration& rhs) BOOST_NOEXCEPT;
    friend BOOST_CONSTEXPR Duration  operator-(const Duration& lhs, const Duration& rhs) BOOST_NOEXCEPT;
    friend BOOST_CONSTEXPR Duration  operator*(const Duration& lhs, const ValueType& n ) BOOST_NOEXCEPT;
    friend BOOST_CONSTEXPR Duration  operator*(const ValueType& n,  const Duration& rhs) BOOST_NOEXCEPT;
    friend BOOST_CONSTEXPR ValueType operator/(const Duration& lhs, const Duration& rhs) BOOST_NOEXCEPT;
    friend BOOST_CONSTEXPR Duration  operator/(const Duration& lhs, const ValueType& n ) BOOST_NOEXCEPT;
    friend BOOST_CONSTEXPR Duration  operator%(const Duration& lhs, const Duration& rhs) BOOST_NOEXCEPT;

    BOOST_CONSTEXPR Duration& operator+=(const Duration& rhs) BOOST_NOEXCEPT
    {
        value_ += rhs.value_;
        return *this;
    }

    BOOST_CONSTEXPR Duration& operator-=(const Duration& rhs) BOOST_NOEXCEPT
    {
        value_ -= rhs.value_;
        return *this;
    }

    BOOST_CONSTEXPR Duration& operator*=(const ValueType& n) BOOST_NOEXCEPT
    {
        value_ *= n;
        return *this;
    }

    BOOST_CONSTEXPR Duration& operator/=(const ValueType& n) BOOST_NOEXCEPT
    {
        value_ /= n;
        return *this;
    }

    BOOST_CONSTEXPR Duration& operator%=(const Duration& rhs) BOOST_NOEXCEPT
    {
        value_ %= rhs.value_;
        return *this;
    }

    BOOST_CONSTEXPR Duration operator-(void) BOOST_NOEXCEPT
    {
        return Duration(-value_);
    }

    BOOST_CONSTEXPR bool operator!(void) BOOST_NOEXCEPT
    {
        return !value_;
    }

    /*}}}*/

    // Methods./*{{{*/
    /**
     * @brief Get the internal value.
     */
    BOOST_CONSTEXPR ValueType GetValue(void) const BOOST_NOEXCEPT
    {
        return value_;
    }

    /**
     * @brief Convert to nanoseconds.
     */
    BOOST_CONSTEXPR ValueType ToNanoSeconds(void) const BOOST_NOEXCEPT
    {
        return value_;
    }

    friend size_t hash_value(const Duration& d) BOOST_NOEXCEPT;

    void swap(Duration& rhs) BOOST_NOEXCEPT
    {
        boost::swap(value_, rhs.value_);
    }

    /*}}}*/

    // I/O./*{{{*/
public:
    /**
     * @brief Convert to formatted string "D HH:MM:SS.mmm.uuu.nnn".
     *
     * The "D" days part is omitted, if the duration is less than a day.
     */
    std::string ToString(void) const;

    /**
     * @brief Convert from formatted string "D HH:MM:SS.mmm.uuu.nnn".
     */
    void FromString(const std::string& str);

    /*}}}*/

    // Limits./*{{{*/
    static Duration Zero(void) BOOST_NOEXCEPT
    {
        return Duration();
    }

    static Duration Min(void) BOOST_NOEXCEPT
    {
        return Duration(std::numeric_limits<ValueType>::min());
    }

    static Duration Max(void) BOOST_NOEXCEPT
    {
        return Duration(std::numeric_limits<ValueType>::max());
    }

    /*}}}*/

private:
    ValueType value_;
};


////////////////////////////////////////////////////////////////////////////////
// Comparisons./*{{{*/
inline BOOST_CONSTEXPR bool
operator< (const Duration& lhs, const Duration& rhs) BOOST_NOEXCEPT
{
    return lhs.value_ < rhs.value_;
}

inline BOOST_CONSTEXPR bool
operator<=(const Duration& lhs, const Duration& rhs) BOOST_NOEXCEPT
{
    return lhs.value_ <= rhs.value_;
}

inline BOOST_CONSTEXPR bool
operator==(const Duration& lhs, const Duration& rhs) BOOST_NOEXCEPT
{
    return lhs.value_ == rhs.value_;
}

inline BOOST_CONSTEXPR bool
operator!=(const Duration& lhs, const Duration& rhs) BOOST_NOEXCEPT
{
    return lhs.value_ != rhs.value_;
}

inline BOOST_CONSTEXPR bool
operator> (const Duration& lhs, const Duration& rhs) BOOST_NOEXCEPT
{
    return lhs.value_ > rhs.value_;
}

inline BOOST_CONSTEXPR bool
operator>=(const Duration& lhs, const Duration& rhs) BOOST_NOEXCEPT
{
    return lhs.value_ >= rhs.value_;
}
/*}}}*/


////////////////////////////////////////////////////////////////////////////////
// Algorithms./*{{{*/
inline BOOST_CONSTEXPR Duration::ValueType
Duration::ModuloDay(void) BOOST_NOEXCEPT
{
    ValueType days = 0;
    if (value_ >= DAY)
    {
        days = static_cast<ValueType>(value_ / DAY);
        value_ %= DAY;
    }
    else if (value_ < 0)
    {
        days = static_cast<ValueType>(value_ / DAY);
        value_ -= days * DAY;
        if (value_ < 0)
        {
            --days;
            value_ += DAY;
        }
    }
    return days;
}

inline BOOST_CONSTEXPR Duration
operator+ (const Duration& lhs, const Duration& rhs) BOOST_NOEXCEPT
{
return Duration(lhs.value_ + rhs.value_);
}

inline BOOST_CONSTEXPR Duration
operator- (const Duration& lhs, const Duration& rhs) BOOST_NOEXCEPT
{
    return Duration(lhs.value_ - rhs.value_);
}

inline BOOST_CONSTEXPR Duration
operator* (const Duration& lhs, const Duration::ValueType& n) BOOST_NOEXCEPT
{
    return Duration(lhs.value_ * n);
}

inline BOOST_CONSTEXPR Duration
operator* (const Duration::ValueType& n, const Duration& rhs) BOOST_NOEXCEPT
{
    return Duration(n * rhs.value_);
}

inline BOOST_CONSTEXPR Duration
operator/ (const Duration& lhs, const Duration::ValueType& n) BOOST_NOEXCEPT
{
    return Duration(lhs.value_ / n);
}

inline BOOST_CONSTEXPR Duration::ValueType
operator/ (const Duration& lhs, const Duration& rhs) BOOST_NOEXCEPT
{
    return lhs.value_ / rhs.value_;
}

inline BOOST_CONSTEXPR Duration
operator% (const Duration& lhs, const Duration& rhs) BOOST_NOEXCEPT
{
    return Duration(lhs.value_ % rhs.value_);
}
/*}}}*/


////////////////////////////////////////////////////////////////////////////////
// Methods./*{{{*/
/**
 * @brief Supports <code>boost::hash<Duration></code>.
 */
inline size_t
hash_value(const Duration& d) BOOST_NOEXCEPT
{
    return boost::hash<Duration::ValueType>()(d.value_);
}

inline void
swap(Duration& lhs, Duration& rhs) BOOST_NOEXCEPT
{
    lhs.swap(rhs);
}

/*}}}*/


////////////////////////////////////////////////////////////////////////////////
// I/O./*{{{*/
inline std::string
Duration::ToString(void) const
{
    std::stringstream oss;
    ValueType rep = value_;
    bool neg = rep < 0;
    if (neg)
    {
        oss << '-';
    }

    ValueType dy =  rep / DAY;
    ValueType hr = (rep % DAY) / HOUR;
    ValueType mn = (rep % HOUR) / MINUTE;
    ValueType sc = (rep % MINUTE) / SECOND;
    ValueType ms = (rep % SECOND) / MILLI_SECOND;
    ValueType us = (rep % MILLI_SECOND) / MICRO_SECOND;
    ValueType ns = (rep % MICRO_SECOND) / NANO_SECOND;

    if (dy)
    {
        oss << std::setw(0) << (neg ? -dy : dy) << " ";
    }
    oss << std::setw(2) << std::setfill('0') << (neg ? -hr : hr) << ":"
        << std::setw(2) << std::setfill('0') << (neg ? -mn : mn) << ":"
        << std::setw(2) << std::setfill('0') << (neg ? -sc : sc) << "."
        << std::setw(3) << std::setfill('0') << (neg ? -ms : ms) << ","
        << std::setw(3) << std::setfill('0') << (neg ? -us : us) << ","
        << std::setw(3) << std::setfill('0') << (neg ? -ns : ns);
    return oss.str();
}

template<class CharT, class TraitsT>
inline std::basic_ostream<CharT, TraitsT>&
operator<<(std::basic_ostream<CharT, TraitsT>& os, const Duration& rhs)
{
    return (os << rhs.ToString());
}
/*}}}*/


////////////////////////////////////////////////////////////////////////////////
// Makers./*{{{*/
inline BOOST_CONSTEXPR Duration
NanoSeconds(Duration::ValueType n) BOOST_NOEXCEPT
{
    return Duration(n * Duration::NANO_SECOND);
}

inline BOOST_CONSTEXPR Duration
MicroSeconds(Duration::ValueType n) BOOST_NOEXCEPT
{
    return Duration(n * Duration::MICRO_SECOND);
}

inline BOOST_CONSTEXPR Duration
MilliSeconds(Duration::ValueType n) BOOST_NOEXCEPT
{
    return Duration(n * Duration::MILLI_SECOND);
}

inline BOOST_CONSTEXPR Duration
Seconds(Duration::ValueType n) BOOST_NOEXCEPT
{
    return Duration(n * Duration::SECOND);
}

inline BOOST_CONSTEXPR Duration
Minutes(Duration::ValueType n) BOOST_NOEXCEPT
{
    return Duration(n * Duration::MINUTE);
}

inline BOOST_CONSTEXPR Duration
Hours(Duration::ValueType n) BOOST_NOEXCEPT
{
    return Duration(n * Duration::HOUR);
}

inline BOOST_CONSTEXPR Duration
Days(Duration::ValueType n) BOOST_NOEXCEPT
{
    return Duration(n * Duration::DAY);
}
/*}}}*/


NSFX_CHRONO_CLOSE_NAMESPACE


#endif // DURATION_H__E90A15BC_96A8_4FA8_99A1_E3DC5B5A2A66

