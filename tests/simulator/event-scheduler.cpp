#include <nsfx/test.h>
#include <nsfx/simulator/event-scheduler.h>
#include <iostream>


NSFX_TEST_SUITE(EventScheduler)
{
    ////////////////////////////////////////////////////////////////////////////
    // Types.
    using nsfx::refcount_t;

    static nsfx::TimePoint g_t;
    struct AlarmClock : nsfx::IClock, nsfx::IAlarm /*{{{*/
    {
        virtual ~AlarmClock(void) {}

        // IClock
        virtual nsfx::TimePoint Now(void)
        {
            return t;
        }

        // IAlarm
        virtual void Connect(nsfx::Ptr<nsfx::IAlarmSink> sink)
        {
            sink_ = sink;
        }

        virtual void Disconnect(void)
        {
            sink_.Reset();
        }

        virtual void ScheduleAt(const nsfx::TimePoint& t)
        {
            t_ = t;
        }

        virtual void ScheduleIn(const Duration& dt)
        {
            t_ = g_t + dt;
        }

        virtual bool IsPending(void)
        {
            return scheduled_;
        }

        virtual void Cancel(void)
        {
            scheduled_ = false;
        }

        // Methods
        void Fire(void)
        {
            if (sink_)
            {
                sink_->Fire();
            }
        }

        NSFX_INTERFACE_MAP_BEGIN(AlarmClock)
            NSFX_INTERFACE_ENTRY(nsfx::IClock)
            NSFX_INTERFACE_ENTRY(nsfx::IAlarm)
        NSFX_INTERFACE_MAP_END()

    private:
        nsfx::Ptr<nsfx::IAlarmSink> sink_;
        nsfx::TimePoint t_;
        bool scheduled_;

    };/*}}}*/

    static bool fired = false;
    struct Sink : nsfx::IEventSink/*{{{*/
    {
        virtual ~Sink(void) {}

        virtual void Fire(void)
        {
            fired = true;
        }

        NSFX_INTERFACE_MAP_BEGIN(Sink)
            NSFX_INTERFACE_ENTRY(IEventSink)
        NSFX_INTERFACE_MAP_END()

    };/*}}}*/

    NSFX_TEST_CASE(IEventScheduler)
    {
        nsfx::Ptr<nsfx::IEventScheduler> sch =
            nsfx::CreateObject<nsfx::IEventScheduler>(
                nsfx::uuid_of<nsfx::EventScheduler>(), nullptr);
    }
}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

