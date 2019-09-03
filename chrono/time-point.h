/**
 * @file
 *
 * @brief Chrono support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-13
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef TIME_POINT_CONCEPT_H__A02D88FE_623D_4897_A2CE_C8C270901D1A
#define TIME_POINT_CONCEPT_H__A02D88FE_623D_4897_A2CE_C8C270901D1A


#include <nsfx/chrono/config.h>
#include <boost/concept_check.hpp>
#include <boost/functional/hash.hpp>
#include <iostream>


NSFX_CHRONO_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Chrono
 * @brief The primary class template for defining a time point class.
 *
 * @tparam Clock    The clock that defines the epoch of the time system.
 * @tparam Duration The duration that defines the time resolution of the time
 *                  system.
 *                  It also defines the range of representable time duration.
 */
template<class Clock, class Duration = typename Clock::Duration>
class TimePoint;


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Chrono
 * @brief Time point concept.
 */
template<class TimePoint>
class TimePointConcept
{
private:
    typedef typename TimePoint::Duration Duration;
    typedef typename TimePoint::Clock    Clock;

    BOOST_CONCEPT_ASSERT((boost::Assignable<TimePoint>));
    BOOST_CONCEPT_ASSERT((boost::DefaultConstructible<TimePoint>));
    BOOST_CONCEPT_ASSERT((boost::CopyConstructible<TimePoint>));
    BOOST_CONCEPT_ASSERT((boost::EqualityComparable<TimePoint>));
    BOOST_CONCEPT_ASSERT((boost::Comparable<TimePoint>));

public:
    BOOST_CONCEPT_USAGE(TimePointConcept)
    {
        FromDuration();
        Arithmetics();
        GetDuration();
        ToDouble();
        Hashable();
        Swappable();
        IO();
        Limits();
    }

private:
    void FromDuration(void)
    {
        Duration dt;
        TimePoint t(dt);
    }

    void Arithmetics(void)
    {
        Duration  dt;
        TimePoint t0;
        TimePoint t1 = t0 + dt;
        TimePoint t2 = dt + t0;
        TimePoint t3 = t0 - dt;
        dt = t1 - t0;
        t0 += dt;
        t0 -= dt;
    }

    void GetDuration(void)
    {
        TimePoint t;
        Duration dt = t.GetDuration();
    }

    void ToDouble(void)
    {
        TimePoint t;
        double d = t.ToDouble();
    }

    void GetSize(void)
    {
        TimePoint::GetSize();
    }

    void Hashable(void)
    {
        TimePoint t;
        size_t h = hash_value(t);
        h = boost::hash<TimePoint>()(t);
    }

    void Swappable(void)
    {
        TimePoint t1;
        TimePoint t2;
        swap(t1, t2);
    }

    void IO(void)
    {
        TimePoint t0;
        std::string s = t0.ToString();
        std::cout << t0;
    }

    void Limits(void)
    {
        TimePoint epoch = TimePoint::Epoch();
        TimePoint tmin  = TimePoint::Min();
        TimePoint tmax  = TimePoint::Max();
    }

};


////////////////////////////////////////////////////////////////////////////////
// ostream <<./*{{{*/
template<class CharT, class TraitsT, class Clock, class Duration>
inline std::basic_ostream<CharT, TraitsT>&
operator<<(std::basic_ostream<CharT, TraitsT>& os,
           const TimePoint<Clock, Duration>& rhs)
{
    return os << rhs.ToString();
}
/*}}}*/


NSFX_CHRONO_CLOSE_NAMESPACE


#endif // TIME_POINT_CONCEPT_H__A02D88FE_623D_4897_A2CE_C8C270901D1A

