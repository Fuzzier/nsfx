#include <nsfx/test.h>
#include <nsfx/component/event-sink-creator.h>
#include <iostream>


NSFX_TEST_SUITE(Ptr)
{
    using nsfx::refcount_t;

    NSFX_DEFINE_EVENT_SINK_INTERFACE(Iv0, ( void(void) ));
    NSFX_DEFINE_CLASS_UUID4(Iv0, 0x67CB03B4, 0x5B1C, 0x4401, 0xB418C6758383839ELL);
    NSFX_DEFINE_EVENT_SINK_INTERFACE(Iv1, ( void(int) ));
    NSFX_DEFINE_CLASS_UUID4(Iv1, 0xC2B34B86, 0xA3F2, 0x45FD, 0x9654CD79A928B1A6LL);
    NSFX_DEFINE_EVENT_SINK_INTERFACE(Iv2, ( void(int, double) ));
    NSFX_DEFINE_CLASS_UUID4(Iv2, 0x1F02B3BA, 0x7567, 0x423B, 0xBDD995AE1F87D30ALL);
    NSFX_DEFINE_EVENT_SINK_INTERFACE(Iv3, ( void(int, double, std::string) ));
    NSFX_DEFINE_CLASS_UUID4(Iv3, 0x5193AFBA, 0x12C5, 0x49ED, 0xAF9C11D00334EC44LL);

    NSFX_DEFINE_EVENT_SINK_INTERFACE(Ir0, ( float(void) ));
    NSFX_DEFINE_CLASS_UUID4(Ir0, 0x5E008399, 0x8D24, 0x4229, 0x84135C57A065AE32LL);
    NSFX_DEFINE_EVENT_SINK_INTERFACE(Ir1, ( float(int) ));
    NSFX_DEFINE_CLASS_UUID4(Ir1, 0xA9E86104, 0x16A0, 0x4CA4, 0xA4B7380190520F5FLL);
    NSFX_DEFINE_EVENT_SINK_INTERFACE(Ir2, ( float(int, double) ));
    NSFX_DEFINE_CLASS_UUID4(Ir2, 0x1D34DD48, 0x566D, 0x4645, 0x9198BB055E281AF9LL);
    NSFX_DEFINE_EVENT_SINK_INTERFACE(Ir3, ( float(int, double, std::string) ));
    NSFX_DEFINE_CLASS_UUID4(Ir3, 0x27951022, 0x2516, 0x43F0, 0xAC57106233362CB0LL);

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
            NSFX_TEST_EXPECT(false) << diagnostic_information(e);
        }
        catch (std::exception& e)
        {
            NSFX_TEST_EXPECT(false) << e.what();
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

