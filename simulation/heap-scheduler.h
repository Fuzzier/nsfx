/**
 * @file
 *
 * @brief Simulation support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2019-09-12
 *
 * @copyright Copyright (c) 2019.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef HEAP_SCHEDULER_H__F5882B9F_C227_4B26_B5C4_22D0CC9286F3
#define HEAP_SCHEDULER_H__F5882B9F_C227_4B26_B5C4_22D0CC9286F3


#include <nsfx/simulation/config.h>
#include <nsfx/simulation/i-scheduler.h>
#include <nsfx/simulation/event-handle.h>
#include <nsfx/simulation/i-clock.h>
#include <nsfx/component/class-registry.h>
#include <functional>
#include <memory>
#include <algorithm>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// HeapScheduler.
/**
 * @ingroup Simulator
 * @brief An event scheduler based on heap.
 *
 * # Uid
 * @code
 * "edu.uestc.nsfx.HeapScheduler"
 * @endcode
 *
 * # Interfaces
 * * Uses
 *   + \c IClock
 * * Provides
 *   + \c IScheduler
 */
class HeapScheduler :
    public IClockUser,
    public IScheduler
{
private:
    typedef HeapScheduler   ThisClass;

private:
    struct HeapLessThan
    {
        bool operator()(const EventHandle* lhs, const EventHandle* rhs) const BOOST_NOEXCEPT
        {
            // For a max-heap, put the event that happens earliest at the head.
            // If `rhs` is to be fired earlier than `lhs`, then `rhs` shall be
            // placed ahead of `lhs`.
            return *rhs < *lhs;
        }
    };

public:
    HeapScheduler(void) BOOST_NOEXCEPT :
        initialized_(false),
        nextEventId_(0)
    {}

    virtual ~HeapScheduler(void)
    {
        for (auto it = events_.begin(); it != events_.end(); ++it)
        {
            (*it)->Release();
        }
    }

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
        initialized_ = true;
    }

    /*}}}*/

    // IScheduler /*{{{*/
public:
    virtual Ptr<IEventHandle> ScheduleNow(Ptr<IEventSink<>> sink) NSFX_OVERRIDE
    {
        return ThisClass::ScheduleAt(clock_->Now(), std::move(sink));
    }

    virtual Ptr<IEventHandle> ScheduleIn(const Duration& dt,
                                         Ptr<IEventSink<>> sink) NSFX_OVERRIDE
    {
        return ThisClass::ScheduleAt(clock_->Now() + dt, std::move(sink));
    }

    virtual Ptr<IEventHandle> ScheduleAt(const TimePoint& t,
                                         Ptr<IEventSink<>> sink) NSFX_OVERRIDE
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
        EventHandle* handle = new Object<EventHandle>(
                                  nextEventId_++, t, std::move(sink));
        handle->AddRef();
        events_.push_back(handle);
        std::push_heap(events_.begin(), events_.end(), HeapLessThan());
        // BOOST_ASSERT(IsOrdered());
        return handle->GetIntf();
    }

    virtual uint64_t GetNumEvents(void) BOOST_NOEXCEPT NSFX_OVERRIDE
    {
        return events_.size();
    }

    virtual Ptr<IEventHandle> GetNextEvent(void) NSFX_OVERRIDE
    {
        IEventHandle* result = nullptr;
        if (events_.size())
        {
            result = events_.front()->GetIntf();
        }
        return result;
    }

    virtual void FireAndRemoveNextEvent(void) NSFX_OVERRIDE
    {
        IEventHandle* result = nullptr;
        if (events_.size())
        {
            std::pop_heap(events_.begin(), events_.end(), HeapLessThan());
            events_.back()->Fire();
            events_.back()->Release();
            events_.pop_back();
            // BOOST_ASSERT(IsOrdered());
        }
    }

private:
    bool IsOrdered(void) const BOOST_NOEXCEPT
    {
        bool ordered = true;
        TimePoint t0;
        auto it = events_.begin();
        if (it != events_.end())
        {
            t0 = (*it)->GetTimePoint();
            for (++it; it != events_.end() && ordered; ++it)
            {
                ordered = (t0 <= (*it)->GetTimePoint());
            }
        }
        return ordered;
    }

    /*}}}*/

private:
    NSFX_INTERFACE_MAP_BEGIN(ThisClass)
        NSFX_INTERFACE_ENTRY(IClockUser)
        NSFX_INTERFACE_ENTRY(IScheduler)
    NSFX_INTERFACE_MAP_END()

private:
    bool  initialized_;
    Ptr<IClock>  clock_;
    event_id_t   nextEventId_;
    vector<EventHandle*>  events_;

}; // class HeapScheduler


NSFX_REGISTER_CLASS(HeapScheduler, "edu.uestc.nsfx.HeapScheduler");


NSFX_CLOSE_NAMESPACE


#endif // HEAP_SCHEDULER_H__F5882B9F_C227_4B26_B5C4_22D0CC9286F3

