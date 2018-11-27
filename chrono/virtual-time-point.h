/**
 * @file
 *
 * @brief Chrono support for Network Simulation Frameworks.
 *
 * @version 2.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-13
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef TIME_POINT_H__C416325F_BBBF_4329_B747_387013318F44
#define TIME_POINT_H__C416325F_BBBF_4329_B747_387013318F44


#include <nsfx/chrono/config.h>
#include <nsfx/chrono/duration.h>
#include <nsfx/chrono/time-point.h>
#include <boost/functional/hash.hpp>
#include <boost/core/swap.hpp>


NSFX_CHRONO_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Chrono
 * @brief A virtual clock.
 *
 * This clock can be used in discreet event simulations.
 * The time resolution is 1/10 nanoseconds.
 */
struct VirtualClock
{
    typedef ratio<nano::num, nano::den * 10>  tenth_nano;
    typedef Duration<tenth_nano>  Duration;
};


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Chrono
 * @brief A time point.
 *
 * @tparam Clock The clock that defines the epoch of the time system.
 *               It shall have a nested type \c Duration, which defines
 *               the time resolution of the time system.
 *
 * This class stores the time duration elapsed since the epoch.
 * The time resolution is 1/10 nanoseconds.
 */
template<>
class TimePoint<VirtualClock, VirtualClock::Duration>
{
    // Typedefs./*{{{*/
public:
    typedef VirtualClock::Duration  Duration;
    typedef VirtualClock            Clock;
    /*}}}*/

    // Constructors. /*{{{*/
public:
    /**
     * @brief Obtain a time point at the epoch.
     */
    BOOST_CONSTEXPR TimePoint(void) BOOST_NOEXCEPT :
        dt_(0) {}

    explicit BOOST_CONSTEXPR TimePoint(const Duration& dt) BOOST_NOEXCEPT :
        dt_(dt) {}

    /*}}}*/

    // Comparison./*{{{*/
public:
    friend BOOST_CONSTEXPR bool operator< (const TimePoint& lhs, const TimePoint& rhs) BOOST_NOEXCEPT;

    friend BOOST_CONSTEXPR bool operator<=(const TimePoint& lhs, const TimePoint& rhs) BOOST_NOEXCEPT;

    friend BOOST_CONSTEXPR bool operator==(const TimePoint& lhs, const TimePoint& rhs) BOOST_NOEXCEPT;

    friend BOOST_CONSTEXPR bool operator!=(const TimePoint& lhs, const TimePoint& rhs) BOOST_NOEXCEPT;

    friend BOOST_CONSTEXPR bool operator> (const TimePoint& lhs, const TimePoint& rhs) BOOST_NOEXCEPT;

    friend BOOST_CONSTEXPR bool operator>=(const TimePoint& lhs, const TimePoint& rhs) BOOST_NOEXCEPT;

    /*}}}*/

    // Arithmetic./*{{{*/
public:
    friend BOOST_CONSTEXPR
    TimePoint operator+(const TimePoint& lhs, const Duration& rhs) BOOST_NOEXCEPT;

    friend BOOST_CONSTEXPR
    TimePoint operator+(const Duration& lhs, const TimePoint& rhs) BOOST_NOEXCEPT;

    friend BOOST_CONSTEXPR
    TimePoint operator-(const TimePoint& lhs, const Duration& rhs) BOOST_NOEXCEPT;

    friend BOOST_CONSTEXPR
    Duration operator-(const TimePoint& lhs, const TimePoint& rhs) BOOST_NOEXCEPT;

    BOOST_CONSTEXPR TimePoint& operator+=(const Duration& rhs) BOOST_NOEXCEPT
    {
        dt_ += rhs;
        return *this;
    }

    BOOST_CONSTEXPR TimePoint& operator-=(const Duration& rhs) BOOST_NOEXCEPT
    {
        dt_ -= rhs;
        return *this;
    }

    /*}}}*/

    // Methods./*{{{*/
    /**
     * @brief Get the duration relative to the epoch.
     */
    BOOST_CONSTEXPR Duration GetDuration(void) const BOOST_NOEXCEPT
    {
        return dt_;
    }

    friend size_t hash_value(const TimePoint& t) BOOST_NOEXCEPT;

    void swap(TimePoint& rhs) BOOST_NOEXCEPT
    {
        boost::swap(dt_, rhs.dt_);
    }

    /*}}}*/

