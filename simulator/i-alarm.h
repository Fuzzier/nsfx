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

#ifndef I_ALARM_H__0FC1DAC2_82D2_46A6_9C61_E23484B8608C
#define I_ALARM_H__0FC1DAC2_82D2_46A6_9C61_E23484B8608C


#include <nsfx/simulator/config.h>
#include <nsfx/component/i-alarm-sink.h>


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
     * @pre No sink has been connected yet.
     *
     * @param sink The alarm sink.
     *
     * @throw InvalidPointer  The sink is \c nullptr.
     * @throw ConnectionLimit A sink has already been connected.
     */
    virtual void Connect(Ptr<IAlarmSink> sink) = 0;

    /**
     * @brief Disconnect from the sink.
     *
     * @pre A sink has already been connected.
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
     * @throw InvalidArgument The time point is too early.
     */
    virtual void ScheduleAt(const TimePoint& t) = 0;

    /**
     * @brief Schedule an alarm.
     *
     * @pre A sink has already been connected.<br/>
     *      The alarm has not been scheduled yet.
     *
     * @throw NotConnected No sink has been connected.
     * @throw InvalidArgument The duration is too negative.
     */
    virtual void ScheduleIn(const Duration& dt) = 0;

    /**
     * @brief Whether the alarm is scheduled.
     */
    virtual bool IsPending(void) = 0;

    /**
     * @brief Cancel the scheduled alarm (if any).
     */
    virtual void Cancel(void) = 0;

}; // class IAlarm


NSFX_DEFINE_CALSS_UUID4(IAlarm, 0xF9350F38, 0x9D11, 0x4BF3, 0xB28C143E76AC05C2LL);


NSFX_CLOSE_NAMESPACE


#endif // I_ALARM_H__0FC1DAC2_82D2_46A6_9C61_E23484B8608C

