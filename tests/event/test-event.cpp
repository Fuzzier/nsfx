/**
 * @file
 *
 * @brief Test Event.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-01-14
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/event/event.h>
#include <nsfx/event/event-sink.h>
#include <iostream>


NSFX_TEST_SUITE(Event)
{
    // IEventSink
    NSFX_DEFINE_EVENT_SINK_INTERFACE(
        Iv0, "edu.uestc.nsfx.test.Iv0", ( void(void) ));

    NSFX_DEFINE_EVENT_SINK_INTERFACE(
        Iv1, "edu.uestc.nsfx.test.Iv1", ( void(int) ));

    NSFX_DEFINE_EVENT_SINK_INTERFACE(
        Iv2, "edu.uestc.nsfx.test.Iv2", ( void(int, double) ));

    // IEvent
    NSFX_DEFINE_EVENT_INTERFACE(
        Ev0, "edu.uestc.nsfx.test.Ev0", Iv0);

    NSFX_DEFINE_EVENT_INTERFACE(
        Ev1, "edu.uestc.nsfx.test.Ev1", Iv1);

    NSFX_DEFINE_EVENT_INTERFACE(
        Ev2, "edu.uestc.nsfx.test.Ev2", Iv2);

    static int x;

    // Tools
    void V0(void) { ++x; }
    void V1(int i) { x += i; }
    void V2(int i, double d) { x += i; }

    struct Test : virtual public nsfx::IObject
    {
        Test(void) :
            v0_(this),
            v1_(this),
            v2_(this)
        {}

        virtual ~Test(void) {}

        void VisitV0(void)
        {
            v0_.GetImpl()->Visit([] (Iv0* sink) { sink->Fire(); });
        }

        void FireV0(void)
        {
            v0_.GetImpl()->Fire();
        }

        void VisitV1(int i)
        {
            v1_.GetImpl()->Visit([&] (Iv1* sink) { sink->Fire(i); });
        }

        void FireV1(int i)
        {
            v1_.GetImpl()->Fire(i);
        }

        void VisitV2(int i, double d)
        {
            v2_.GetImpl()->Visit([&] (Iv2* sink) { sink->Fire(i, d); });
        }

        void FireV2(int i, double d)
        {
            v2_.GetImpl()->Fire(i, d);
        }

    private:
        NSFX_INTERFACE_MAP_BEGIN(Test)
            NSFX_INTERFACE_AGGREGATED_ENTRY(Ev0, &v0_)
            NSFX_INTERFACE_AGGREGATED_ENTRY(Ev1, &v1_)
            NSFX_INTERFACE_AGGREGATED_ENTRY(Ev2, &v2_)
        NSFX_INTERFACE_MAP_END()

    public:
        nsfx::MemberAggObject<nsfx::Event<Ev0> >  v0_;
        nsfx::MemberAggObject<nsfx::Event<Ev1> >  v1_;
        nsfx::MemberAggObject<nsfx::Event<Ev2, 1> >  v2_;
    };

    NSFX_TEST_CASE(Connect)
    {
        try
        {
            nsfx::Ptr<Iv0> sv0 = nsfx::EventSinkCreator<Iv0>()(nullptr, &V0);

            nsfx::Ptr<Iv1> sv1 = nsfx::EventSinkCreator<Iv1>()(nullptr, &V1);

            nsfx::Ptr<Iv2> sv2 = nsfx::EventSinkCreator<Iv2>()(nullptr, &V2);

            nsfx::Ptr<Test> test = new nsfx::Object<Test>();

            ////////////////////////////////////////
            // V0
            nsfx::cookie_t cv0[10];
            for (size_t i = 0; i < 10; ++i)
            {
                cv0[i] = nsfx::Ptr<Ev0>(test)->Connect(sv0);
            }

            for (size_t i = 0; i < 10; ++i)
            {
                NSFX_TEST_EXPECT_EQ(test->v0_.GetImpl()->GetNumSinks(), 10-i);
                x = 0;
                test->VisitV0();
                NSFX_TEST_EXPECT_EQ(x, 10 - i);
                nsfx::Ptr<Ev0>(test)->Disconnect(cv0[i]);
            }

            ////////////////////////////////////////
            // V1
            nsfx::cookie_t cv1[10];
            for (size_t i = 0; i < 10; ++i)
            {
                cv1[i] = nsfx::Ptr<Ev1>(test)->Connect(sv1);
            }

            for (size_t i = 0; i < 5; ++i)
            {
                nsfx::Ptr<Ev1>(test)->Disconnect(cv1[i*2]);
            }

            for (size_t i = 0; i < 5; ++i)
            {
                cv1[i*2] = nsfx::Ptr<Ev1>(test)->Connect(sv1);
            }

            for (size_t i = 0; i < 10; ++i)
            {
                NSFX_TEST_EXPECT_EQ(test->v1_.GetImpl()->GetNumSinks(), 10-i);
                x = 0;
                test->VisitV1(2);
                NSFX_TEST_EXPECT_EQ(x, (10-i)*2);
                nsfx::Ptr<Ev1>(test)->Disconnect(cv1[i]);
            }

            ////////////////////////////////////////
            // V2
            nsfx::cookie_t cv2;
            cv2 = nsfx::Ptr<Ev2>(test)->Connect(sv2);

            try
            {
                nsfx::Ptr<Ev2>(test)->Connect(sv2);
                NSFX_TEST_EXPECT(false);
            }
            catch (nsfx::ConnectionLimit& )
            {
                // Should come here.
            }

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

