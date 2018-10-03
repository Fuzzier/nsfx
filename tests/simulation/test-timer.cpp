/**
 * @file
 *
 * @brief Test Timer.
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
#include <nsfx/simulation/timer.h>
#include <nsfx/simulation/simulator.h>
#include <nsfx/simulation/set-event-scheduler.h>
#include <iostream>


NSFX_TEST_SUITE(Timer)
{
    uint32_t count = 0;
    nsfx::TimePoint t0(nsfx::Seconds(1));
    nsfx::Duration  p0(nsfx::Seconds(2));

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
            NSFX_TEST_EXPECT_EQ(clock_->Now(), t0 + p0 * count);
            ++count;
        }

        NSFX_INTERFACE_MAP_BEGIN(Sink)
            NSFX_INTERFACE_ENTRY(nsfx::IEventSink<>)
        NSFX_INTERFACE_MAP_END()

    private:
        nsfx::Ptr<nsfx::IClock>  clock_;
    };/*}}}*/

    typedef nsfx::Object<Sink>  SinkClass;

    NSFX_TEST_CASE(Start)/*{{{*/
    {
        try
        {
            nsfx::Ptr<nsfx::ISimulator>  simulator =
                nsfx::CreateObject<nsfx::ISimulator>(
                    "edu.uestc.nsfx.Simulator");
            nsfx::Ptr<nsfx::IClock>  clock(simulator);
            nsfx::Ptr<nsfx::IEventScheduler>  scheduler =
                nsfx::CreateObject<nsfx::IEventScheduler>(
                    "edu.uestc.nsfx.SetEventScheduler");
            // wire scheduler
            nsfx::Ptr<nsfx::IClockUser>(scheduler)->Use(clock);
            // wire simulator
            nsfx::Ptr<nsfx::IEventSchedulerUser>(simulator)->Use(scheduler);

            nsfx::Ptr<nsfx::ITimer>  timer =
                nsfx::CreateObject<nsfx::ITimer>(
                    "edu.uestc.nsfx.Timer");
            // wire timer
            nsfx::Ptr<nsfx::IClockUser>(timer)->Use(clock);
            nsfx::Ptr<nsfx::IEventSchedulerUser>(timer)->Use(scheduler);

            nsfx::Ptr<SinkClass>  sink(new SinkClass(clock));

            count = 0;
            t0 = nsfx::TimePoint(nsfx::Seconds(1));
            p0 = nsfx::Seconds(2);

            timer->StartAt(t0, p0, sink);
            simulator->RunUntil(t0);
            NSFX_TEST_EXPECT_EQ(count, 1);

            simulator->RunUntil(t0 + nsfx::Seconds(8));
            NSFX_TEST_EXPECT_EQ(count, 1 + 8 / 2);

            timer->Stop();

            count = 0;
            t0 = clock->Now();
            p0 = nsfx::Seconds(2);
            timer->StartNow(p0, sink);
            simulator->RunFor(nsfx::Seconds(8));
            NSFX_TEST_EXPECT_EQ(count, 5);

        }
        catch (boost::exception& e)
        {
            NSFX_TEST_EXPECT(false) << diagnostic_information(e) << std::endl;
        }
        catch (std::exception& e)
        {
            NSFX_TEST_EXPECT(false) << e.what() << std::endl;
        }
    }/*}}}*/

}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cerr);
    nsfx::test::runner::Run();

    return 0;
}

