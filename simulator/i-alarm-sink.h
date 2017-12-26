/**
 * @file
 *
 * @brief Simulator for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-18
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef I_ALARM_SINK_H__39A3740A_3B90_4851_9BC0_E910E3F2AD46
#define I_ALARM_SINK_H__39A3740A_3B90_4851_9BC0_E910E3F2AD46


#include <nsfx/simulator/config.h>
#include <nsfx/component/i-object.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// IAlarmSink.
/**
 * @ingroup Simulator
 * @brief The alarm sink interface.
 */
class IAlarmSink :
    virtual public IObject
{
public:
    virtual ~IAlarmSink(void) BOOST_NOEXCEPT {}

    virtual void Fire(void) = 0;
};


NSFX_DEFINE_CALSS_UUID4(IAlarmSink, 0x88630CC3, 0xFCE6, 0x452B, 0x825ABCA384FC8C89LL);


NSFX_CLOSE_NAMESPACE


#endif // I_ALARM_SINK_H__39A3740A_3B90_4851_9BC0_E910E3F2AD46

