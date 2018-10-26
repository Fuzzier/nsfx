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

#ifndef LIST_EVENT_SCHEDULER_H__38B15EBB_9CE4_4670_A446_55235A1EEF60
#define LIST_EVENT_SCHEDULER_H__38B15EBB_9CE4_4670_A446_55235A1EEF60


#include <nsfx/simulation/config.h>
#include <nsfx/simulation/i-event-scheduler.h>
#include <nsfx/simulation/i-clock.h>
#include <nsfx/simulation/event-handle.h>
#include <nsfx/component/class-registry.h>
#include <functional>
#include <memory>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Types.
class ListEventScheduler;


////////////////////////////////////////////////////////////////////////////////
// ListEventScheduler.
/**
 * @ingroup Simulator
 * @brief An event scheduler based on list.
 *
 * ## Interfaces
 * ### Uses
 * * \c IClockUser
 * ### Provides
 * * \c IEventScheduler
 */
class ListEventScheduler :
    public IClockUser,
    public IEventScheduler
{
private:
    typedef ListEventScheduler  ThisClass;
    typedef Object<EventHandle> EventHandleClass;

public:
    ListEventScheduler(void) BOOST_NOEXCEPT :
        initialized_(false)
    {}

    virtual ~ListEventScheduler(void) {}

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
        CheckInitialized();
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
        if (!list_.size())
        {
            list_.push_front(handle);
        }
        else // if (list_.size() > 0)
        {
            bool inserted = false;
            for (auto it = list_.begin(); it != list_.end(); ++it)
            {
                Ptr<EventHandleClass>&  h = *it;
                if (h->GetTimePoint() > t)
                {
                    list_.insert(it, handle);
                    inserted = true;
                    break;
                }
            }
            if (!inserted)
            {
                list_.push_back(handle);
            }
        }
        // BOOST_ASSERT(IsOrdered());
        return std::move(handle);
    }

    virtual size_t GetNumEvents(void) BOOST_NOEXCEPT NSFX_OVERRIDE
    {
        return list_.size();
    }

    virtual Ptr<IEventHandle> GetNextEvent(void) NSFX_OVERRIDE
    {
        EventHandleClass* result = InternalGetNextEvent();
        return Ptr<IEventHandle>(result);
    }

    virtual void FireAndRemoveNextEvent(void) NSFX_OVERRIDE
    {
        EventHandleClass* result = InternalRemoveNextEvent();
        Ptr<EventHandleClass>(result)->Fire();
    }

private:
    EventHandleClass* InternalGetNextEvent(void) BOOST_NOEXCEPT
    {
        EventHandleClass* result = nullptr;
        if (list_.size() > 0)
        {
            result = list_.front().Get();
        }
        return result;
    }

    EventHandleClass* InternalRemoveNextEvent(void) BOOST_NOEXCEPT
    {
        EventHandleClass* result = nullptr;
        if (list_.size() > 0)
        {
            result = list_.front().Get();
            list_.pop_front();
        }
        return result;
    }

    bool IsOrdered(void) const
    {
        bool ordered = true;
        TimePoint t0;
        for (auto it = list_.cbegin(); it != list_.cend() && ordered; ++it)
        {
            ordered = (t0 <= (*it)->GetTimePoint());
            t0 = (*it)->GetTimePoint();
        }
        return ordered;
    }

    /*}}}*/

public:
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

    void CheckInitialized(void)
    {
        if (!initialized_)
        {
            BOOST_THROW_EXCEPTION(Uninitialized());
        }
    }

    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IClockUser)
        NSFX_INTERFACE_ENTRY(IEventScheduler)
    NSFX_INTERFACE_MAP_END()

private:
    bool  initialized_;
    Ptr<IClock>  clock_;
    list<Ptr<EventHandleClass> >  list_;

}; // class ListEventScheduler


NSFX_REGISTER_CLASS(ListEventScheduler, "edu.uestc.nsfx.ListEventScheduler");


NSFX_CLOSE_NAMESPACE


#endif // LIST_EVENT_SCHEDULER_H__38B15EBB_9CE4_4670_A446_55235A1EEF60

