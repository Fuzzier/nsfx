#include <nsfx/test.h>
#include <nsfx/simulation/timer.h>
#include <nsfx/simulation/simulator.h>
#include <nsfx/simulation/list-event-scheduler.h>
#include <iostream>


NSFX_TEST_SUITE(Timer)
{
    static uint32_t count = 0;
    struct Sink : nsfx::IEventSink<>/*{{{*/
    {
        Sink() {}
        Sink(nsfx::Ptr<nsfx::IClock> clock) :
            clock_(std::move(clock))
        {}

        virtual ~Sink(void) {}

        // IEventSink<>
        virtual void Fire(void) NSFX_OVERRIDE
        {
            ++count;
            std::cout << clock_->Now() << std::endl;
        }

        NSFX_INTERFACE_MAP_BEGIN(Sink)
            NSFX_INTERFACE_ENTRY(nsfx::IEventSink<>)
        NSFX_INTERFACE_MAP_END()

    private:
        nsfx::Ptr<nsfx::IClock>  clock_;
    };/*}}}*/

    typedef nsfx::Object<Sink>  SinkType;

    NSFX_TEST_CASE(StartAt)/*{{{*/
    {
        try
        {
            nsfx::Ptr<nsfx::ISimulator>  simulator =
                nsfx::CreateObject<nsfx::ISimulator>(NSFX_CID_Simulator);
            nsfx::Ptr<nsfx::IClock>  clock(simulator);
            nsfx::Ptr<nsfx::IEventScheduler>  scheduler =
                nsfx::CreateObject<nsfx::IEventScheduler>(
                    NSFX_CID_ListEventScheduler);
            // wire scheduler
            nsfx::Ptr<nsfx::IClockUser>(scheduler)->UseClock(clock);
            // wire simulator
            nsfx::Ptr<nsfx::IEventSchedulerUser>(simulator)->UseEventScheduler(scheduler);

            nsfx::Ptr<nsfx::ITimer>  timer =
                nsfx::CreateObject<nsfx::ITimer>(NSFX_CID_Timer);
            // wire timer
            nsfx::Ptr<nsfx::IClockUser>(timer)->UseClock(clock);
            nsfx::Ptr<nsfx::IEventSchedulerUser>(timer)->UseEventScheduler(scheduler);

            nsfx::TimePoint t0(nsfx::Seconds(1));
            nsfx::Duration  p0(nsfx::Seconds(2));
            nsfx::Ptr<SinkType>  sink(new SinkType(clock));

            count = 0;
            nsfx::Ptr<nsfx::ITimerHandle>  h0 = timer->StartAt(t0, p0, sink);
            simulator->RunUntil(t0);
            NSFX_TEST_EXPECT_EQ(count, 1);

            simulator->RunUntil(t0 + nsfx::Seconds(8));
            NSFX_TEST_EXPECT_EQ(count, 1 + 8 / 2);

            nsfx::Duration  p1(nsfx::Seconds(1));
            nsfx::Ptr<nsfx::ITimerHandle>  h1 = timer->StartNow(p1, sink);
            simulator->RunFor(nsfx::Seconds(8));
            NSFX_TEST_EXPECT_EQ(count, 5 + 8 / 2 + 9);

            h0->Stop();
            simulator->RunFor(nsfx::Seconds(4));

            h1->Stop();
            simulator->RunFor(nsfx::Seconds(4));
            NSFX_TEST_EXPECT_EQ(count, 22);

            nsfx::Ptr<nsfx::IDisposable>(simulator)->Dispose();
            nsfx::Ptr<nsfx::IDisposable>(scheduler)->Dispose();
            nsfx::Ptr<nsfx::IDisposable>(timer)->Dispose();
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e);
        }
        catch (std::exception& e)
        {
            NSFX_TEST_EXPECT(false) << e.what();
        }
    }/*}}}*/

}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

