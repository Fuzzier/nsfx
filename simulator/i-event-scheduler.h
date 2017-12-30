/**
 * @file
 *
 * @brief Simulator for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-25
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef I_EVENT_SCHEDULER_H__B3B744CD_6782_43F2_A063_8B25FD33374E
#define I_EVENT_SCHEDULER_H__B3B744CD_6782_43F2_A063_8B25FD33374E


#include <nsfx/simulator/config.h>
#include <nsfx/simulator/i-alarm-sink.h>
#include <nsfx/simulator/i-event-handle.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Types.
/**
 * @ingroup Simulator
 * @brief The event sink interface (an alias of alarm sink interface).
 */
typedef IAlarmSink  IEventSink;


////////////////////////////////////////////////////////////////////////////////
// IEventSchedulerInitialize.
/**
 * @ingroup Simulator
 * @brief The event scheduler initialize interface.
 */
class IEventSchedulerInitialize :
    virtual public IObject
{
public:
    virtual ~IEventSchedulerInitialize(void) BOOST_NOEXCEPT {}

    /**
     * @brief Set the alarm clock.
     *
     * @pre An alarm clock has not been set yet.
     *
     * @param[in] alarmClock An object that supports both \c IClock and
     *                       \c IAlarm interfaces.
     *
     * @throw InvalidPointer The \c alarmClock is \c nullptr.
     * @throw NoInterface    The \c alarmClock does not support the required
     *                       interfaces.
     * @throw IllegalMethodCall An alarm clock has already been set.
     */
    virtual void SetAlarmClock(Ptr<IObject> alarmClock) = 0;

}; // class IEventSchedulerInitialize


NSFX_DEFINE_CALSS_UUID4(IEventSchedulerInitialize, 0xB9C69134, 0xB864, 0x405C, 0xADD64998E7C6096CLL);


////////////////////////////////////////////////////////////////////////////////
// IEventScheduler.
/**
 * @ingroup Simulator
 * @brief The event scheduler interface.
 *
 * An object that supports \c IEventScheduler interface may optionally support
 * \c IEventSchedulerInitialize interface.
 */
class IEventScheduler :
    virtual public IObject
{
public:
    virtual ~IEventScheduler(void) BOOST_NOEXCEPT {}

    /**
     * @brief Schedule an event.
     *
     * @param[in]  sink   The event sink.
     * @param[out] handle Optional. A handle of the event.
     *
     * @throw OutOfMemory
     * @throw Uninitialized  The scheduler is not initialized.
     * @throw InvalidPointer The sink is \c nullptr.
     */
    virtual void ScheduleNow(Ptr<IEventSink> sink,
                             Ptr<IEventHandle>* handle) = 0;

    /**
     * @brief Schedule an event.
     *
     * @param[in]  sink   The event sink.
     * @param[out] handle Optional. A handle of the event.
     *
     * @throw OutOfMemory
     * @throw Uninitialized   The scheduler is not initialized.
     * @throw InvalidPointer  The sink is \c nullptr.
     * @throw InvalidArgument Thrown by \c IAlarm.
     */
    virtual void ScheduleIn(const Duration& dt,
                            Ptr<IEventSink> sink,
                            Ptr<IEventHandle>* handle) = 0;

    /**
     * @brief Schedule an event.
     *
     * @param[in]  sink   The event sink.
     * @param[out] handle Optional. A handle of the event.
     *
     * @throw OutOfMemory
     * @throw Uninitialized   The scheduler is not initialized.
     * @throw InvalidPointer  The sink is \c nullptr.
     * @throw InvalidArgument Thrown by \c IAlarm.
     */
    virtual void ScheduleAt(const TimePoint& t,
                            Ptr<IEventSink> sink,
                            Ptr<IEventHandle>* handle) = 0;

    /**
     * @brief Get the number of scheduled events.
     */
    virtual size_t GetNumEvents(void) BOOST_NOEXCEPT = 0;

    /**
     * @brief Get the next scheduled event.
     */
    virtual IEventHandle* GetNextEvent(void) BOOST_NOEXCEPT = 0;

}; // class IEventScheduler


NSFX_DEFINE_CALSS_UUID4(IEventScheduler, 0x4A9E5C84, 0x1D35, 0x4CAA, 0x9FA3F155AA5E5B30LL);


NSFX_CLOSE_NAMESPACE


#define NSFX_IID_IEventScheduler  NSFX_UUID_OF(::nsfx::IEventScheduler)


#endif // I_EVENT_SCHEDULER_H__B3B744CD_6782_43F2_A063_8B25FD33374E

