/**
 * @file
 *
 * @brief Simulator for Network Simulation Frameworks.
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


#include <nsfx/simulator/config.h>
#include <nsfx/simulator/i-timer.h>
#include <nsfx/simulator/i-event-scheduler.h>
#include <nsfx/event/event.h>
#include <nsfx/component/object.h>
#include <nsfx/component/ptr.h>
#include <nsfx/component/class-registry.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Types.
class Timer;

#define NSFX_CID_Timer        NSFX_UUID_OF(::nsfx::Timer)


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

private:
    class Handle :/*{{{*/
        public ITimerHandle,
        public IEventSink<>
    {
    public:
        Handle(void) BOOST_NOEXCEPT {}

        Handle(Ptr<IEventSink<> >&& sink,
               Ptr<IEventScheduler> scheduler,
               const TimePoint& t0,
               const Duration& period) :
            sink_(std::move(sink)),
            scheduler_(scheduler),
            t0_(t0),
            period_(period)
        {
        }

        // ITimerHandle
        virtual void Stop(void) NSFX_OVERRIDE
        {
            handle_->Cancel();
            handle_ = nullptr;
            sink_   = nullptr;
            scheduler_ = nullptr;
        }

        // IEventSink
        virtual void Fire(void) NSFX_OVERRIDE
        {
            Signal();
        }

        // Methods.
        void Signal(void)
        {
            sink_->Fire();
            t0_ += period_;
            ScheduleNextTimeout();
        }

        void ScheduleNextTimeout(void)
        {
            handle_ = scheduler_->ScheduleAt(t0_, Ptr<IEventSink<> >(this));
        }

        NSFX_INTERFACE_MAP_BEGIN(Handle)
            NSFX_INTERFACE_ENTRY(ITimerHandle)
            NSFX_INTERFACE_ENTRY(IEventSink<>)
        NSFX_INTERFACE_MAP_END()

    private:
        Ptr<IEventHandle>     handle_;
        Ptr<IEventSink<> >    sink_;
        Ptr<IEventScheduler>  scheduler_;
        TimePoint  t0_;
        Duration   period_;
    };/*}}}*/

    typedef Object<Handle>  HandleClass;

public:
    Timer(void) BOOST_NOEXCEPT :
        initialized_(false)
    {}

    virtual ~Timer(void) {}

    // ITimer
    virtual Ptr<ITimerHandle> StartNow(const Duration& period,
                                       Ptr<IEventSink<> > sink) NSFX_OVERRIDE;

    virtual Ptr<ITimerHandle> StartAt(const TimePoint& t0,
                                      const Duration& period,
                                      Ptr<IEventSink<> > sink) NSFX_OVERRIDE;

    // IClockUser
    virtual void UseClock(Ptr<IClock> clock) NSFX_OVERRIDE;

    // IEventSchedulerUser
    virtual void UseEventScheduler(Ptr<IEventScheduler> scheduler) NSFX_OVERRIDE;

    // IDisposable
    virtual void Dispose(void) NSFX_OVERRIDE;

private:
    Ptr<ITimerHandle> InternalStartAt(const TimePoint& t0,
                                      const Duration& period,
                                      Ptr<IEventSink<> >&& sink);

    void CheckInitialized(void);

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(ITimer)
        NSFX_INTERFACE_ENTRY(IDisposable)
        NSFX_INTERFACE_ENTRY(IClockUser)
        NSFX_INTERFACE_ENTRY(IEventSchedulerUser)
    NSFX_INTERFACE_MAP_END()

private:
    bool  initialized_;
    Ptr<IClock>  clock_;
    Ptr<IEventScheduler>  scheduler_;

}; // class Timer


NSFX_DEFINE_CLASS_UUID4(Timer, 0x8642A6AF, 0x03CA, 0x4F1C, 0x8287C1CDB1CB20E5LL);
NSFX_REGISTER_CLASS(Timer);


////////////////////////////////////////////////////////////////////////////////
inline void Timer::UseClock(Ptr<IClock> clock)
{
    if (clock_)
    {
        BOOST_THROW_EXCEPTION(CannotReinitialize());
    }
    if (!clock)
    {
        BOOST_THROW_EXCEPTION(InvalidPointer());
    }
    clock_ = clock;
}

inline void Timer::UseEventScheduler(Ptr<IEventScheduler> scheduler)
{
    if (scheduler_)
    {
        BOOST_THROW_EXCEPTION(CannotReinitialize());
    }
    if (!scheduler)
    {
        BOOST_THROW_EXCEPTION(InvalidPointer());
    }
    scheduler_ = scheduler;
}

inline Ptr<ITimerHandle>
Timer::StartNow(const Duration& period, Ptr<IEventSink<> > sink)
{
    return InternalStartAt(clock_->Now(), period, std::move(sink));
}

inline Ptr<ITimerHandle>
Timer::StartAt(const TimePoint& t0,
               const Duration& period,
               Ptr<IEventSink<> > sink)
{
    return InternalStartAt(t0, period, std::move(sink));
}

inline Ptr<ITimerHandle>
Timer::InternalStartAt(const TimePoint& t0,
                       const Duration& period,
                       Ptr<IEventSink<> >&& sink)
{
    CheckInitialized();
    if (!sink)
    {
        BOOST_THROW_EXCEPTION(InvalidPointer());
    }
    Ptr<HandleClass> handle(
        new HandleClass(std::move(sink), scheduler_, t0, period));
    handle->GetEnveloped()->ScheduleNextTimeout();
    return std::move(handle);
}

inline void Timer::Dispose(void)
{
    initialized_ = false;
    clock_       = nullptr;
    scheduler_   = nullptr;
}

inline void Timer::CheckInitialized(void)
{
    if (!initialized_)
    {
        if (clock_ && scheduler_)
        {
            initialized_ = true;
        }
        else
        {
            BOOST_THROW_EXCEPTION(Uninitialized());
        }
    }
}


NSFX_CLOSE_NAMESPACE


#endif // TIMER_H__61C0726E_E1D4_4006_A241_5D08B097ABA3

