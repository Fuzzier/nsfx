#include <nsfx/test.h>
#include <nsfx/simulator/simulator.h>
#include <nsfx/simulator/list-event-scheduler.h>
#include <iostream>

#define PTR(x)  nsfx::Ptr<x, false>

NSFX_TEST_SUITE(Simulator)
{
    static int counter = 0;
    struct Sink :/*{{{*/
        nsfx::IEventSink,
        nsfx::ISimulatorSink,
        nsfx::IClockUser,
        nsfx::ISimulatorUser,
        nsfx::IEventSchedulerUser
    {
        virtual ~Sink(void) {}

        virtual void UseClock(nsfx::Ptr<nsfx::IClock> clock) NSFX_OVERRIDE
        {
            clock_ = clock;
        }

        virtual void UseSimulator(nsfx::Ptr<nsfx::ISimulator> simulator) NSFX_OVERRIDE
        {
            simulator_ = simulator;
        }

        virtual void UseEventScheduler(nsfx::Ptr<nsfx::IEventScheduler> scheduler) NSFX_OVERRIDE
        {
            scheduler_ = scheduler;
        }

        virtual void OnEvent(void) NSFX_OVERRIDE
        {
            std::cout << clock_->Now() << ": " << ++counter << std::endl;
            if (counter < 10)
            {
                scheduler_->ScheduleAt(
                    clock_->Now() + nsfx::chrono::Seconds(1), this);
            }
            else if (counter < 20)
            {
                scheduler_->ScheduleIn(nsfx::chrono::Seconds(1), this);
            }
        }

        virtual void OnSimulatorEvent(nsfx::SimulatorEventType type) NSFX_OVERRIDE
        {
            switch (type)
            {
            case nsfx::NSFX_SIMULATOR_EVENT_BEGIN:
                std::cout << "BEGIN" << std::endl;
                break;
            case nsfx::NSFX_SIMULATOR_EVENT_RUN:
                std::cout << "RUN" << std::endl;
                break;
            case nsfx::NSFX_SIMULATOR_EVENT_PAUSE:
                std::cout << "PAUSE" << std::endl;
                break;
            case nsfx::NSFX_SIMULATOR_EVENT_END:
                std::cout << "END" << std::endl;
                simulator_ = nullptr;
                scheduler_ = nullptr;
                clock_ = nullptr;
                break;
            default:
                break;
            }
        }

        NSFX_INTERFACE_MAP_BEGIN(Sink)
            NSFX_INTERFACE_ENTRY(IEventSink)
            NSFX_INTERFACE_ENTRY(ISimulatorSink)
            NSFX_INTERFACE_ENTRY(IClockUser)
            NSFX_INTERFACE_ENTRY(ISimulatorUser)
            NSFX_INTERFACE_ENTRY(IEventSchedulerUser)
        NSFX_INTERFACE_MAP_END()

    private:
        nsfx::Ptr<nsfx::ISimulator> simulator_;
        nsfx::Ptr<nsfx::IClock> clock_;
        nsfx::Ptr<nsfx::IEventScheduler> scheduler_;
    };/*}}}*/

    NSFX_TEST_CASE(Simulator)
    {
        typedef nsfx::Object<Sink>  SinkType;
        try
        {
            counter = 0;

            // Create objects.
            PTR(nsfx::IEventScheduler) scheduler =
                nsfx::CreateObject<nsfx::IEventScheduler>(
                    NSFX_CID_ListEventScheduler);

            PTR(nsfx::ISimulator)  simulator =
                nsfx::CreateObject<nsfx::ISimulator>(
                    NSFX_CID_Simulator);
            PTR(nsfx::IClock)  clock(simulator);

            PTR(SinkType)  sink(new SinkType);
            PTR(nsfx::IEventSink)  eventSink(sink);
            PTR(nsfx::ISimulatorSink)  simulatorSink(sink);

            // Wire simulator.
            {
                PTR(nsfx::IEventSchedulerUser) u(simulator);
                u->UseEventScheduler(scheduler);
            }
            // Wire scheduler.
            {
                PTR(nsfx::IClockUser) u(scheduler);
                u->UseClock(clock);
            }
            // Wire sink.
            {
                sink->UseClock(clock);
                sink->UseSimulator(simulator);
                sink->UseEventScheduler(scheduler);
            }

            nsfx::cookie_t cookie = simulator->Connect(
                simulatorSink, nsfx::NSFX_SIMULATOR_EVENT_ALL);
            simulator->Disconnect(cookie);
            cookie = simulator->Connect(
                simulatorSink, nsfx::NSFX_SIMULATOR_EVENT_ALL);

            // start at 1s.
            scheduler->ScheduleAt(clock->Now() + nsfx::chrono::Seconds(1), eventSink);

            // run to 1s.
            simulator->RunUntil(clock->Now() + nsfx::chrono::Seconds(1));
            NSFX_TEST_EXPECT_EQ(counter, 1);

            // run to 10s.
            simulator->RunFor(nsfx::chrono::Seconds(9));
            NSFX_TEST_EXPECT_EQ(counter, 10);

            // run to the end (20s).
            simulator->Run();
            NSFX_TEST_EXPECT_EQ(counter, 20);
        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e);
        }
        catch (std::exception& e)
        {
            NSFX_TEST_EXPECT(false) << e.what();
        }
    }
}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

