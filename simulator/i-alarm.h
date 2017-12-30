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
#include <nsfx/component/i-object.h>
#include <nsfx/component/ptr.h>


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

    virtual void OnAlarm(void) = 0;
};


NSFX_DEFINE_CLASS_UUID4(IAlarmSink, 0x88630CC3, 0xFCE6, 0x452B, 0x825ABCA384FC8C89LL);


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
     */
    virtual void Disconnect(void) BOOST_NOEXCEPT = 0;

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
    virtual bool IsPending(void) BOOST_NOEXCEPT = 0;

    /**
     * @brief Cancel the scheduled alarm (if any).
     */
    virtual void Cancel(void) BOOST_NOEXCEPT = 0;

}; // class IAlarm


NSFX_DEFINE_CLASS_UUID4(IAlarm, 0xF9350F38, 0x9D11, 0x4BF3, 0xB28C143E76AC05C2LL);


NSFX_CLOSE_NAMESPACE


#define NSFX_IID_IAlarmSink  NSFX_UUID_OF(::nsfx::IAlarmSink)
#define NSFX_IID_IAlarm      NSFX_UUID_OF(::nsfx::IAlarm)


#endif // I_ALARM_H__0FC1DAC2_82D2_46A6_9C61_E23484B8608C

