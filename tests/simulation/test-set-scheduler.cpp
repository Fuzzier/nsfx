/**
 * @file
 *
 * @brief Test SetScheduler.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-01-20
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/simulation/set-scheduler.h>
#include <iostream>
#include <random>


NSFX_TEST_SUITE(SetScheduler)
{
    using nsfx::Ptr;

    static nsfx::TimePoint tp;
    struct Sink : nsfx::IEventSink<> /*{{{*/
    {
        Sink() {}
        Sink(nsfx::TimePoint tp) : tp_(tp) {}

        virtual ~Sink(void) {}

        virtual void Fire(void) NSFX_OVERRIDE
        {
            tp = tp_;
        }

        NSFX_INTERFACE_MAP_BEGIN(Sink)
            NSFX_INTERFACE_ENTRY(nsfx::IEventSink<>)
        NSFX_INTERFACE_MAP_END()

    private:
        nsfx::TimePoint tp_;
    };/*}}}*/
    typedef nsfx::Object<Sink>  SinkClass;

    static nsfx::TimePoint clk;
    struct Clock : nsfx::IClock/*{{{*/
    {
        virtual ~Clock() {}

        virtual nsfx::TimePoint Now() NSFX_OVERRIDE
        {
            return clk;
        }

        NSFX_INTERFACE_MAP_BEGIN(Clock)
            NSFX_INTERFACE_ENTRY(nsfx::IClock)
        NSFX_INTERFACE_MAP_END()

    };/*}}}*/
    typedef nsfx::Object<Clock>  ClockClass;
    NSFX_REGISTER_CLASS(Clock, "edu.uestc.nsfx.test.Clock");

    NSFX_TEST_CASE(ExternalDriven)/*{{{*/
    {
        try
        {
            Ptr<nsfx::IScheduler> sch =
                nsfx::CreateObject<nsfx::IScheduler>(
                    "edu.uestc.nsfx.SetScheduler");
            {
                Ptr<nsfx::IClock> clock =
                    nsfx::CreateObject<nsfx::IClock>(
                        "edu.uestc.nsfx.test.Clock");
                Ptr<nsfx::IClockUser>(sch)->Use(clock);
            }
            nsfx::TimePoint t1(nsfx::Duration(1));
            nsfx::TimePoint t2(nsfx::Duration(2));
            nsfx::TimePoint t3(nsfx::Duration(3));
            Ptr<SinkClass> s1(new SinkClass(t1));
            Ptr<SinkClass> s2(new SinkClass(t2));
            Ptr<SinkClass> s3(new SinkClass(t3));
            Ptr<SinkClass> s3_1(new SinkClass(t3));

            NSFX_TEST_EXPECT_EQ(sch->GetNumEvents(), 0);
            Ptr<nsfx::IEventHandle> h2 = sch->ScheduleAt(t2, s2);
            NSFX_TEST_ASSERT_EQ(sch->GetNumEvents(), 1);
            Ptr<nsfx::IEventHandle> h1 = sch->ScheduleAt(t1, s1);
            NSFX_TEST_ASSERT_EQ(sch->GetNumEvents(), 2);
            Ptr<nsfx::IEventHandle> h3 = sch->ScheduleAt(t3, s3);
            NSFX_TEST_ASSERT_EQ(sch->GetNumEvents(), 3);

            NSFX_TEST_EXPECT_EQ(sch->GetNextEvent()->GetTimePoint(), t1);
            clk = t1;
            sch->FireAndRemoveNextEvent();
            NSFX_TEST_ASSERT_EQ(sch->GetNumEvents(), 2);
            NSFX_TEST_EXPECT_EQ(clk, tp);

            NSFX_TEST_EXPECT_EQ(sch->GetNextEvent()->GetTimePoint(), t2);
            clk = t2;
            sch->FireAndRemoveNextEvent();
            NSFX_TEST_ASSERT_EQ(sch->GetNumEvents(), 1);
            NSFX_TEST_EXPECT_EQ(clk, tp);

            NSFX_TEST_EXPECT_EQ(sch->GetNextEvent()->GetTimePoint(), t3);
            clk = t3;
            sch->FireAndRemoveNextEvent();
            // Do not save the handle.
            /* Ptr<nsfx::IEventHandle> h3_1 = */ sch->ScheduleNow(s3_1);
            NSFX_TEST_ASSERT_EQ(sch->GetNumEvents(), 1);
            NSFX_TEST_EXPECT_EQ(clk, tp);

            NSFX_TEST_EXPECT_EQ(sch->GetNextEvent()->GetTimePoint(), t3);
            clk = t3;
            sch->FireAndRemoveNextEvent();
            NSFX_TEST_ASSERT_EQ(sch->GetNumEvents(), 0);
            NSFX_TEST_EXPECT_EQ(clk, tp);
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

    NSFX_TEST_CASE(Random)/*{{{*/
    {
        try
        {
            Ptr<nsfx::IScheduler> sch =
                nsfx::CreateObject<nsfx::IScheduler>(
                    "edu.uestc.nsfx.SetScheduler");
            {
                Ptr<nsfx::IClock> clock =
                    nsfx::CreateObject<nsfx::IClock>(
                        "edu.uestc.nsfx.test.Clock");
                Ptr<nsfx::IClockUser>(sch)->Use(clock);
            }
            clk = nsfx::TimePoint::Epoch();

            std::mt19937 g;
            std::uniform_real_distribution<double> u(0, 1000);
            // Schedule events.
            for (size_t i = 0; i < 500; ++i)
            {
                double d = u(g);
                nsfx::Duration dt(d, nsfx::round_downward);
                nsfx::ScheduleAt(sch, nsfx::TimePoint(dt), [] {});
            }
            NSFX_TEST_EXPECT_EQ(sch->GetNumEvents(), 500);
            // Fire events.
            for (size_t i = 0; i < 200; ++i)
            {
                sch->FireAndRemoveNextEvent();
            }
            NSFX_TEST_EXPECT_EQ(sch->GetNumEvents(), 300);
            // Schedule events.
            for (size_t i = 0; i < 500; ++i)
            {
                double d = u(g);
                nsfx::Duration dt(d, nsfx::round_downward);
                nsfx::ScheduleIn(sch, dt, [] {});
            }
            NSFX_TEST_EXPECT_EQ(sch->GetNumEvents(), 800);
            // Fire events.
            for (size_t i = 0; i < 800; ++i)
            {
                sch->FireAndRemoveNextEvent();
            }
            NSFX_TEST_EXPECT_EQ(sch->GetNumEvents(), 0);

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

