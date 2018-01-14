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

#ifndef LIST_EVENT_SCHEDULER_H__38B15EBB_9CE4_4670_A446_55235A1EEF60
#define LIST_EVENT_SCHEDULER_H__38B15EBB_9CE4_4670_A446_55235A1EEF60


#include <nsfx/simulator/config.h>
#include <nsfx/simulator/i-event-scheduler.h>
#include <nsfx/simulator/i-clock.h>
#include <nsfx/component/i-disposable.h>
#include <nsfx/component/class-registry.h>
#include <functional>
#include <memory>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Types.
class ListEventScheduler;

#define NSFX_CID_ListEventScheduler  NSFX_UUID_OF(::nsfx::ListEventScheduler)


////////////////////////////////////////////////////////////////////////////////
// ListEventScheduler.
/**
 * @ingroup Simulator
 * @brief The list event scheduler.
 *
 * ## Interfaces
 * ### Uses
 * * \c IClockUser
 * ### Provides
 * * \c IEventScheduler
 * * \c IDisposable
 */
class ListEventScheduler :
    public IClockUser,
    public IEventScheduler,
    public IDisposable
{
private:
    typedef ListEventScheduler  ThisType;

    class EventHandle :/*{{{*/
        public IEventHandle
    {
    public:
        EventHandle(ListEventScheduler* scheduler,
                    const TimePoint& t,
                    Ptr<IEventSink<> >&& sink) BOOST_NOEXCEPT:
            scheduler_(scheduler),
            t_(t),
            sink_(std::move(sink)),
            removed_(false),
            running_(false)
        {
        }

        virtual ~EventHandle(void) {}

        // IEventHandle /*{{{*/
        virtual bool IsPending(void) BOOST_NOEXCEPT NSFX_OVERRIDE
        {
            return (!removed_ && !running_);
        }

        virtual bool IsRunning(void) BOOST_NOEXCEPT NSFX_OVERRIDE
        {
            return running_;
        }

        virtual bool IsValid(void) BOOST_NOEXCEPT NSFX_OVERRIDE
        {
            return (!removed_);
        }

        virtual void Cancel(void) BOOST_NOEXCEPT NSFX_OVERRIDE
        {
            removed_ = true;
        }

        virtual TimePoint GetTimePoint(void) BOOST_NOEXCEPT NSFX_OVERRIDE
        {
            return t_;
        }

        virtual void Signal(void) NSFX_OVERRIDE
        {
            if (this != scheduler_->InternalGetNextEventHandle())
            {
                BOOST_THROW_EXCEPTION(IllegalMethodCall());
            }
            if (!removed_)
            {
                running_ = true;
                sink_->Fire();
                running_ = false;
                removed_ = true;
            }
            scheduler_->AfterEvent();
        }

        /*}}}*/

        NSFX_INTERFACE_MAP_BEGIN(EventHandle)
            NSFX_INTERFACE_ENTRY(IEventHandle)
        NSFX_INTERFACE_MAP_END()

    private:
        ListEventScheduler* scheduler_;
        TimePoint t_;
        Ptr<IEventSink<> > sink_;
        bool removed_;
        bool running_;
    }; // class EventHandle /*}}}*/

    typedef Object<EventHandle>  EventHandleClass;

public:
    ListEventScheduler(void) BOOST_NOEXCEPT :
        initialized_(false)
    {}

    virtual ~ListEventScheduler(void) {}

    // IClockUser /*{{{*/
public:
    virtual void UseClock(Ptr<IClock> clock) NSFX_OVERRIDE
    {
        if (clock_)
        {
            BOOST_THROW_EXCEPTION(CannotReinitialize());
        }
        if (!clock)
        {
            BOOST_THROW_EXCEPTION(InvalidPointer());
        }
        clock_ = std::move(clock);
        initialized_ = true;
    }

    /*}}}*/

    // IEventScheduler /*{{{*/
public:
    virtual Ptr<IEventHandle> ScheduleNow(Ptr<IEventSink<> > sink) NSFX_OVERRIDE
    {
        return ThisType::ScheduleAt(clock_->Now(), std::move(sink));
    }

    virtual Ptr<IEventHandle> ScheduleIn(const Duration& dt,
                                         Ptr<IEventSink<> > sink) NSFX_OVERRIDE
    {
        return ThisType::ScheduleAt(clock_->Now() + dt, std::move(sink));
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
        Ptr<EventHandleClass> handle(
            new EventHandleClass(this, t, std::move(sink)));
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
                if (h->GetEnveloped()->EventHandle::GetTimePoint() > t)
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
        return Ptr<IEventHandle>(handle.Detach(), true);
    }

    virtual size_t GetNumEvents(void) BOOST_NOEXCEPT NSFX_OVERRIDE
    {
        return list_.size();
    }

    virtual Ptr<IEventHandle> GetNextEvent(void) BOOST_NOEXCEPT NSFX_OVERRIDE
    {
        return InternalGetNextEventHandle();
    }

private:
    IEventHandle* InternalGetNextEventHandle(void) BOOST_NOEXCEPT
    {
        IEventHandle* result = nullptr;
        if (list_.size())
        {
            result = list_.front().Get();
        }
        return result;
    }

    void AfterEvent(void)
    {
        list_.pop_front();
    }

    /*}}}*/

    // IDisposable /*{{{*/
    virtual void Dispose(void) NSFX_OVERRIDE
    {
        initialized_ = false;
        clock_ = nullptr;
        list_.clear();
    }
    /*}}}*/

    NSFX_INTERFACE_MAP_BEGIN(ListEventScheduler)
        NSFX_INTERFACE_ENTRY(IEventScheduler)
        NSFX_INTERFACE_ENTRY(IClockUser)
        NSFX_INTERFACE_ENTRY(IDisposable)
    NSFX_INTERFACE_MAP_END()

private:
    bool initialized_;
    Ptr<IClock> clock_;
    list<Ptr<EventHandleClass> >  list_;

}; // class ListEventScheduler


NSFX_DEFINE_CLASS_UUID4(ListEventScheduler, 0xD365832F, 0x64C0, 0x4618, 0x8B4D34948267A900LL);
NSFX_REGISTER_CLASS(ListEventScheduler);


NSFX_CLOSE_NAMESPACE


#endif // LIST_EVENT_SCHEDULER_H__38B15EBB_9CE4_4670_A446_55235A1EEF60

