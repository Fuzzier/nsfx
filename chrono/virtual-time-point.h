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
#include <nsfx/chrono/time-point-concept.h>
#include <boost/functional/hash.hpp>
#include <boost/core/swap.hpp>


NSFX_CHRONO_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Chrono
 * @brief Virtual time epoch.
 *
 * This time epoch can be used in discreet event simulations.<br/>
 */
class VirtualEpoch;


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Chrono
 * @brief TimePoint point.
 *
 * @tparam Epoch TimePoint epoch.<br/>
 *
 * TimePoint epoch represents the starting time point in a time system.<br/>
 * TimePoint point stores the time duration elapsed since the epoch.<br/>
 * The time resolution is 1 nanosecond.<br/>
 */
template<>
class TimePoint<VirtualEpoch>
{
    // Typedefs./*{{{*/
public:
    typedef Duration      DurationType;
    typedef VirtualEpoch  EpochType;
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

    // Comparisons./*{{{*/
public:
    friend BOOST_CONSTEXPR bool operator< (const TimePoint& lhs, const TimePoint& rhs) BOOST_NOEXCEPT;

    friend BOOST_CONSTEXPR bool operator<=(const TimePoint& lhs, const TimePoint& rhs) BOOST_NOEXCEPT;

    friend BOOST_CONSTEXPR bool operator==(const TimePoint& lhs, const TimePoint& rhs) BOOST_NOEXCEPT;

    friend BOOST_CONSTEXPR bool operator!=(const TimePoint& lhs, const TimePoint& rhs) BOOST_NOEXCEPT;

    friend BOOST_CONSTEXPR bool operator> (const TimePoint& lhs, const TimePoint& rhs) BOOST_NOEXCEPT;

    friend BOOST_CONSTEXPR bool operator>=(const TimePoint& lhs, const TimePoint& rhs) BOOST_NOEXCEPT;

    /*}}}*/

    // Algorithms./*{{{*/
public:
    friend BOOST_CONSTEXPR TimePoint operator+ (const TimePoint& lhs, const DurationType& rhs) BOOST_NOEXCEPT;

    friend BOOST_CONSTEXPR TimePoint operator+ (const DurationType& lhs, const TimePoint& rhs) BOOST_NOEXCEPT;

    friend BOOST_CONSTEXPR TimePoint operator- (const TimePoint& lhs, const DurationType& rhs) BOOST_NOEXCEPT;

    friend BOOST_CONSTEXPR DurationType operator- (const TimePoint& lhs, const TimePoint& rhs) BOOST_NOEXCEPT;

    BOOST_CONSTEXPR TimePoint& operator+=(const DurationType& rhs) BOOST_NOEXCEPT
    {
        dt_ += rhs;
        return *this;
    }

    BOOST_CONSTEXPR TimePoint& operator-=(const DurationType& rhs) BOOST_NOEXCEPT
    {
        dt_ -= rhs;
        return *this;
    }

    /*}}}*/

    // Methods./*{{{*/
    /**
     * @brief Get the duration relative to the epoch.
     */
    BOOST_CONSTEXPR DurationType GetDuration(void) const BOOST_NOEXCEPT
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
    DurationType dt_;
}; // class TimePoint<VirtualEpoch>


BOOST_CONCEPT_ASSERT((TimePointConcept<TimePoint<VirtualEpoch> >));

typedef TimePoint<VirtualEpoch>  VirtualTimePoint;


////////////////////////////////////////////////////////////////////////////////
// Comparisons./*{{{*/
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
// Algorithms./*{{{*/
inline BOOST_CONSTEXPR VirtualTimePoint
operator+ (const VirtualTimePoint& lhs, const VirtualTimePoint::DurationType& rhs) BOOST_NOEXCEPT
{
    return VirtualTimePoint(lhs.dt_ + rhs);
}

inline BOOST_CONSTEXPR VirtualTimePoint
operator+ (const VirtualTimePoint::DurationType& lhs, const VirtualTimePoint& rhs) BOOST_NOEXCEPT
{
    return VirtualTimePoint(lhs + rhs.dt_);
}

inline BOOST_CONSTEXPR VirtualTimePoint
operator- (const VirtualTimePoint& lhs, const VirtualTimePoint::DurationType& rhs) BOOST_NOEXCEPT
{
    return VirtualTimePoint(lhs.dt_ - rhs);
}

inline BOOST_CONSTEXPR VirtualTimePoint::DurationType
operator-(const VirtualTimePoint& lhs, const VirtualTimePoint& rhs) BOOST_NOEXCEPT
{
    return VirtualTimePoint::DurationType(lhs.dt_ - rhs.dt_);
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

