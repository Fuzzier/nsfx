/**
 * @file
 *
 * @brief Simulation support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-01-04
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef TIMER_H__61C0726E_E1D4_4006_A241_5D08B097ABA3
#define TIMER_H__61C0726E_E1D4_4006_A241_5D08B097ABA3


#include <nsfx/simulation/config.h>
#include <nsfx/simulation/i-timer.h>
#include <nsfx/simulation/i-clock.h>
#include <nsfx/simulation/i-event-scheduler.h>
#include <nsfx/simulation/i-disposable.h>
#include <nsfx/event/event-sink.h>
#include <nsfx/component/object.h>
#include <nsfx/component/ptr.h>
#include <nsfx/component/class-registry.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Types.
class Timer;

#define NSFX_CID_Timer  NSFX_UUID_OF(::nsfx::Timer)


////////////////////////////////////////////////////////////////////////////////
// Timer.
/**
 * @ingroup Simulator
 * @brief A periodic timer.
 *
 * ## Interfaces
 * ### Uses
 * * \c IClockUser
 * * \c IEventSchedulerUser
 * ### Provides
 * * \c ITimer
 * * \c IDisposable
 */
class Timer :
    public IClockUser,
    public IEventSchedulerUser,
    public ITimer,
    public IDisposable
{
    typedef Timer  ThisClass;

public:
    Timer(void) BOOST_NOEXCEPT :
        initialized_(false),
        timeoutEventSink_(/* controller = */this, /* o = */this, /* ptmf = */&ThisClass::Fire)
    {}

    virtual ~Timer(void) {}

    // ITimer
    virtual void StartNow(const Duration& period,
                          Ptr<IEventSink<> > sink) NSFX_OVERRIDE;

    virtual void StartAt(const TimePoint& t0,
                         const Duration& period,
                         Ptr<IEventSink<> > sink) NSFX_OVERRIDE;

    virtual void Stop(void) NSFX_OVERRIDE;

    // IClockUser
    virtual void Use(Ptr<IClock> clock) NSFX_OVERRIDE;

    // IEventSchedulerUser
    virtual void Use(Ptr<IEventScheduler> scheduler) NSFX_OVERRIDE;

    // IDisposable
    virtual void Dispose(void) NSFX_OVERRIDE;

private:
    void Fire(void)
    {
        sink_->Fire();
        t_ += dt_;
        ScheduleNextTimeout();
    }

    void ScheduleNextTimeout(void)
    {
        handle_ = scheduler_->
            ScheduleAt(t_, Ptr<IEventSink<> >(&timeoutEventSink_));
    }

    void Initialize(void);
    void CheckInitialized(void);

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IClockUser)
        NSFX_INTERFACE_ENTRY(IEventSchedulerUser)
        NSFX_INTERFACE_ENTRY(ITimer)
        NSFX_INTERFACE_ENTRY(IDisposable)
    NSFX_INTERFACE_MAP_END()

private:
    bool  initialized_;
    Ptr<IClock>  clock_;
    Ptr<IEventScheduler>  scheduler_;

    TimePoint t_;
    Duration dt_;
    Ptr<IEventSink<> > sink_;
    Ptr<IEventHandle> handle_;

    MutualObject<MemberFunctionBasedEventSink<IEventSink<>, ThisClass> >
        timeoutEventSink_;

}; // class Timer


NSFX_DEFINE_CLASS_UUID(Timer, 0x8642A6AF, 0x03CA, 0x4F1C, 0x8287C1CDB1CB20E5LL);
NSFX_REGISTER_CLASS(Timer);


////////////////////////////////////////////////////////////////////////////////
inline void Timer::Use(Ptr<IClock> clock)
{
    if (!clock)
    {
        BOOST_THROW_EXCEPTION(InvalidPointer());
    }
    if (clock_)
    {
        BOOST_THROW_EXCEPTION(CannotReinitialize());
    }
    clock_ = clock;
    Initialize();
}

inline void Timer::Use(Ptr<IEventScheduler> scheduler)
{
    if (!scheduler)
    {
        BOOST_THROW_EXCEPTION(InvalidPointer());
    }
    if (scheduler_)
    {
        BOOST_THROW_EXCEPTION(CannotReinitialize());
    }
    scheduler_ = scheduler;
    Initialize();
}

inline void
Timer::StartNow(const Duration& period, Ptr<IEventSink<> > sink)
{
    return StartAt(clock_->Now(), period, sink);
}

inline void
Timer::StartAt(const TimePoint& t0,
               const Duration& period,
               Ptr<IEventSink<> > sink)
{
    CheckInitialized();
    if (!sink)
    {
        BOOST_THROW_EXCEPTION(InvalidPointer());
    }
    if (period <= Duration::Zero())
    {
        BOOST_THROW_EXCEPTION(
            InvalidArgument() <<
            ErrorMessage("The period of the timer must be positive."));
    }
    if (t0 < clock_->Now())
    {
        BOOST_THROW_EXCEPTION(
            InvalidArgument() <<
            ErrorMessage("The start time of the timer cannot be earlier than "
                         "the current time."));
    }
    if (handle_)
    {
        BOOST_THROW_EXCEPTION(
            IllegalMethodCall() <<
            ErrorMessage("The timer is running, stop the timer before re-start "
                         "it."));
    }
    t_ = t0;
    dt_ = period;
    sink_ = sink;
    ScheduleNextTimeout();
}

inline void Timer::Stop(void)
{
    if (handle_)
    {
        handle_->Cancel();
        handle_ = nullptr;
        sink_   = nullptr;
    }
}

inline void Timer::Dispose(void)
{
    handle_->Cancel();
    initialized_ = false;
    clock_       = nullptr;
    scheduler_   = nullptr;
    handle_      = nullptr;
    sink_        = nullptr;
}

inline void Timer::Initialize(void)
{
    if (!initialized_)
    {
        if (clock_ && scheduler_)
        {
            initialized_ = true;
        }
    }
}

inline void Timer::CheckInitialized(void)
{
    if (!initialized_)
    {
        BOOST_THROW_EXCEPTION(Uninitialized());
    }
}


NSFX_CLOSE_NAMESPACE


#endif // TIMER_H__61C0726E_E1D4_4006_A241_5D08B097ABA3

