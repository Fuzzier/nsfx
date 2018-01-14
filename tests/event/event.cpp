#include <nsfx/test.h>
#include <nsfx/event/event.h>
#include <nsfx/event/event-sink.h>
#include <iostream>


NSFX_TEST_SUITE(Event)
{
    // IEventSink
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

    // IEvent
    NSFX_DEFINE_EVENT_INTERFACE(Ev0, Iv0);
    NSFX_DEFINE_EVENT_INTERFACE(Ev1, Iv1);
    NSFX_DEFINE_EVENT_INTERFACE(Ev2, Iv2);
    NSFX_DEFINE_EVENT_INTERFACE(Ev3, Iv3);

    NSFX_DEFINE_EVENT_INTERFACE(Er0, Ir0);
    NSFX_DEFINE_EVENT_INTERFACE(Er1, Ir1);
    NSFX_DEFINE_EVENT_INTERFACE(Er2, Ir2);
    NSFX_DEFINE_EVENT_INTERFACE(Er3, Ir3);

    NSFX_DEFINE_CLASS_UUID4(Ev0, 0x7F1AE701, 0xAA48, 0x469D, 0x922576E4CB76F385LL);
    NSFX_DEFINE_CLASS_UUID4(Ev1, 0x21416D80, 0xF7B4, 0x4302, 0x8E8EC3DE38DED5B9LL);
    NSFX_DEFINE_CLASS_UUID4(Ev2, 0x8C4B03E4, 0x8137, 0x4DC1, 0x9AEC76EDABFE09BCLL);
    NSFX_DEFINE_CLASS_UUID4(Ev3, 0x2C4F3609, 0x6C25, 0x4B48, 0x8D559A1013F9380CLL);

    NSFX_DEFINE_CLASS_UUID4(Er0, 0xB9255B19, 0x5C91, 0x47A0, 0xBA0A8D9B920504F8LL);
    NSFX_DEFINE_CLASS_UUID4(Er1, 0x7E4FD966, 0x57E7, 0x4109, 0xB06889D28DA8B5EDLL);
    NSFX_DEFINE_CLASS_UUID4(Er2, 0x30B62CAA, 0x5BBF, 0x413C, 0xA7E3FE5CFCFF5A13LL);
    NSFX_DEFINE_CLASS_UUID4(Er3, 0x1E5947E6, 0x8B0B, 0x43A3, 0xA210DD62A76017E5LL);

    // Tools
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

    struct ITest : virtual nsfx::IObject
    {
        virtual ~ITest(void) {}
        virtual void V0(void) = 0;
        virtual void V1(int) = 0;
        virtual void V2(int, double) = 0;
        virtual void V3(int, double, std::string) = 0;
        virtual float R0(void) = 0;
        virtual float R1(int) = 0;
        virtual float R2(int, double) = 0;
        virtual float R3(int, double, std::string) = 0;
    };
    NSFX_DEFINE_CLASS_UUID4(ITest, 0x87738A28, 0xE040, 0x408D, 0xA2CBC09772AA6D6DLL);

    struct Test : ITest, Ev0, Ev1, Ev2, Ev3, Er0, Er1, Er2, Er3
    {
#pragma warning(push)
#pragma warning(disable : 4355)
        Test(void) :
            v0_(this),
            v1_(this),
            v2_(this),
            v3_(this),
            r0_(this),
            r1_(this),
            r2_(this),
            r3_(this)
#pragma warning(pop)
        {}

        virtual ~Test(void) {}

        virtual void V0(void) NSFX_OVERRIDE
        {
            v0_.GetEnveloped()->Visit([&] (Iv0* sink) { sink->Fire(); });
        }

        virtual void V1(int i) NSFX_OVERRIDE
        {
            v1_.GetEnveloped()->Visit([&] (Iv1* sink) { sink->Fire(i); });
        }

        virtual void V2(int i, double d) NSFX_OVERRIDE
        {
            v2_.GetEnveloped()->Visit([&] (Iv2* sink) { sink->Fire(i, d); });
        }

        virtual void V3(int i, double d, std::string s) NSFX_OVERRIDE
        {
            v3_.GetEnveloped()->Visit([&] (Iv3* sink) { sink->Fire(i, d, s); });
        }

        virtual float R0(void) NSFX_OVERRIDE
        {
            r0_.GetEnveloped()->Visit([&] (Ir0* sink) { return sink->Fire(); });
        }

        virtual float R1(int i) NSFX_OVERRIDE
        {
            r1_.GetEnveloped()->Visit([&] (Ir1* sink) { return sink->Fire(i); });
        }

        virtual float R2(int i, double d) NSFX_OVERRIDE
        {
            r2_.GetEnveloped()->Visit([&] (Ir2* sink) { return sink->Fire(i, d); });
        }

        virtual float R3(int i, double d, std::string s) NSFX_OVERRIDE
        {
            r3_.GetEnveloped()->Visit([&] (Ir3* sink) { return sink->Fire(i, d, s); });
        }

        NSFX_INTERFACE_MAP_BEGIN(Test)
            NSFX_INTERFACE_ENTRY(ITest)
            NSFX_INTERFACE_AGGREGATED_ENTRY(Ev0, &v0_)
            NSFX_INTERFACE_AGGREGATED_ENTRY(Ev1, &v1_)
            NSFX_INTERFACE_AGGREGATED_ENTRY(Ev2, &v2_)
            NSFX_INTERFACE_AGGREGATED_ENTRY(Ev3, &v3_)
            NSFX_INTERFACE_AGGREGATED_ENTRY(Er0, &r0_)
            NSFX_INTERFACE_AGGREGATED_ENTRY(Er1, &r1_)
            NSFX_INTERFACE_AGGREGATED_ENTRY(Er2, &r2_)
            NSFX_INTERFACE_AGGREGATED_ENTRY(Er3, &r3_)
        NSFX_INTERFACE_MAP_END()

        nsfx::AggObject<nsfx::Event<Ev0>, false> v0_;
        nsfx::AggObject<nsfx::Event<Ev1>, false> v1_;
        nsfx::AggObject<nsfx::Event<Ev2>, false> v2_;
        nsfx::AggObject<nsfx::Event<Ev3>, false> v3_;
        nsfx::AggObject<nsfx::Event<Er0>, false> r0_;
        nsfx::AggObject<nsfx::Event<Er1>, false> r1_;
        nsfx::AggObject<nsfx::Event<Er2>, false> r2_;
        nsfx::AggObject<nsfx::Event<Er3>, false> r3_;
    };
    NSFX_DEFINE_CLASS_UUID4(Test, 0x37207699, 0x8512, 0x4846, 0xB30079F3E7ECAD15LL);

    NSFX_TEST_CASE(Event)
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

