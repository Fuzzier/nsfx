/**
 * @file
 *
 * @brief Packet model.
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


#include "config.h"
#include "i-timer.h"
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
class Timer :
    public ITimer,
    public IClockUser,
    public IEventSchedulerUser
{
private:
    class Handle :/*{{{*/
        public ITimerHandle,
        public IEventSink
    {
    public:
        Handle(void) BOOST_NOEXCEPT {}

        Handle(Ptr<ITimerSink>&& sink,
               Ptr<IEventScheduler> scheduler,
               const TimePoint& t0,
               const Duration& period) :
            sink_(std::move(sink)),
            scheduler_(scheduler),
            t0_(t0),
            period_(period)
        {
        }

        virtual void OnEvent(void) NSFX_OVERRIDE
        {
            sink_->OnTimeout();
            t0_ += period_;
            ScheduleNextTimeout();
        }

        virtual void Stop(void) NSFX_OVERRIDE
        {
            handle_->Cancel();
            handle_ = nullptr;
            sink_   = nullptr;
            scheduler_ = nullptr;
        }

        void ScheduleNextTimeout(void)
        {
            handle_ = scheduler_->ScheduleAt(t0_, this);
        }

        NSFX_INTERFACE_MAP_BEGIN(Handle)
            NSFX_INTERFACE_ENTRY(ITimerHandle)
            NSFX_INTERFACE_ENTRY(IEventSink)
        NSFX_INTERFACE_MAP_END()

    private:
        Ptr<IEventHandle>     handle_;
        Ptr<ITimerSink>       sink_;
        Ptr<IEventScheduler>  scheduler_;
        TimePoint  t0_;
        Duration   period_;
    };/*}}}*/

    typedef Object<Handle>  HandleType;

public:
    Timer(void) BOOST_NOEXCEPT :
        initialized_(false)
    {}

    virtual ~Timer(void) BOOST_NOEXCEPT {}

    // ITimer
    virtual Ptr<ITimerHandle> StartNow(const Duration& period,
                                       Ptr<ITimerSink> sink) NSFX_OVERRIDE;

    virtual Ptr<ITimerHandle> StartAt(const TimePoint& t0,
                                      const Duration& period,
                                      Ptr<ITimerSink> sink) NSFX_OVERRIDE;

    // IClockUser
    virtual void UseClock(Ptr<IClock> clock) NSFX_OVERRIDE;

    // IEventSchedulerUser
    virtual void UseEventScheduler(Ptr<IEventScheduler> scheduler) NSFX_OVERRIDE;

private:
    Ptr<ITimerHandle> InternalStartAt(const TimePoint& t0,
                                      const Duration& period,
                                      Ptr<ITimerSink>&& sink);

    void CheckInitialized(void) BOOST_NOEXCEPT;

    NSFX_INTERFACE_MAP_BEGIN(Timer)
        NSFX_INTERFACE_ENTRY(ITimer)
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
Ptr<ITimerHandle>
Timer::StartNow(const Duration& period, Ptr<ITimerSink> sink)
{
    return InternalStartAt(clock_->Now(), period, std::move(sink));
}

Ptr<ITimerHandle>
Timer::StartAt(const TimePoint& t0, const Duration& period, Ptr<ITimerSink> sink)
{
    return InternalStartAt(t0, period, std::move(sink));
}

Ptr<ITimerHandle>
Timer::InternalStartAt(const TimePoint& t0,
                       const Duration& period,
                       Ptr<ITimerSink>&& sink)
{
    if (!initialized_)
    {
        BOOST_THROW_EXCEPTION(Uninitialized());
    }
    if (!sink)
    {
        BOOST_THROW_EXCEPTION(InvalidPointer());
    }
    try
    {
        Ptr<HandleType> handle(
            new HandleType(std::move(sink), scheduler_, t0, period));
        handle->ScheduleNextTimeout();
        return Ptr<ITimerHandle>(handle.Detach(), true);
    }
    catch (std::bad_alloc& )
    {
        BOOST_THROW_EXCEPTION(OutOfMemory());
    }
}

void Timer::UseClock(Ptr<IClock> clock)
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
    CheckInitialized();
}

void Timer::UseEventScheduler(Ptr<IEventScheduler> scheduler)
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
    CheckInitialized();
}

void Timer::CheckInitialized(void) BOOST_NOEXCEPT
{
    if (clock_ && scheduler_)
    {
        initialized_ = true;
    }
}


NSFX_CLOSE_NAMESPACE


#endif // TIMER_H__61C0726E_E1D4_4006_A241_5D08B097ABA3

