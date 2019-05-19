/**
 * @file
 *
 * @brief Test Simulator.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-01-16
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/simulation/simulator.h>
#include <nsfx/simulation/set-scheduler.h>
#include <nsfx/event/event-sink.h>
#include <iostream>

NSFX_TEST_SUITE(Simulator)
{
    static int counter = 0;
    static bool begin = false;
    static bool run = false;

    struct Sink :/*{{{*/
        nsfx::IClockUser,
        nsfx::ISimulatorUser,
        nsfx::ISchedulerUser,
        nsfx::IEventSink<>
    {
        typedef Sink  ThisClass;

        typedef nsfx::AggObject<nsfx::MemberFunctionBasedEventSink<nsfx::ISimulationBeginEventSink, ThisClass>>  BeginEventSinkClass;
        typedef nsfx::AggObject<nsfx::MemberFunctionBasedEventSink<nsfx::ISimulationRunEventSink,   ThisClass>>  RunEventSinkClass;
        typedef nsfx::AggObject<nsfx::MemberFunctionBasedEventSink<nsfx::ISimulationPauseEventSink, ThisClass>>  PauseEventSinkClass;
        typedef nsfx::AggObject<nsfx::MemberFunctionBasedEventSink<nsfx::ISimulationEndEventSink,   ThisClass>>  EndEventSinkClass;

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

        virtual void Use(nsfx::Ptr<nsfx::IScheduler> scheduler) NSFX_OVERRIDE
        {
            scheduler_ = scheduler;
        }

        virtual void Fire(void) NSFX_OVERRIDE
        {
            NSFX_TEST_EXPECT(begin);
            NSFX_TEST_EXPECT(run);
            ++counter;
            if (counter < 10)
            {
                scheduler_->ScheduleAt(
                    clock_->Now() + nsfx::Seconds(1), this);
            }
            else if (counter == 15)
            {
                scheduler_->ScheduleIn(nsfx::Seconds(1), this);
                simulator_->Pause();
            }
            else if (counter < 20)
            {
                scheduler_->ScheduleIn(nsfx::Seconds(1), this);
            }
        }

        void OnSimulationBegin(void)
        {
            begin = true;
        }

        void OnSimulationRun(void)
        {
            run = true;
        }

        void OnSimulationPause(void)
        {
            run = false;
        }

        void OnSimulationEnd(void)
        {
            begin = false;
        }

        NSFX_INTERFACE_MAP_BEGIN(ThisClass)
            NSFX_INTERFACE_ENTRY(nsfx::IClockUser)
            NSFX_INTERFACE_ENTRY(nsfx::ISimulatorUser)
            NSFX_INTERFACE_ENTRY(nsfx::ISchedulerUser)
            NSFX_INTERFACE_ENTRY(nsfx::IEventSink<>)
        NSFX_INTERFACE_MAP_END()

    private:
        nsfx::Ptr<nsfx::ISimulator> simulator_;
        nsfx::Ptr<nsfx::IClock> clock_;
        nsfx::Ptr<nsfx::IScheduler> scheduler_;
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
            nsfx::Ptr<nsfx::IScheduler> scheduler =
                nsfx::CreateObject<nsfx::IScheduler>(
                    "edu.uestc.nsfx.SetScheduler");

            nsfx::Ptr<nsfx::ISimulator>  simulator =
                nsfx::CreateObject<nsfx::ISimulator>(
                    "edu.uestc.nsfx.Simulator");
            nsfx::Ptr<nsfx::IClock>  clock(simulator);

            nsfx::Ptr<SinkClass>  sink(new SinkClass);
            nsfx::Ptr<nsfx::IEventSink<>>  eventSink(sink);

            // Wire simulator.
            {
                nsfx::Ptr<nsfx::ISchedulerUser>(simulator)->Use(scheduler);
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
            scheduler->ScheduleAt(clock->Now() + nsfx::Seconds(1), eventSink);

            // run to 1s.
            simulator->RunUntil(clock->Now() + nsfx::Seconds(1));
            NSFX_TEST_EXPECT_EQ(counter, 1);
            NSFX_TEST_EXPECT(begin);
            NSFX_TEST_EXPECT(!run);

            // run to 10s.
            simulator->RunFor(nsfx::chrono::Seconds(9));
            NSFX_TEST_EXPECT_EQ(counter, 10);
            NSFX_TEST_EXPECT(begin);
            NSFX_TEST_EXPECT(!run);

            // run to the end (20s).
            // The simulator will be paused at 15s.
            simulator->Run();
            NSFX_TEST_EXPECT_EQ(counter, 15);
            NSFX_TEST_EXPECT(begin);
            NSFX_TEST_EXPECT(!run);

            // run to the end (20s).
            simulator->Run();
            NSFX_TEST_EXPECT_EQ(counter, 20);
            NSFX_TEST_EXPECT(!begin);
            NSFX_TEST_EXPECT(!run);

        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e) << std::endl;
        }
        catch (std::exception& e)
        {
            NSFX_TEST_EXPECT(false) << e.what() << std::endl;
        }
    }
}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cerr);
    nsfx::test::runner::Run();

    return 0;
}

