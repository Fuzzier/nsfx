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
#include <nsfx/simulator/i-event-sink.h>
#include <nsfx/simulator/i-event-handle.h>
#include <nsfx/component/ptr.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// IEventScheduler.
/**
 * @ingroup Simulator
 * @brief The event scheduler interface.
 */
class IEventScheduler :/*{{{*/
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
    virtual Ptr<IEventHandle> ScheduleNow(Ptr<IEventSink> sink) = 0;

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
    virtual Ptr<IEventHandle> ScheduleIn(const Duration& dt,
                                         Ptr<IEventSink> sink) = 0;

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
    virtual Ptr<IEventHandle> ScheduleAt(const TimePoint& t,
                                         Ptr<IEventSink> sink) = 0;

    /**
     * @brief Get the number of scheduled events.
     */
    virtual size_t GetNumEvents(void) BOOST_NOEXCEPT = 0;

    /**
     * @brief Get the next scheduled event.
     */
    virtual Ptr<IEventHandle> GetNextEvent(void) BOOST_NOEXCEPT = 0;

}; // class IEventScheduler /*}}}*/


NSFX_DEFINE_CLASS_UUID4(IEventScheduler, 0x4A9E5C84, 0x1D35, 0x4CAA, 0x9FA3F155AA5E5B30LL);


////////////////////////////////////////////////////////////////////////////////
// IEventSchedulerUser.
/**
 * @ingroup Simulator
 * @brief The event scheduler user interface.
 */
class IEventSchedulerUser :/*{{{*/
    virtual public IObject
{
public:
    virtual ~IEventSchedulerUser(void) BOOST_NOEXCEPT {}

    virtual void UseEventScheduler(Ptr<IEventScheduler> scheduler) = 0;

}; // class IEventScheduler /*}}}*/


NSFX_DEFINE_CLASS_UUID4(IEventSchedulerUser, 0xA72965EB, 0x80C9, 0x42ED, 0x934FC8CD3E1B3670LL);


NSFX_CLOSE_NAMESPACE


#define NSFX_IID_IEventScheduler      NSFX_UUID_OF(::nsfx::IEventScheduler)
#define NSFX_IID_IEventSchedulerUser  NSFX_UUID_OF(::nsfx::IEventSchedulerUser)


#endif // I_EVENT_SCHEDULER_H__B3B744CD_6782_43F2_A063_8B25FD33374E

