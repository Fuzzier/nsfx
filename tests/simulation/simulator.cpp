#include <nsfx/test.h>
#include <nsfx/simulation/simulator.h>
#include <nsfx/simulation/set-event-scheduler.h>
#include <nsfx/event/event-sink.h>
#include <nsfx/component/i-disposable.h>
#include <iostream>

NSFX_TEST_SUITE(Simulator)
{
    static int counter = 0;
    struct Sink :/*{{{*/
        nsfx::IClockUser,
        nsfx::ISimulatorUser,
        nsfx::IEventSchedulerUser,
        nsfx::IEventSink<>,
        nsfx::IDisposable
    {
        typedef Sink  ThisClass;

        typedef nsfx::AggObject<nsfx::MemberFunctionBasedEventSink<nsfx::ISimulationBeginEventSink, ThisClass> >  BeginEventSinkClass;
        typedef nsfx::AggObject<nsfx::MemberFunctionBasedEventSink<nsfx::ISimulationRunEventSink,   ThisClass> >  RunEventSinkClass;
        typedef nsfx::AggObject<nsfx::MemberFunctionBasedEventSink<nsfx::ISimulationPauseEventSink, ThisClass> >  PauseEventSinkClass;
        typedef nsfx::AggObject<nsfx::MemberFunctionBasedEventSink<nsfx::ISimulationEndEventSink,   ThisClass> >  EndEventSinkClass;

        virtual ~Sink(void) {}

        virtual void Use(nsfx::Ptr<nsfx::IClock> clock) NSFX_OVERRIDE
        {
            clock_ = clock;
        }

        virtual void Use(nsfx::Ptr<nsfx::ISimulator> simulator) NSFX_OVERRIDE
        {
            simulator_ = simulator;
            beginSink_ = new BeginEventSinkClass(this, this, &ThisClass::OnSimulationBegin);
            runSink_   = new RunEventSinkClass  (this, this, &ThisClass::OnSimulationRun);
            pauseSink_ = new PauseEventSinkClass(this, this, &ThisClass::OnSimulationPause);
            endSink_   = new EndEventSinkClass  (this, this, &ThisClass::OnSimulationEnd);
            beginSinkCookie_ = nsfx::Ptr<nsfx::ISimulationBeginEvent>(simulator_)->Connect(beginSink_);
            runSinkCookie_   = nsfx::Ptr<nsfx::ISimulationRunEvent>(simulator_)->Connect(runSink_);
            pauseSinkCookie_ = nsfx::Ptr<nsfx::ISimulationPauseEvent>(simulator_)->Connect(pauseSink_);
            endSinkCookie_   = nsfx::Ptr<nsfx::ISimulationEndEvent>(simulator_)->Connect(endSink_);
        }

        virtual void Use(nsfx::Ptr<nsfx::IEventScheduler> scheduler) NSFX_OVERRIDE
        {
            scheduler_ = scheduler;
        }

        virtual void Fire(void) NSFX_OVERRIDE
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

        virtual void Dispose(void) NSFX_OVERRIDE
        {
            nsfx::Ptr<nsfx::ISimulationBeginEvent>(simulator_)->Disconnect(beginSinkCookie_);
            nsfx::Ptr<nsfx::ISimulationRunEvent>(simulator_)->Disconnect(runSinkCookie_);
            nsfx::Ptr<nsfx::ISimulationPauseEvent>(simulator_)->Disconnect(pauseSinkCookie_);
            nsfx::Ptr<nsfx::ISimulationEndEvent>(simulator_)->Disconnect(endSinkCookie_);
            simulator_ = nullptr;
            clock_     = nullptr;
            scheduler_ = nullptr;
            beginSink_ = nullptr;
            runSink_   = nullptr;
            pauseSink_ = nullptr;
            endSink_   = nullptr;
        }

        void OnSimulationBegin(void)
        {
            std::cout << "BEGIN" << std::endl;
        }

        void OnSimulationRun(void)
        {
            std::cout << "RUN" << std::endl;
        }

        void OnSimulationPause(void)
        {
            std::cout << "PAUSE" << std::endl;
        }

        void OnSimulationEnd(void)
        {
            std::cout << "END" << std::endl;
        }

        NSFX_INTERFACE_MAP_BEGIN(ThisClass)
            NSFX_INTERFACE_ENTRY(nsfx::IClockUser)
            NSFX_INTERFACE_ENTRY(nsfx::ISimulatorUser)
            NSFX_INTERFACE_ENTRY(nsfx::IEventSchedulerUser)
            NSFX_INTERFACE_ENTRY(nsfx::IDisposable)
            NSFX_INTERFACE_ENTRY(nsfx::IEventSink<>)
        NSFX_INTERFACE_MAP_END()

    private:
        nsfx::Ptr<nsfx::ISimulator> simulator_;
        nsfx::Ptr<nsfx::IClock> clock_;
        nsfx::Ptr<nsfx::IEventScheduler> scheduler_;
        nsfx::Ptr<nsfx::IObject>  beginSink_;
        nsfx::Ptr<nsfx::IObject>  runSink_;
        nsfx::Ptr<nsfx::IObject>  pauseSink_;
        nsfx::Ptr<nsfx::IObject>  endSink_;
        nsfx::cookie_t  beginSinkCookie_;
        nsfx::cookie_t  runSinkCookie_;
        nsfx::cookie_t  pauseSinkCookie_;
        nsfx::cookie_t  endSinkCookie_;
    };/*}}}*/

    NSFX_TEST_CASE(Simulator)
    {
        typedef nsfx::Object<Sink>  SinkClass;
        try
        {
            counter = 0;

            // Create objects.
            nsfx::Ptr<nsfx::IEventScheduler> scheduler =
                nsfx::CreateObject<nsfx::IEventScheduler>(
                    NSFX_CID_SetEventScheduler);

            nsfx::Ptr<nsfx::ISimulator>  simulator =
                nsfx::CreateObject<nsfx::ISimulator>(
                    NSFX_CID_Simulator);
            nsfx::Ptr<nsfx::IClock>  clock(simulator);

            nsfx::Ptr<SinkClass>  sink(new SinkClass);
            nsfx::Ptr<nsfx::IEventSink<> >  eventSink(sink);

            // Wire simulator.
            {
                nsfx::Ptr<nsfx::IEventSchedulerUser>(simulator)->Use(scheduler);
            }
            // Wire scheduler.
            {
                nsfx::Ptr<nsfx::IClockUser>(scheduler)->Use(clock);
            }
            // Wire sink.
            {
                sink->Use(clock);
                sink->Use(simulator);
                sink->Use(scheduler);
            }

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

            nsfx::Ptr<nsfx::IDisposable>(simulator)->Dispose();
            nsfx::Ptr<nsfx::IDisposable>(scheduler)->Dispose();
            nsfx::Ptr<nsfx::IDisposable>(sink)->Dispose();
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

