/**
 * @file
 *
 * @brief Test EventSink.
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
#include <nsfx/event/event-sink.h>
#include <iostream>


NSFX_TEST_SUITE(EventSink)
{
    NSFX_DEFINE_EVENT_SINK_INTERFACE(
        Iv0, "edu.uestc.nsfx.test.Iv0", ( void(void) ));
    NSFX_DEFINE_EVENT_SINK_INTERFACE(
        Iv1, "edu.uestc.nsfx.test.Iv1", ( void(int) ));
    NSFX_DEFINE_EVENT_SINK_INTERFACE(
        Iv2, "edu.uestc.nsfx.test.Iv2", ( void(int, double) ));
    NSFX_DEFINE_EVENT_SINK_INTERFACE(
        Iv3, "edu.uestc.nsfx.test.Iv3", ( void(int, double, std::string) ));

    NSFX_DEFINE_EVENT_SINK_INTERFACE(
        Ir0, "edu.uestc.nsfx.test.Ir0", ( float(void) ));
    NSFX_DEFINE_EVENT_SINK_INTERFACE(
        Ir1, "edu.uestc.nsfx.test.Ir1", ( float(int) ));
    NSFX_DEFINE_EVENT_SINK_INTERFACE(
        Ir2, "edu.uestc.nsfx.test.Ir2", ( float(int, double) ));
    NSFX_DEFINE_EVENT_SINK_INTERFACE(
        Ir3, "edu.uestc.nsfx.test.Ir3", ( float(int, double, std::string) ));

    NSFX_TEST_CASE(Concept)
    {
        BOOST_CONCEPT_ASSERT((nsfx::IEventSinkConcept<Iv0>));
        BOOST_CONCEPT_ASSERT((nsfx::IEventSinkConcept<Iv1>));
        BOOST_CONCEPT_ASSERT((nsfx::IEventSinkConcept<Iv2>));
        BOOST_CONCEPT_ASSERT((nsfx::IEventSinkConcept<Iv3>));
        BOOST_CONCEPT_ASSERT((nsfx::IEventSinkConcept<Ir0>));
        BOOST_CONCEPT_ASSERT((nsfx::IEventSinkConcept<Ir1>));
        BOOST_CONCEPT_ASSERT((nsfx::IEventSinkConcept<Ir2>));
        BOOST_CONCEPT_ASSERT((nsfx::IEventSinkConcept<Ir3>));
    }

    void v0(void) {}
    void v1(int) {}
    void v2(int, double) {}
    void v3(int, double, std::string) {}

    float r0(void) { return 1; }
    float r1(int) { return 2; }
    float r2(int, double) { return 3; }
    float r3(int, double, std::string) { return 4; }

    struct V0 { void v0(void) {} };
    struct V1 { void v1(int) {} };
    struct V2 { void v2(int, double) {} };
    struct V3 { void v3(int, double, std::string) {} };

    struct R0 { float r0(void) { return 5; } };
    struct R1 { float r1(int) { return 6; } };
    struct R2 { float r2(int, double) { return 7; } };
    struct R3 { float r3(int, double, std::string) { return 8; } };

    NSFX_TEST_CASE(Creator)
    {
        try
        {
            V0 cv0;
            V1 cv1;
            V2 cv2;
            V3 cv3;

            // l: lambda expr;
            // f: function pointer;
            // o: object and pointer to member function.
            nsfx::Ptr<Iv0> lv0 = nsfx::EventSinkCreator<Iv0>()(nullptr, [] { v0(); });
            nsfx::Ptr<Iv0> fv0 = nsfx::EventSinkCreator<Iv0>()(nullptr, &v0);
            nsfx::Ptr<Iv0> ov0 = nsfx::EventSinkCreator<Iv0>()(nullptr, &cv0, &V0::v0);
            lv0->Fire();
            fv0->Fire();
            ov0->Fire();

            nsfx::Ptr<Iv1> lv1 = nsfx::EventSinkCreator<Iv1>()(nullptr, [] (int i) { v1(i); });
            nsfx::Ptr<Iv1> fv1 = nsfx::EventSinkCreator<Iv1>()(nullptr, &v1);
            nsfx::Ptr<Iv1> ov1 = nsfx::EventSinkCreator<Iv1>()(nullptr, &cv1, &V1::v1);
            lv1->Fire(1);
            fv1->Fire(1);
            ov1->Fire(1);

            nsfx::Ptr<Iv2> lv2 = nsfx::EventSinkCreator<Iv2>()(nullptr, [] (int i, double d) { v2(i, d); });
            nsfx::Ptr<Iv2> fv2 = nsfx::EventSinkCreator<Iv2>()(nullptr, &v2);
            nsfx::Ptr<Iv2> ov2 = nsfx::EventSinkCreator<Iv2>()(nullptr, &cv2, &V2::v2);
            lv2->Fire(1, 2.0);
            fv2->Fire(1, 2.0);
            ov2->Fire(1, 2.0);

            nsfx::Ptr<Iv3> lv3 = nsfx::EventSinkCreator<Iv3>()(nullptr, [] (int i, double d, std::string s) { v3(i, d, s); });
            nsfx::Ptr<Iv3> fv3 = nsfx::EventSinkCreator<Iv3>()(nullptr, &v3);
            nsfx::Ptr<Iv3> ov3 = nsfx::EventSinkCreator<Iv3>()(nullptr, &cv3, &V3::v3);
            lv3->Fire(1, 2.0, "3");
            fv3->Fire(1, 2.0, "3");
            ov3->Fire(1, 2.0, "3");

            R0 cr0;
            R1 cr1;
            R2 cr2;
            R3 cr3;

            nsfx::Ptr<Ir0> lr0 = nsfx::EventSinkCreator<Ir0>()(nullptr, [] { return r0(); });
            nsfx::Ptr<Ir0> fr0 = nsfx::EventSinkCreator<Ir0>()(nullptr, &r0);
            nsfx::Ptr<Ir0> or0 = nsfx::EventSinkCreator<Ir0>()(nullptr, &cr0, &R0::r0);
            NSFX_TEST_EXPECT_EQ(lr0->Fire(), 1);
            NSFX_TEST_EXPECT_EQ(fr0->Fire(), 1);
            NSFX_TEST_EXPECT_EQ(or0->Fire(), 5);

            nsfx::Ptr<Ir1> lr1 = nsfx::EventSinkCreator<Ir1>()(nullptr, [] (int i) { return r1(i); });
            nsfx::Ptr<Ir1> fr1 = nsfx::EventSinkCreator<Ir1>()(nullptr, &r1);
            nsfx::Ptr<Ir1> or1 = nsfx::EventSinkCreator<Ir1>()(nullptr, &cr1, &R1::r1);
            NSFX_TEST_EXPECT_EQ(lr1->Fire(1), 2);
            NSFX_TEST_EXPECT_EQ(fr1->Fire(1), 2);
            NSFX_TEST_EXPECT_EQ(or1->Fire(1), 6);

            nsfx::Ptr<Ir2> lr2 = nsfx::EventSinkCreator<Ir2>()(nullptr, [] (int i, double d) { return r2(i, d); });
            nsfx::Ptr<Ir2> fr2 = nsfx::EventSinkCreator<Ir2>()(nullptr, &r2);
            nsfx::Ptr<Ir2> or2 = nsfx::EventSinkCreator<Ir2>()(nullptr, &cr2, &R2::r2);
            NSFX_TEST_EXPECT_EQ(lr2->Fire(1, 2.0), 3);
            NSFX_TEST_EXPECT_EQ(fr2->Fire(1, 2.0), 3);
            NSFX_TEST_EXPECT_EQ(or2->Fire(1, 2.0), 7);

            nsfx::Ptr<Ir3> lr3 = nsfx::EventSinkCreator<Ir3>()(nullptr, [] (int i, double d, std::string s) { return r3(i, d, s); });
            nsfx::Ptr<Ir3> fr3 = nsfx::EventSinkCreator<Ir3>()(nullptr, &r3);
            nsfx::Ptr<Ir3> or3 = nsfx::EventSinkCreator<Ir3>()(nullptr, &cr3, &R3::r3);
            NSFX_TEST_EXPECT_EQ(lr3->Fire(1, 2.0, "3"), 4);
            NSFX_TEST_EXPECT_EQ(fr3->Fire(1, 2.0, "3"), 4);
            NSFX_TEST_EXPECT_EQ(or3->Fire(1, 2.0, "3"), 8);

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

    NSFX_TEST_CASE(Create)
    {
        try
        {
            V0 cv0;
            V1 cv1;
            V2 cv2;
            V3 cv3;

            // l: lambda expr;
            // f: function pointer;
            // o: object and pointer to member function.
            nsfx::Ptr<Iv0> lv0 = nsfx::CreateEventSink<Iv0>(nullptr, [] { v0(); });
            nsfx::Ptr<Iv0> fv0 = nsfx::CreateEventSink<Iv0>(nullptr, &v0);
            nsfx::Ptr<Iv0> ov0 = nsfx::CreateEventSink<Iv0>(nullptr, &cv0, &V0::v0);
            lv0->Fire();
            fv0->Fire();
            ov0->Fire();

            nsfx::Ptr<Iv1> lv1 = nsfx::CreateEventSink<Iv1>(nullptr, [] (int i) { v1(i); });
            nsfx::Ptr<Iv1> fv1 = nsfx::CreateEventSink<Iv1>(nullptr, &v1);
            nsfx::Ptr<Iv1> ov1 = nsfx::CreateEventSink<Iv1>(nullptr, &cv1, &V1::v1);
            lv1->Fire(1);
            fv1->Fire(1);
            ov1->Fire(1);

            nsfx::Ptr<Iv2> lv2 = nsfx::CreateEventSink<Iv2>(nullptr, [] (int i, double d) { v2(i, d); });
            nsfx::Ptr<Iv2> fv2 = nsfx::CreateEventSink<Iv2>(nullptr, &v2);
            nsfx::Ptr<Iv2> ov2 = nsfx::CreateEventSink<Iv2>(nullptr, &cv2, &V2::v2);
            lv2->Fire(1, 2.0);
            fv2->Fire(1, 2.0);
            ov2->Fire(1, 2.0);

            nsfx::Ptr<Iv3> lv3 = nsfx::CreateEventSink<Iv3>(nullptr, [] (int i, double d, std::string s) { v3(i, d, s); });
            nsfx::Ptr<Iv3> fv3 = nsfx::CreateEventSink<Iv3>(nullptr, &v3);
            nsfx::Ptr<Iv3> ov3 = nsfx::CreateEventSink<Iv3>(nullptr, &cv3, &V3::v3);
            lv3->Fire(1, 2.0, "3");
            fv3->Fire(1, 2.0, "3");
            ov3->Fire(1, 2.0, "3");

            R0 cr0;
            R1 cr1;
            R2 cr2;
            R3 cr3;

            nsfx::Ptr<Ir0> lr0 = nsfx::CreateEventSink<Ir0>(nullptr, [] { return r0(); });
            nsfx::Ptr<Ir0> fr0 = nsfx::CreateEventSink<Ir0>(nullptr, &r0);
            nsfx::Ptr<Ir0> or0 = nsfx::CreateEventSink<Ir0>(nullptr, &cr0, &R0::r0);
            NSFX_TEST_EXPECT_EQ(lr0->Fire(), 1);
            NSFX_TEST_EXPECT_EQ(fr0->Fire(), 1);
            NSFX_TEST_EXPECT_EQ(or0->Fire(), 5);

            nsfx::Ptr<Ir1> lr1 = nsfx::CreateEventSink<Ir1>(nullptr, [] (int i) { return r1(i); });
            nsfx::Ptr<Ir1> fr1 = nsfx::CreateEventSink<Ir1>(nullptr, &r1);
            nsfx::Ptr<Ir1> or1 = nsfx::CreateEventSink<Ir1>(nullptr, &cr1, &R1::r1);
            NSFX_TEST_EXPECT_EQ(lr1->Fire(1), 2);
            NSFX_TEST_EXPECT_EQ(fr1->Fire(1), 2);
            NSFX_TEST_EXPECT_EQ(or1->Fire(1), 6);

            nsfx::Ptr<Ir2> lr2 = nsfx::CreateEventSink<Ir2>(nullptr, [] (int i, double d) { return r2(i, d); });
            nsfx::Ptr<Ir2> fr2 = nsfx::CreateEventSink<Ir2>(nullptr, &r2);
            nsfx::Ptr<Ir2> or2 = nsfx::CreateEventSink<Ir2>(nullptr, &cr2, &R2::r2);
            NSFX_TEST_EXPECT_EQ(lr2->Fire(1, 2.0), 3);
            NSFX_TEST_EXPECT_EQ(fr2->Fire(1, 2.0), 3);
            NSFX_TEST_EXPECT_EQ(or2->Fire(1, 2.0), 7);

            nsfx::Ptr<Ir3> lr3 = nsfx::CreateEventSink<Ir3>(nullptr, [] (int i, double d, std::string s) { return r3(i, d, s); });
            nsfx::Ptr<Ir3> fr3 = nsfx::CreateEventSink<Ir3>(nullptr, &r3);
            nsfx::Ptr<Ir3> or3 = nsfx::CreateEventSink<Ir3>(nullptr, &cr3, &R3::r3);
            NSFX_TEST_EXPECT_EQ(lr3->Fire(1, 2.0, "3"), 4);
            NSFX_TEST_EXPECT_EQ(fr3->Fire(1, 2.0, "3"), 4);
            NSFX_TEST_EXPECT_EQ(or3->Fire(1, 2.0, "3"), 8);

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
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

