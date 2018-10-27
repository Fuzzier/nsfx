/**
 * @file
 *
 * @brief Simulation support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-01-20
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef SET_EVENT_SCHEDULER_H__93403085_3F17_4CF9_A51E_04EA65436B49
#define SET_EVENT_SCHEDULER_H__93403085_3F17_4CF9_A51E_04EA65436B49


#include <nsfx/simulation/config.h>
#include <nsfx/simulation/i-event-scheduler.h>
#include <nsfx/simulation/event-handle.h>
#include <nsfx/simulation/i-clock.h>
#include <nsfx/component/class-registry.h>
#include <functional>
#include <memory>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Types.
class SetEventScheduler;


////////////////////////////////////////////////////////////////////////////////
// SetEventScheduler.
/**
 * @ingroup Simulator
 * @brief An event scheduler based on set.
 *
 * ## Interfaces
 * ### Uses
 * * \c IClockUser
 * ### Provides
 * * \c IEventScheduler
 */
class SetEventScheduler :
    public IClockUser,
    public IEventScheduler
{
private:
    typedef SetEventScheduler   ThisClass;
    typedef Object<EventHandle> EventHandleClass;

public:
    SetEventScheduler(void) BOOST_NOEXCEPT :
        initialized_(false)
    {}

    virtual ~SetEventScheduler(void) {}

    // IClockUser /*{{{*/
public:
    virtual void Use(Ptr<IClock> clock) NSFX_OVERRIDE
    {
        if (initialized_)
        {
            BOOST_THROW_EXCEPTION(
                IllegalMethodCall() <<
                ErrorMessage("Cannot change the clock after initialization."));
        }
        if (!clock)
        {
            BOOST_THROW_EXCEPTION(InvalidPointer());
        }
        clock_ = clock;
        Initialize();
    }

    /*}}}*/

    // IEventScheduler /*{{{*/
public:
    virtual Ptr<IEventHandle> ScheduleNow(Ptr<IEventSink<> > sink) NSFX_OVERRIDE
    {
        return ThisClass::ScheduleAt(clock_->Now(), std::move(sink));
    }

    virtual Ptr<IEventHandle> ScheduleIn(const Duration& dt,
                                         Ptr<IEventSink<> > sink) NSFX_OVERRIDE
    {
        return ThisClass::ScheduleAt(clock_->Now() + dt, std::move(sink));
    }

    virtual Ptr<IEventHandle> ScheduleAt(const TimePoint& t,
                                         Ptr<IEventSink<> > sink) NSFX_OVERRIDE
    {
        if (!initialized_)
        {
            BOOST_THROW_EXCEPTION(Uninitialized());
        }
        if (!sink)
        {
            BOOST_THROW_EXCEPTION(InvalidPointer());
        }
        if (t < clock_->Now())
        {
            BOOST_THROW_EXCEPTION(
                InvalidArgument() <<
                ErrorMessage("Cannot schedule an event that "
                             "happens before the current time.") <<
                CurrentTimeErrorInfo(clock_->Now()) <<
                ScheduledTimeErrorInfo(t));
        }
        Ptr<EventHandleClass> handle(new EventHandleClass(t, std::move(sink)));
        set_.insert(handle);
        // BOOST_ASSERT(IsOrdered());
        return Ptr<IEventHandle>(static_cast<IEventHandle*>(handle.Detach()),
                                 false);
    }

    virtual size_t GetNumEvents(void) BOOST_NOEXCEPT NSFX_OVERRIDE
    {
        return set_.size();
    }

    virtual Ptr<IEventHandle> GetNextEvent(void) NSFX_OVERRIDE
    {
        EventHandleClass* result = InternalGetNextEvent();
        return static_cast<IEventHandle*>(result);
    }

    void FireAndRemoveNextEvent(void)
    {
        EventHandleClass* result = InternalRemoveNextEvent();
        static_cast<EventHandleClass*>(result)->Fire();
    }

private:
    EventHandleClass* InternalGetNextEvent(void) BOOST_NOEXCEPT
    {
        EventHandleClass* result = nullptr;
        if (set_.size())
        {
            result = set_.begin()->Get();
        }
        return result;
    }

    EventHandleClass* InternalRemoveNextEvent(void)
    {
        EventHandleClass* result = nullptr;
        if (set_.size() > 0)
        {
            auto it = set_.begin();
            result = it->Get();
            set_.erase(it);
        }
        return result;
    }

    bool IsOrdered(void) const BOOST_NOEXCEPT
    {
        bool ordered = true;
        TimePoint t0;
        for (auto it = set_.cbegin(); it != set_.cend() && ordered; ++it)
        {
            ordered = (t0 <= (*it)->GetTimePoint());
            t0 = (*it)->GetTimePoint();
        }
        return ordered;
    }

    /*}}}*/

private:
    void Initialize(void)
    {
        if (!initialized_)
        {
            if (clock_)
            {
                initialized_ = true;
            }
        }
    }

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IClockUser)
        NSFX_INTERFACE_ENTRY(IEventScheduler)
    NSFX_INTERFACE_MAP_END()

private:
    bool  initialized_;
    Ptr<IClock>  clock_;
    set<Ptr<EventHandleClass> >  set_;

}; // class SetEventScheduler


NSFX_REGISTER_CLASS(SetEventScheduler, "edu.uestc.nsfx.SetEventScheduler");


NSFX_CLOSE_NAMESPACE


#endif // SET_EVENT_SCHEDULER_H__93403085_3F17_4CF9_A51E_04EA65436B49

