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

#ifndef EVENT_SCHEDULER_H__38B15EBB_9CE4_4670_A446_55235A1EEF60
#define EVENT_SCHEDULER_H__38B15EBB_9CE4_4670_A446_55235A1EEF60


#include <nsfx/simulator/config.h>
#include <nsfx/simulator/i-event-scheduler.h>
#include <nsfx/component/class-registry.h>
#include <functional>
#include <memory>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// EventScheduler.
/**
 * @ingroup Simulator
 * @brief The event scheduler.
 *
 * Interfaces:<br/>
 * \c IEventSchedulerInitialize, \c IEventScheduler and \c IAlarm.</br>
 * The \c IAlarm is a virtualized alarm.
 *
 * The default alarm scheduler is built on \c IClock and \c IAlarm interfaces.</br>
 * It provides multiplexed alarms that virtualizes the \c IAlarm interface, such
 * that each user seems to have a dictated alarm.</br>
 *
 * The main job of this event scheduler is to manage an event queue, and
 * connect the earliest event to the alarm.</br>
 */
class EventScheduler :
    public IEventSchedulerInitialize,
    public IEventScheduler
{
private:
    typedef EventScheduler  ThisType;

    class EventHandle :/*{{{*/
        public IEventHandle
    {
    public:
        EventHandle(void) :
            scheduler_(nullptr),
            removed_(false),
            running_(false)
        {
        }

        virtual ~EventHandle(void) {}

        void Initialize(EventScheduler* scheduler,
                        const TimePoint& t,
                        Ptr<IEventSink> sink)
        {
            scheduler_ = scheduler;
            t_ = t;
            sink_ = std::move(sink);
        }

        // IEventHandle /*{{{*/
        virtual bool IsPending(void) NSFX_OVERRIDE
        {
            return (!removed_ && !running_);
        }

        virtual bool IsRunning(void) NSFX_OVERRIDE
        {
            return running_;
        }

        virtual void IsValid(void) NSFX_OVERRIDE
        {
            return (!removed_);
        }

        virtual void Cancel(void) NSFX_OVERRIDE
        {
            removed_ = true;
        }

        virtual TimePoint GetTimePoint(void) NSFX_OVERRIDE
        {
            return t_;
        }

        /*}}}*/

        void Fire(void)
        {
            if (!removed_)
            {
                running_ = true;
                sink_->Fire();
                removed_ = true;
                running_ = false;
            }
            scheduler_->AfterEventFired();
        }

        IEventSink* GetSink(void) BOOST_NOEXCEPT
        {
            return sink_.Get();
        }

        NSFX_INTERFACE_MAP_BEGIN(EventHandle)
            NSFX_INTERFACE_ENTRY(IEventHandle)
        NSFX_INTERFACE_MAP_END()

    private:
        EventScheduler* scheduler_;
        TimePoint t_;
        Ptr<IEventSink> sink_;
        bool removed_;
        bool running_;
    }; // class EventHandle

    /*}}}*/

    typedef Object<EventHandle>  EventHandleType;

    class Alarm :/*{{{*/
        public IAlarm,
        public IEventSink
    {
    public:
        typedef Alarm  ThisType;

        Alarm(void) :
            scheduler_(nullptr)
        {
        }

        virtual ~AlarmSink(void) {}

        void Initialize(EventScheduler* scheduler)
        {
            scheduler_ = scheduler;
        }

        // IAlarm /*{{{*/
        virtual void Connect(Ptr<IAlarmSink> sink) NSFX_OVERRIDE
        {
            if (!sink)
            {
                BOOST_THROW_EXCEPTION(InvalidPointer());
            }
            if (sink_)
            {
                BOOST_THROW_EXCEPTION(ConnectionLimit());
            }
            sink_ = std::move(sink);
        }

        virtual void Disconnect(void) NSFX_OVERRIDE
        {
            if (!sink_)
            {
                BOOST_THROW_EXCEPTION(NotConnected());
            }
            ThisType::Cancel();
            sink_.Reset();
        }

        virtual void ScheduleAt(const TimePoint& t) NSFX_OVERRIDE
        {
            if (!sink_)
            {
                BOOST_THROW_EXCEPTION(NotConnected());
            }
            handle_ = scheduler_->
                EventScheduler::ScheduleAt(
                    t, Ptr<IEventSink>(static_cast<IEventSink>(this)));
        }

        virtual void ScheduleIn(const Duration& dt) NSFX_OVERRIDE
        {
            if (!sink_)
            {
                BOOST_THROW_EXCEPTION(NotConnected());
            }
            handle_ = scheduler_->
                EventScheduler::ScheduleIn(
                    dt, Ptr<IEventSink>(static_cast<IEventSink>(this)));
        }

        virtual bool IsPending(void) NSFX_OVERRIDE
        {
            bool result = handle_ ? handle_->IsPending() : false;
            return result;
        }

        virtual void Cancel(void) NSFX_OVERRIDE
        {
            if (handle_)
            {
                handle_->Cancel();
                handle_.Reset();
            }
        }

        /*}}}*/

        // IAlarmSink /*{{{*/
        virtual void Fire(void) NSFX_OVERRIDE
        {
            sink_->Fire();
            handle_.Reset();
        }
        /*}}}*/

        NSFX_INTERFACE_MAP_BEGIN(Alarm)
            NSFX_INTERFACE_ENTRY(IAlarm)
            NSFX_INTERFACE_ENTRY(IEventSink)
        NSFX_INTERFACE_MAP_END()

    private:
        EventScheduler* scheduler_;
        Ptr<IAlarmSink> sink_;
        Ptr<IEventHandle> handle_;
    }; // class Alarm

    /*}}}*/

    typedef AggObject<Alarm>  AggAlarmType;

public:
    EventScheduler(void) BOOST_NOEXCEPT :
        initialized_(false)
    {
        alarm1_.Initialize(this);
    }

    virtual ~EventScheduler(void) BOOST_NOEXCEPT {}

    // IEventSchedulerInitialize /*{{{*/
    virtual void SetEventClock(Ptr<IObject> alarmClock) NSFX_OVERRIDE
    {
        if (initialized_)
        {
            BOOST_THROW_EXCEPTION(IllegalMethodCall());
        }
        if (!alarmClock)
        {
            BOOST_THROW_EXCEPTION(InvalidPointer());
        }
        if (!(alarm_ = alarmClock))
        {
            BOOST_THROW_EXCEPTION(NoInterface());
        }
        if (!(clock_ = alarmClock))
        {
            BOOST_THROW_EXCEPTION(NoInterface());
        }
        initialized_ = true;
    }

    /*}}}*/

    // IEventScheduler /*{{{*/
    virtual Ptr<IEventHandle> ScheduleNow(Ptr<IEventSink> sink) NSFX_OVERRIDE
    {
        ThisType::ScheduleAt(clock_->Now(), std::move(sink), handle);
    }

    virtual Ptr<IEventHandle> ScheduleIn(const Duration& dt,
                                         Ptr<IEventSink> sink) NSFX_OVERRIDE
    {
        ThisType::ScheduleAt(t - clock_->Now(), std::move(sink), handle);
    }

    virtual Ptr<IEventHandle> ScheduleAt(const TimePoint& t,
                                         Ptr<IEventSink> sink) NSFX_OVERRIDE
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
            Ptr<EventHandleType> handle(new EventHandleType);
        }
        catch (std::bad_alloc& )
        {
            BOOST_THROW_EXCEPTION(OutOfMemory());
        }
        handle->Initialize(t, std::move(sink));
        if (!list_.size())
        {
            list_.push_front(handle);
            ConnectEvent(handle.Get());
        }
        else // if (list_.size() > 0)
        {
            bool inserted = false;
            for (auto it = list_.begin(); it != list_.end(); ++it)
            {
                // Call the non-virtual function, since it is known that
                // GetTimePoint() is implemented by EventHandle class.
                if (it->EventHandle::GetTimePoint() > t)
                {
                    auto it0 = list_.insert(it, handle);
                    inserted = true;
                    // If the first event is replaced.
                    if (it0 == list_.begin())
                    {
                        ReconnectEvent(handle.Get());
                    }
                    break;
                }
            }
            if (!inserted)
            {
                list_.push_back(handle);
            }
        }
        return Ptr<IEventHandle>(handle, true);
    }

    virtual size_t GetNumEvents(void) BOOST_NOEXCEPT NSFX_OVERRIDE
    {
        return list_.size();
    }

    virtual IEventHandle* GetNextEvent(void) BOOST_NOEXCEPT NSFX_OVERRIDE
    {
        IEventHandle* result = nullptr;
        if (list_.size())
        {
            auto it = list_.begin();
            // static cast EventHandleType* to IEventHandle* since it is
            // already known.
            result = static_cast<IEventHandle*>(it->Get());
        }
        return result;
    }

    void ConnectEvent(EventHandle* handle)
    {
        alarm_->Connect(handle->GetSink());
    }

    void ReconnectEvent(EventHandle* handle)
    {
        alarm_->Disconnect();
        ConnectEvent(handle);
    }

    void AfterEventFired(void)
    {
        list_.pop_front();
        alarm_->Disconnect();
        if (list_.size())
        {
            ConnectEvent(list_.front().Get());
        }
    }

    /*}}}*/

    NSFX_INTERFACE_MAP_BEGIN(EventScheduler)
        NSFX_INTERFACE_ENTRY(IEventSchedulerInitialize)
        NSFX_INTERFACE_ENTRY(IEventScheduler)
        NSFX_INTERFACE_AGGREGATED_ENTRY(IAlarm, &alarm1_)
    NSFX_INTERFACE_MAP_END()

private:
    bool initialized_;
    Ptr<IAlarm> alarm_;
    Ptr<IClock> clock_;
    AggAlarmType alarm1_;
    list<Ptr<EventHandleType> >  list_;

}; // class EventScheduler


NSFX_DEFINE_CLASS_UUID4(EventScheduler, 0xD365832F, 0x64C0, 0x4618, 0x8B4D34948267A900LL);
NSFX_REGISTER_CLASS(EventScheduler);


NSFX_CLOSE_NAMESPACE


#endif // EVENT_SCHEDULER_H__38B15EBB_9CE4_4670_A446_55235A1EEF60

