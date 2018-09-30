/**
 * @file
 *
 * @brief Test ListEventScheduler.
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
#include <nsfx/simulation/list-event-scheduler.h>
#include <iostream>


NSFX_TEST_SUITE(ListEventScheduler)
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
    NSFX_REGISTER_CLASS(Clock, "edu.uestc.nsfx.test.Clock");

    NSFX_TEST_CASE(ExternalDriven)/*{{{*/
    {
        try
        {
            Ptr<nsfx::IEventScheduler> sch =
                nsfx::CreateObject<nsfx::IEventScheduler>(
                    "edu.uestc.nsfx.ListEventScheduler");
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
            NSFX_TEST_EXPECT_EQ(clk, t1);

            NSFX_TEST_EXPECT_EQ(sch->GetNextEvent()->GetTimePoint(), t2);
            clk = t2;
            sch->FireAndRemoveNextEvent();
            NSFX_TEST_ASSERT_EQ(sch->GetNumEvents(), 1);
            NSFX_TEST_EXPECT_EQ(clk, t2);

            NSFX_TEST_EXPECT_EQ(sch->GetNextEvent()->GetTimePoint(), t3);
            clk = t3;
            sch->FireAndRemoveNextEvent();
            Ptr<nsfx::IEventHandle> h3_1 = sch->ScheduleNow(s3_1);
            NSFX_TEST_ASSERT_EQ(sch->GetNumEvents(), 1);
            NSFX_TEST_EXPECT_EQ(clk, t3);

            NSFX_TEST_EXPECT_EQ(sch->GetNextEvent()->GetTimePoint(), t3);
            clk = t3;
            sch->FireAndRemoveNextEvent();
            NSFX_TEST_ASSERT_EQ(sch->GetNumEvents(), 0);
            NSFX_TEST_EXPECT_EQ(clk, t3);

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
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