    // I/O./*{{{*/
public:
    /**
     * @brief Convert to formatted string "HH:MM:SS.mmm.uuu.nnn".
     */
    std::string ToString(void) const;

    /*}}}*/

    // Limits./*{{{*/
    static TimePoint Epoch(void) BOOST_NOEXCEPT
    {
        return TimePoint();
    }

    static TimePoint Min(void) BOOST_NOEXCEPT
    {
        return TimePoint(Duration::Min());
    }

    static TimePoint Max(void) BOOST_NOEXCEPT
    {
        return TimePoint(Duration::Max());
    }

    /*}}}*/

private:
    /**
     * @brief The duration relative to the epoch.
     */
    Duration dt_;
};


BOOST_CONCEPT_ASSERT((TimePointConcept<TimePoint<VirtualClock>>));

typedef TimePoint<VirtualClock>  VirtualTimePoint;


////////////////////////////////////////////////////////////////////////////////
// Comparison./*{{{*/
inline BOOST_CONSTEXPR bool
operator< (const VirtualTimePoint& lhs, const VirtualTimePoint& rhs) BOOST_NOEXCEPT
{
    return lhs.dt_ < rhs.dt_;
}

inline BOOST_CONSTEXPR bool
operator<=(const VirtualTimePoint& lhs, const VirtualTimePoint& rhs) BOOST_NOEXCEPT
{
    return lhs.dt_ <= rhs.dt_;
}

inline BOOST_CONSTEXPR bool
operator==(const VirtualTimePoint& lhs, const VirtualTimePoint& rhs) BOOST_NOEXCEPT
{
    return lhs.dt_ == rhs.dt_;
}

inline BOOST_CONSTEXPR bool
operator!=(const VirtualTimePoint& lhs, const VirtualTimePoint& rhs) BOOST_NOEXCEPT
{
    return lhs.dt_ != rhs.dt_;
}

inline BOOST_CONSTEXPR bool
operator> (const VirtualTimePoint& lhs, const VirtualTimePoint& rhs) BOOST_NOEXCEPT
{
    return lhs.dt_ > rhs.dt_;
}

inline BOOST_CONSTEXPR bool
operator>=(const VirtualTimePoint& lhs, const VirtualTimePoint& rhs) BOOST_NOEXCEPT
{
    return lhs.dt_ >= rhs.dt_;
}
/*}}}*/


////////////////////////////////////////////////////////////////////////////////
// Arithmetic./*{{{*/
inline BOOST_CONSTEXPR VirtualTimePoint
operator+(const VirtualTimePoint& lhs, const VirtualTimePoint::Duration& rhs) BOOST_NOEXCEPT
{
    return VirtualTimePoint(lhs.dt_ + rhs);
}

inline BOOST_CONSTEXPR VirtualTimePoint
operator+(const VirtualTimePoint::Duration& lhs, const VirtualTimePoint& rhs) BOOST_NOEXCEPT
{
    return VirtualTimePoint(lhs + rhs.dt_);
}

inline BOOST_CONSTEXPR VirtualTimePoint
operator-(const VirtualTimePoint& lhs, const VirtualTimePoint::Duration& rhs) BOOST_NOEXCEPT
{
    return VirtualTimePoint(lhs.dt_ - rhs);
}

inline BOOST_CONSTEXPR VirtualTimePoint::Duration
operator-(const VirtualTimePoint& lhs, const VirtualTimePoint& rhs) BOOST_NOEXCEPT
{
    return VirtualTimePoint::Duration(lhs.dt_ - rhs.dt_);
}
/*}}}*/


////////////////////////////////////////////////////////////////////////////////
// Methods./*{{{*/
/**
 * @brief Supports <code>boost::hash<VirtualTimePoint></code>.
 */
inline size_t
hash_value(const VirtualTimePoint& t) BOOST_NOEXCEPT
{
    return hash_value(t.dt_);
}

inline void
swap(VirtualTimePoint& lhs, VirtualTimePoint& rhs) BOOST_NOEXCEPT
{
    lhs.swap(rhs);
}

/*}}}*/

////////////////////////////////////////////////////////////////////////////////
// I/O./*{{{*/
inline std::string
VirtualTimePoint::ToString(void) const
{
    return dt_.ToString();
}
/*}}}*/


NSFX_CHRONO_CLOSE_NAMESPACE


#endif // TIME_POINT_H__C416325F_BBBF_4329_B747_387013318F44

