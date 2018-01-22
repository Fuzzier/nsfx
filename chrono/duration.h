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
    typedef int64_t  Rep;

    BOOST_STATIC_CONSTANT(Rep,  NANO_SECOND =                1    );
    BOOST_STATIC_CONSTANT(Rep, MICRO_SECOND =  NANO_SECOND * 1000 );
    BOOST_STATIC_CONSTANT(Rep, MILLI_SECOND = MICRO_SECOND * 1000 );
    BOOST_STATIC_CONSTANT(Rep,       SECOND = MILLI_SECOND * 1000 );
    BOOST_STATIC_CONSTANT(Rep,       MINUTE =       SECOND * 60   );
    BOOST_STATIC_CONSTANT(Rep,         HOUR =       MINUTE * 60   );
    BOOST_STATIC_CONSTANT(Rep,          DAY =         HOUR * 24   );

    /*}}}*/

    // Constructors. /*{{{*/
public:
    BOOST_CONSTEXPR Duration(void) BOOST_NOEXCEPT :
        rep_(0) {}

    explicit BOOST_CONSTEXPR Duration(const Rep& ns) BOOST_NOEXCEPT :
        rep_(ns) {}

    BOOST_CONSTEXPR Duration(const Rep& us, const Rep& ns) BOOST_NOEXCEPT :
        rep_(us * MICRO_SECOND + ns) {}

    BOOST_CONSTEXPR Duration(const Rep& ms, const Rep& us, const Rep& ns) BOOST_NOEXCEPT :
        rep_(ms * MILLI_SECOND + us * MICRO_SECOND + ns) {}

    BOOST_CONSTEXPR Duration(const Rep& sc,
                             const Rep& ms, const Rep& us, const Rep& ns) BOOST_NOEXCEPT :
        rep_(sc * SECOND +
             ms * MILLI_SECOND + us * MICRO_SECOND + ns) {}

    BOOST_CONSTEXPR Duration(const Rep& mn, const Rep& sc,
                             const Rep& ms, const Rep& us, const Rep& ns) BOOST_NOEXCEPT :
        rep_(mn * MINUTE + sc * SECOND +
             ms * MILLI_SECOND + us * MICRO_SECOND + ns) {}

    BOOST_CONSTEXPR Duration(const Rep& hr, const Rep& mn, const Rep& sc,
                             const Rep& ms, const Rep& us, const Rep& ns) BOOST_NOEXCEPT :
        rep_(hr * HOUR + mn * MINUTE + sc * SECOND +
             ms * MILLI_SECOND + us * MICRO_SECOND + ns) {}

    BOOST_CONSTEXPR Duration(const Rep& dy,
                             const Rep& hr, const Rep& mn, const Rep& sc,
                             const Rep& ms, const Rep& us, const Rep& ns) BOOST_NOEXCEPT :
        rep_(dy * DAY +
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
    BOOST_CONSTEXPR Rep ModuloDay(void) BOOST_NOEXCEPT;

    friend BOOST_CONSTEXPR Duration operator+ (const Duration& lhs, const Duration& rhs) BOOST_NOEXCEPT;
    friend BOOST_CONSTEXPR Duration operator- (const Duration& lhs, const Duration& rhs) BOOST_NOEXCEPT;
    friend BOOST_CONSTEXPR Duration operator* (const Duration& lhs, const Rep& n       ) BOOST_NOEXCEPT;
    friend BOOST_CONSTEXPR Duration operator* (const Rep& n,        const Duration& rhs) BOOST_NOEXCEPT;
    friend BOOST_CONSTEXPR Rep      operator/ (const Duration& lhs, const Duration& rhs) BOOST_NOEXCEPT;
    friend BOOST_CONSTEXPR Duration operator/ (const Duration& lhs, const Rep& n       ) BOOST_NOEXCEPT;
    friend BOOST_CONSTEXPR Duration operator% (const Duration& lhs, const Duration& rhs) BOOST_NOEXCEPT;

    BOOST_CONSTEXPR Duration& operator+=(const Duration& rhs) BOOST_NOEXCEPT
    {
        rep_ += rhs.rep_;
        return *this;
    }

    BOOST_CONSTEXPR Duration& operator-=(const Duration& rhs) BOOST_NOEXCEPT
    {
        rep_ -= rhs.rep_;
        return *this;
    }

    BOOST_CONSTEXPR Duration& operator*=(const Rep& n) BOOST_NOEXCEPT
    {
        rep_ *= n;
        return *this;
    }

    BOOST_CONSTEXPR Duration& operator/=(const Rep& n) BOOST_NOEXCEPT
    {
        rep_ /= n;
        return *this;
    }

    BOOST_CONSTEXPR Duration& operator%=(const Duration& rhs) BOOST_NOEXCEPT
    {
        rep_ %= rhs.rep_;
        return *this;
    }

    BOOST_CONSTEXPR Duration operator-(void) BOOST_NOEXCEPT
    {
        return Duration(-rep_);
    }

    BOOST_CONSTEXPR bool operator!(void) BOOST_NOEXCEPT
    {
        return !rep_;
    }

    /*}}}*/

    // Methods./*{{{*/
    /**
     * @brief Convert to nanoseconds.
     */
    BOOST_CONSTEXPR Rep ToNanoSeconds(void) const
    {
        return rep_;
    }

    friend size_t hash_value(const Duration& d) BOOST_NOEXCEPT;

    void swap(Duration& rhs) BOOST_NOEXCEPT
    {
        boost::swap(rep_, rhs.rep_);
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
    void FromString(void);

    /*}}}*/

    // Limits./*{{{*/
    static Duration Zero(void) BOOST_NOEXCEPT
    {
        return Duration();
    }

    static Duration Min(void) BOOST_NOEXCEPT
    {
        return Duration(std::numeric_limits<Rep>::min());
    }

    static Duration Max(void) BOOST_NOEXCEPT
    {
        return Duration(std::numeric_limits<Rep>::max());
    }

    /*}}}*/

private:
    Rep rep_;
};


////////////////////////////////////////////////////////////////////////////////
// Comparisons./*{{{*/
inline BOOST_CONSTEXPR bool
operator< (const Duration& lhs, const Duration& rhs) BOOST_NOEXCEPT
{
    return lhs.rep_ < rhs.rep_;
}

inline BOOST_CONSTEXPR bool
operator<=(const Duration& lhs, const Duration& rhs) BOOST_NOEXCEPT
{
    return lhs.rep_ <= rhs.rep_;
}

inline BOOST_CONSTEXPR bool
operator==(const Duration& lhs, const Duration& rhs) BOOST_NOEXCEPT
{
    return lhs.rep_ == rhs.rep_;
}

inline BOOST_CONSTEXPR bool
operator!=(const Duration& lhs, const Duration& rhs) BOOST_NOEXCEPT
{
    return lhs.rep_ != rhs.rep_;
}

inline BOOST_CONSTEXPR bool
operator> (const Duration& lhs, const Duration& rhs) BOOST_NOEXCEPT
{
    return lhs.rep_ > rhs.rep_;
}

inline BOOST_CONSTEXPR bool
operator>=(const Duration& lhs, const Duration& rhs) BOOST_NOEXCEPT
{
    return lhs.rep_ >= rhs.rep_;
}
/*}}}*/


////////////////////////////////////////////////////////////////////////////////
// Algorithms./*{{{*/
inline BOOST_CONSTEXPR Duration::Rep
Duration::ModuloDay(void) BOOST_NOEXCEPT
{
    Rep days = 0;
    if (rep_ >= DAY)
    {
        days = static_cast<Rep>(rep_ / DAY);
        rep_ %= DAY;
    }
    else if (rep_ < 0)
    {
        days = static_cast<Rep>(rep_ / DAY);
        rep_ -= days * DAY;
        if (rep_ < 0)
        {
            --days;
            rep_ += DAY;
        }
    }
    return days;
}

inline BOOST_CONSTEXPR Duration
operator+ (const Duration& lhs, const Duration& rhs) BOOST_NOEXCEPT
{
return Duration(lhs.rep_ + rhs.rep_);
}

inline BOOST_CONSTEXPR Duration
operator- (const Duration& lhs, const Duration& rhs) BOOST_NOEXCEPT
{
    return Duration(lhs.rep_ - rhs.rep_);
}

inline BOOST_CONSTEXPR Duration
operator* (const Duration& lhs, const Duration::Rep& n) BOOST_NOEXCEPT
{
    return Duration(lhs.rep_ * n);
}

inline BOOST_CONSTEXPR Duration
operator* (const Duration::Rep& n, const Duration& rhs) BOOST_NOEXCEPT
{
    return Duration(n * rhs.rep_);
}

inline BOOST_CONSTEXPR Duration
operator/ (const Duration& lhs, const Duration::Rep& n) BOOST_NOEXCEPT
{
    return Duration(lhs.rep_ / n);
}

inline BOOST_CONSTEXPR Duration::Rep
operator/ (const Duration& lhs, const Duration& rhs) BOOST_NOEXCEPT
{
    return lhs.rep_ / rhs.rep_;
}

inline BOOST_CONSTEXPR Duration
operator% (const Duration& lhs, const Duration& rhs) BOOST_NOEXCEPT
{
    return Duration(lhs.rep_ % rhs.rep_);
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
    return boost::hash<Duration::Rep>()(d.rep_);
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
    Rep rep = rep_;
    bool neg = rep < 0;
    if (neg)
    {
        oss << '-';
    }

    Rep dy =  rep / DAY;
    Rep hr = (rep % DAY) / HOUR;
    Rep mn = (rep % HOUR) / MINUTE;
    Rep sc = (rep % MINUTE) / SECOND;
    Rep ms = (rep % SECOND) / MILLI_SECOND;
    Rep us = (rep % MILLI_SECOND) / MICRO_SECOND;
    Rep ns = (rep % MICRO_SECOND) / NANO_SECOND;

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
NanoSeconds(Duration::Rep n) BOOST_NOEXCEPT
{
    return Duration(n * Duration::NANO_SECOND);
}

inline BOOST_CONSTEXPR Duration
MicroSeconds(Duration::Rep n) BOOST_NOEXCEPT
{
    return Duration(n * Duration::MICRO_SECOND);
}

inline BOOST_CONSTEXPR Duration
MilliSeconds(Duration::Rep n) BOOST_NOEXCEPT
{
    return Duration(n * Duration::MILLI_SECOND);
}

inline BOOST_CONSTEXPR Duration
Seconds(Duration::Rep n) BOOST_NOEXCEPT
{
    return Duration(n * Duration::SECOND);
}

inline BOOST_CONSTEXPR Duration
Minutes(Duration::Rep n) BOOST_NOEXCEPT
{
    return Duration(n * Duration::MINUTE);
}

inline BOOST_CONSTEXPR Duration
Hours(Duration::Rep n) BOOST_NOEXCEPT
{
    return Duration(n * Duration::HOUR);
}

inline BOOST_CONSTEXPR Duration
Days(Duration::Rep n) BOOST_NOEXCEPT
{
    return Duration(n * Duration::DAY);
}
/*}}}*/


NSFX_CHRONO_CLOSE_NAMESPACE


#endif // DURATION_H__E90A15BC_96A8_4FA8_99A1_E3DC5B5A2A66

