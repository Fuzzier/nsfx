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

#ifndef I_EVENT_SCHEDULER_H__DB08F8D9_56BF_4941_8149_EA4E6589A33B
#define I_EVENT_SCHEDULER_H__DB08F8D9_56BF_4941_8149_EA4E6589A33B


#include <nsfx/simulator/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/i-event-sink.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// IEventScheduler.
/**
 * @ingroup Simulator
 * @brief The event scheduler interface.
 */
class IEventScheduler :
    virtual public IObject
{
public:
    virtual ~IEventScheduler(void) BOOST_NOEXCEPT {}

    /**
     * @brief Schedule an event.
     *
     * @param sink The sink must expose \c IEventSink interface.
     *
     * @throw InvalidPointer  The sink is \c nullptr.
     */
    virtual void ScheduleNow(IEventSink* sink) = 0;

    /**
     * @brief Schedule an event.
     *
     * @param sink The sink must expose \c IEventSink interface.
     *
     * @throw InvalidArgument The time point is earlier then the current time.
     * @throw InvalidPointer  The sink is \c nullptr.
     */
    virtual void ScheduleAt(const TimePoint& t0, IEventSink* sink) = 0;

    /**
     * @brief Schedule an event.
     *
     * @param sink The sink must expose \c IEventSink interface.
     *
     * @throw InvalidArgument The duration is less then \c 0.
     * @throw InvalidPointer  The sink is \c nullptr.
     */
    virtual void ScheduleFor(const Duration& dt, IEventSink* sink) = 0;

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
     * @throw InvalidArgument The time point is earlier than the current time.
     */
    virtual void ScheduleAt(const TimePoint& t) = 0;

    /**
     * @brief Schedule an alarm.
     *
     * @pre A sink has already been connected.<br/>
     *      The alarm has not been scheduled yet.
     *
     * @throw NotConnected No sink has been connected.
     * @throw InvalidArgument The duration is negative.
     */
    virtual void ScheduleFor(const Duration& dt) = 0;

    /**
     * @brief Whether the alarm is scheduled.
     */
    virtual bool IsPending(void) BOOST_NOEXCEPT = 0;

    /**
     * @brief Cancel the scheduled alarm (if any).
     */
    virtual void Cancel(void) BOOST_NOEXCEPT = 0;

};


NSFX_DEFINE_CALSS_UUID4(IEventScheduler, 0xDB08F8D9, 0x56BF, 0x4941, 0x8149EA4E6589A33BLL)


NSFX_CLOSE_NAMESPACE


#endif // I_EVENT_SCHEDULER_H__DB08F8D9_56BF_4941_8149_EA4E6589A33B

