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

#ifndef IALARM_H__0FC1DAC2_82D2_46A6_9C61_E23484B8608C
#define IALARM_H__0FC1DAC2_82D2_46A6_9C61_E23484B8608C


#include <nsfx/simulator/config.h>
#include <nsfx/component/iobject.h>
#include <nsfx/component/ialarm-sink.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// IAlarm.
/**
 * @ingroup Simulator
 * @brief The alarm interface.
 */
class IAlarm :
    virtual public IObject
{
public:
    virtual ~IAlarm(void) BOOST_NOEXCEPT {}

    /**
     * @brief Connect to a sink.
     *
     * @param sink An alarm must query \c IAlarmSink interface from the sink.
     *
     * @throw InvalidPointer  The sink is \c nullptr.
     * @throw ConnectionLimit A sink has already been connected.
     * @throw CannotConnect   The sink does not support \c IAlarmSink interface.
     */
    virtual void Connect(IUnknown* sink) = 0;

    /**
     * @brief Disconnect from the sink.
     *
     * The scheduled alarm (if any) is also cancelled.
     *
     * @throw NotConnected No sink has been connected.
     */
    virtual void Disconnect(void) = 0;

    /**
     * @brief Schedule an alarm.
     *
     * @throw NotConnected No sink has been connected.
     * @throw InvalidArgument The time point is earlier than the current time.
     */
    virtual void ScheduleAt(const TimePoint& t) = 0;

    /**
     * @brief Schedule an alarm.
     *
     * @throw NotConnected No sink has been connected.
     * @throw InvalidArgument The duration is negative.
     */
    virtual void ScheduleFor(const Duration& dt) = 0;

    /**
     * @brief Check whether an alarm is pending.
     */
    virtual bool IsPending(void) BOOST_NOEXCEPT = 0;

    /**
     * @brief Cancel the scheduled alarm (if any).
     */
    virtual void Cancel(void) BOOST_NOEXCEPT = 0;

};


NSFX_DEFINE_CLASS_UUID4(IAlarm, 0x0FC1DAC2, 0x82D2, 0x46A6, 0x9C61E23484B8608CLL);


NSFX_CLOSE_NAMESPACE


#endif // IALARM_H__0FC1DAC2_82D2_46A6_9C61_E23484B8608C

