#include <nsfx/test.h>
#include <nsfx/simulator/list-event-scheduler.h>
#include <iostream>


NSFX_TEST_SUITE(EventScheduler)
{
    using nsfx::Ptr;

    static nsfx::TimePoint tp;
    struct Sink : nsfx::IEventSink/*{{{*/
    {
        Sink() {}
        Sink(nsfx::TimePoint tp) : tp_(tp) {}

        virtual ~Sink(void) {}

        virtual void OnEvent(void) NSFX_OVERRIDE
        {
            tp = tp_;
        }

        NSFX_INTERFACE_MAP_BEGIN(Sink)
            NSFX_INTERFACE_ENTRY(nsfx::IEventSink)
        NSFX_INTERFACE_MAP_END()

    private:
        nsfx::TimePoint tp_;
    };/*}}}*/
    typedef nsfx::Object<Sink>  SinkType;
    NSFX_DEFINE_CLASS_UUID4(Sink, 0x684CC68A, 0x03B2, 0x4618, 0xA15A279D015DE048LL);

    static nsfx::TimePoint clk;
    struct Clock : nsfx::IClock/*{{{*/
    {
        virtual ~Clock() {}

        virtual nsfx::TimePoint Now() NSFX_OVERRIDE
        {
            return clk;
        }

        NSFX_INTERFACE_MAP_BEGIN(Clock)
            NSFX_INTERFACE_ENTRY(IClock)
        NSFX_INTERFACE_MAP_END()

    };/*}}}*/
    typedef nsfx::Object<Clock>  ClockType;
    NSFX_DEFINE_CLASS_UUID4(Clock, 0x705B763E, 0x9C77, 0x4C26, 0x9B9EAD27263A2A05LL);
    NSFX_REGISTER_CLASS(Clock);

    NSFX_TEST_CASE(ExternalDriven)/*{{{*/
    {
        try
        {
            Ptr<nsfx::IEventScheduler> sch =
                nsfx::CreateObject<nsfx::IEventScheduler>(
                    NSFX_CID_ListEventScheduler);
            {
                Ptr<nsfx::IClock> clock =
                    nsfx::CreateObject<nsfx::IClock>(nsfx::uuid_of<Clock>());
                Ptr<nsfx::IClockUser> u(sch);
                NSFX_TEST_ASSERT(u);
                u->UseClock(clock);
            }
            nsfx::TimePoint t1(nsfx::Duration(1));
            nsfx::TimePoint t2(nsfx::Duration(2));
            nsfx::TimePoint t3(nsfx::Duration(3));
            Ptr<SinkType> s1(new SinkType(t1));
            Ptr<SinkType> s2(new SinkType(t2));
            Ptr<SinkType> s3(new SinkType(t3));
            Ptr<SinkType> s3_1(new SinkType(t3));

            NSFX_TEST_EXPECT_EQ(sch->GetNumEvents(), 0);
            Ptr<nsfx::IEventHandle> h2 = sch->ScheduleAt(t2, s2);
            NSFX_TEST_ASSERT_EQ(sch->GetNumEvents(), 1);
            Ptr<nsfx::IEventHandle> h1 = sch->ScheduleAt(t1, s1);
            NSFX_TEST_ASSERT_EQ(sch->GetNumEvents(), 2);
            Ptr<nsfx::IEventHandle> h3 = sch->ScheduleAt(t3, s3);
            NSFX_TEST_ASSERT_EQ(sch->GetNumEvents(), 3);

            NSFX_TEST_EXPECT_EQ(sch->GetNextEvent()->GetTimePoint(), t1);
            clk = t1;
            h1->Fire();
            NSFX_TEST_ASSERT_EQ(sch->GetNumEvents(), 2);
            NSFX_TEST_EXPECT_EQ(clk, t1);

            NSFX_TEST_EXPECT_EQ(sch->GetNextEvent()->GetTimePoint(), t2);
            clk = t2;
            h2->Fire();
            NSFX_TEST_ASSERT_EQ(sch->GetNumEvents(), 1);
            NSFX_TEST_EXPECT_EQ(clk, t2);

            NSFX_TEST_EXPECT_EQ(sch->GetNextEvent()->GetTimePoint(), t3);
            clk = t3;
            Ptr<nsfx::IEventHandle> h3_1 = sch->ScheduleNow(s3_1);
            h3->Fire();
            NSFX_TEST_ASSERT_EQ(sch->GetNumEvents(), 1);
            NSFX_TEST_EXPECT_EQ(clk, t3);

            NSFX_TEST_EXPECT_EQ(sch->GetNextEvent()->GetTimePoint(), t3);
            clk = t3;
            h3_1->Fire();
            NSFX_TEST_ASSERT_EQ(sch->GetNumEvents(), 0);
            NSFX_TEST_EXPECT_EQ(clk, t3);

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

