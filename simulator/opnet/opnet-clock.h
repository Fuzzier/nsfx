/**
 * @file
 *
 * @brief Simulator for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-19
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef OPNET_CLOCK_H__DD1517B3_2912_4930_891A_A36F79369B97
#define OPNET_CLOCK_H__DD1517B3_2912_4930_891A_A36F79369B97


#include <nsfx/simulator/config.h>
#include <nsfx/simulator/i-clock.h>
#include <nsfx/simulator/opnet/chrono.h>
#include <nsfx/component/class-registry.h>
#include <opnet.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// OpnetClock.
/**
 * @ingroup Simulator
 * @brief The clock provided by OPNET.
 */
class OpnetClock :
    public IClock
{
public:
    typedef OpnetClock  ThisType;

public:
    OpnetClock(void) {}

    virtual ~OpnetClock(void) BOOST_NOEXCEPT {}

    // IAlarm./*{{{*/
    virtual TimePoint Now(void) NSFX_FINAL NSFX_OVERRIDE
    {
        return from_opnet_time_point(::op_sim_time());
    }

    /*}}}*/

    NSFX_INTERFACE_MAP_BEGIN(OpnetClock)
        NSFX_INTERFACE_ENTRY(IClock)
    NSFX_INTERFACE_MAP_END()

};


NSFX_DEFINE_CLASS_UUID4(OpnetClock, 0xDD1517B3, 0x2912, 0x4930, 0x891AA36F79369B97LL);
NSFX_REGISTER_CLASS(OpnetClock);


NSFX_CLOSE_NAMESPACE


#endif // OPNET_CLOCK_H__DD1517B3_2912_4930_891A_A36F79369B97

