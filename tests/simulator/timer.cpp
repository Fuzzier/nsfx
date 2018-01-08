#include <nsfx/test.h>
#include <nsfx/simulator/timer.h>
#include <nsfx/simulator/simulator.h>
#include <nsfx/simulator/list-event-scheduler.h>
#include <iostream>


#define PTR(x)  nsfx::Ptr<x, false>


NSFX_TEST_SUITE(Timer)
{
    using nsfx::Ptr;

    static uint32_t count = 0;
    struct Sink : nsfx::ITimerSink/*{{{*/
    {
        Sink() {}
        Sink(Ptr<nsfx::IClock> clock) :
            clock_(std::move(clock))
        {}

        virtual ~Sink(void) {}

        virtual void OnTimeout(void) NSFX_OVERRIDE
        {
            ++count;
            std::cout << clock_->Now() << std::endl;
        }

        NSFX_INTERFACE_MAP_BEGIN(Sink)
            NSFX_INTERFACE_ENTRY(nsfx::ITimerSink)
        NSFX_INTERFACE_MAP_END()

    private:
        Ptr<nsfx::IClock>  clock_;
    };/*}}}*/

    typedef nsfx::Object<Sink>  SinkType;

    NSFX_TEST_CASE(StartAt)/*{{{*/
    {
        try
        {
            PTR(nsfx::ISimulator)  simulator =
                nsfx::CreateObject<nsfx::ISimulator>(NSFX_CID_Simulator);
            PTR(nsfx::IClock)  clock(simulator);
            PTR(nsfx::IEventScheduler)  scheduler =
                nsfx::CreateObject<nsfx::IEventScheduler>(
                    NSFX_CID_ListEventScheduler);
            // wire scheduler
            {
                PTR(nsfx::IClockUser)  u(scheduler);
                u->UseClock(clock);
            }
            // wire simulator
            {
                PTR(nsfx::IEventSchedulerUser)  u(simulator);
                u->UseEventScheduler(scheduler);
            }

            PTR(nsfx::ITimer)  timer =
                nsfx::CreateObject<nsfx::ITimer>(NSFX_CID_Timer);
            // wire timer
            {
                PTR(nsfx::IClockUser) u(timer);
                u->UseClock(clock);
            }
            {
                PTR(nsfx::IEventSchedulerUser) u(timer);
                u->UseEventScheduler(scheduler);
            }

            nsfx::TimePoint t0(nsfx::Seconds(1));
            nsfx::Duration  p0(nsfx::Seconds(2));
            PTR(SinkType)  sink(new SinkType(clock));

            count = 0;
            PTR(nsfx::ITimerHandle)  h0 = timer->StartAt(t0, p0, sink);
            simulator->RunUntil(t0);
            NSFX_TEST_EXPECT_EQ(count, 1);

            simulator->RunUntil(t0 + nsfx::Seconds(8));
            NSFX_TEST_EXPECT_EQ(count, 1 + 8 / 2);

            nsfx::Duration  p1(nsfx::Seconds(1));
            PTR(nsfx::ITimerHandle)  h1 = timer->StartNow(p1, sink);
            simulator->RunFor(nsfx::Seconds(8));
            NSFX_TEST_EXPECT_EQ(count, 5 + 8 / 2 + 9);

            h0->Stop();
            simulator->RunFor(nsfx::Seconds(4));

            h1->Stop();
            simulator->RunFor(nsfx::Seconds(4));
            NSFX_TEST_EXPECT_EQ(count, 22);
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e);
        }
    }/*}}}*/

}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

