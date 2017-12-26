/**
 * @file
 *
 * @brief Simulator for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-26
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef OPNET_CHRONO_H__597E70E4_4216_4A35_A2D0_17A609022602
#define OPNET_CHRONO_H__597E70E4_4216_4A35_A2D0_17A609022602


#include <nsfx/simulator/config.h>
#include <cmath>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Free functions.
Duration from_opnet_duration(double dt) BOOST_NOEXCEPT
{
    return NanoSeconds(std::ceil(dt * 1e9));
}

TimePoint from_opnet_time_point(double t) BOOST_NOEXCEPT
{
    return TimePoint(from_opnet_duration(t));
}

double to_opnet_duration(const Duration& dt) BOOST_NOEXCEPT
{
    return ((dt.ToNanoSeconds() + 1e-6) * 1e-9);
}

double to_opnet_time_point(const TimePoint& t) BOOST_NOEXCEPT
{
    return to_opnet_duration(t.GetDuration());
}


NSFX_CLOSE_NAMESPACE


#endif // OPNET_CHRONO_H__597E70E4_4216_4A35_A2D0_17A609022602

