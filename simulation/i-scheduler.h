/**
 * @file
 *
 * @brief Simulation support for Network Simulation Frameworks.
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


#include <nsfx/simulation/config.h>
#include <nsfx/simulation/i-event-handle.h>
#include <nsfx/event/i-event-sink.h>
#include <nsfx/event/event-sink.h>
#include <nsfx/component/i-user.h>
#include <nsfx/component/ptr.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Error info.
typedef boost::error_info<struct tag_CurrentTime, TimePoint>
        CurrentTimeErrorInfo;

typedef boost::error_info<struct tag_ScheduledTime, TimePoint>
        ScheduledTimeErrorInfo;


////////////////////////////////////////////////////////////////////////////////
// IScheduler.
/**
 * @ingroup Simulator
 * @brief An event scheduler.
 */
class IScheduler :
    virtual public IObject
{
public:
    virtual ~IScheduler(void) BOOST_NOEXCEPT {}

    /**
     * @brief Schedule an event.
     *
     * @param[in] sink The event sink.
     *
     * @throw OutOfMemory
     * @throw Uninitialized  The scheduler is not initialized.
     * @throw InvalidPointer The sink is \c nullptr.
     */
    virtual Ptr<IEventHandle> ScheduleNow(Ptr<IEventSink<>> sink) = 0;

    /**
     * @brief Schedule an event.
     *
     * @param[in] sink The event sink.
     *
     * @throw OutOfMemory
     * @throw Uninitialized   The scheduler is not initialized.
     * @throw InvalidPointer  The sink is \c nullptr.
     * @throw InvalidArgument The duration is invalid.
     *                         e.g., it is negative.
     */
    virtual Ptr<IEventHandle> ScheduleIn(const Duration& dt,
                                         Ptr<IEventSink<>> sink) = 0;

    /**
     * @brief Schedule an event.
     *
     * @param[in] sink The event sink.
     *
     * @throw OutOfMemory
     * @throw Uninitialized   The scheduler is not initialized.
     * @throw InvalidPointer  The sink is \c nullptr.
     * @throw InvalidArgument The time point is invalid.
     *                         e.g., it is earlier than the current time.
     */
    virtual Ptr<IEventHandle> ScheduleAt(const TimePoint& t,
                                         Ptr<IEventSink<>> sink) = 0;

    /**
     * @brief Get the number of events in the scheduler.
     *
     * The events may be pending or cancelled.
     * The currently running event is not counted.
     */
    virtual uint64_t GetNumEvents(void) = 0;

    /**
     * @brief Get the next event in the scheduler.
     *
     * @return Return \c nullptr if there is no events in the scheduler.
     *
     * The events may be pending or cancelled.
     *
     * This function <b>cannot</b> return a free pointer to \c IEventHandle,
     * since the \c IEventHandle does not share the same lifetime as the
     * \c IScheduler.
     */
    virtual Ptr<IEventHandle> GetNextEvent(void) = 0;

    /**
     * @brief Fire and remove the next event in the scheduler.
     *
     * If the events has been cancelled, it is remove without being fired.
     */
    virtual void FireAndRemoveNextEvent(void) = 0;

};

NSFX_DEFINE_CLASS_UID(IScheduler, "edu.uestc.nsfx.IScheduler");


////////////////////////////////////////////////////////////////////////////////
// ISchedulerUser.
NSFX_DEFINE_USER_INTERFACE(
    ISchedulerUser, "edu.uestc.nsfx.ISchedulerUser",
    IScheduler);


////////////////////////////////////////////////////////////////////////////////
// Free functions.
inline Ptr<IEventHandle> ScheduleNow(Ptr<IScheduler> scheduler,
                                     Ptr<IEventSink<>> sink)
{
    if (!scheduler)
    {
        BOOST_THROW_EXCEPTION(
            InvalidPointer() <<
            ErrorMessage("The scheduler is nullptr."));
    }
    return scheduler->ScheduleNow(sink);
}

template<class Functor>
inline Ptr<IEventHandle> ScheduleNow(Ptr<IScheduler> scheduler,
                                     Functor&& f)
{
    if (!scheduler)
    {
        BOOST_THROW_EXCEPTION(
            InvalidPointer() <<
            ErrorMessage("The scheduler is nullptr."));
    }
    return scheduler->ScheduleNow(CreateEventSink<IEventSink<>>(
                                  nullptr, std::forward<Functor>(f)));
}

////////////////////////////////////////
inline Ptr<IEventHandle> ScheduleAt(Ptr<IScheduler> scheduler,
                                    const TimePoint& t0,
                                    Ptr<IEventSink<>> sink)
{
    if (!scheduler)
    {
        BOOST_THROW_EXCEPTION(
            InvalidPointer() <<
            ErrorMessage("The scheduler is nullptr."));
    }
    return scheduler->ScheduleAt(t0, sink);
}

template<class Functor>
inline Ptr<IEventHandle> ScheduleAt(Ptr<IScheduler> scheduler,
                                    const TimePoint& t0,
                                    Functor&& f)
{
    if (!scheduler)
    {
        BOOST_THROW_EXCEPTION(
            InvalidPointer() <<
            ErrorMessage("The scheduler is nullptr."));
    }
    return scheduler->ScheduleAt(t0, CreateEventSink<IEventSink<>>(
                                     nullptr, std::forward<Functor>(f)));
}

////////////////////////////////////////
inline Ptr<IEventHandle> ScheduleIn(Ptr<IScheduler> scheduler,
                                    const Duration& dt,
                                    Ptr<IEventSink<>> sink)
{
    if (!scheduler)
    {
        BOOST_THROW_EXCEPTION(
            InvalidPointer() <<
            ErrorMessage("The scheduler is nullptr."));
    }
    return scheduler->ScheduleIn(dt, sink);
}

template<class Functor>
inline Ptr<IEventHandle> ScheduleIn(Ptr<IScheduler> scheduler,
                                    const Duration& dt,
                                    Functor&& f)
{
    if (!scheduler)
    {
        BOOST_THROW_EXCEPTION(
            InvalidPointer() <<
            ErrorMessage("The scheduler is nullptr."));
    }
    return scheduler->ScheduleIn(dt, CreateEventSink<IEventSink<>>(
                                     nullptr, std::forward<Functor>(f)));
}


NSFX_CLOSE_NAMESPACE


#endif // I_EVENT_SCHEDULER_H__B3B744CD_6782_43F2_A063_8B25FD33374E

