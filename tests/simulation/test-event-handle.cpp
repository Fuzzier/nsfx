/**
 * @file
 *
 * @brief Test EventHandle.
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
#include <nsfx/simulation/event-handle.h>
#include <iostream>


NSFX_TEST_SUITE(EventHandle)
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
    typedef nsfx::Object<nsfx::EventHandle>  EventHandleClass;

    NSFX_TEST_CASE(Comparison)/*{{{*/
    {
        try
        {
            nsfx::TimePoint t1(nsfx::Duration(1));
            nsfx::TimePoint t2(nsfx::Duration(2));
            Ptr<SinkClass> s1(new SinkClass(t1));

            Ptr<EventHandleClass>  h1(new EventHandleClass(t1, s1));
            Ptr<EventHandleClass>  h2(new EventHandleClass(t2, s1));
            Ptr<EventHandleClass>  h2_1(new EventHandleClass(t2, s1));

            NSFX_TEST_EXPECT_EQ(h1->GetId(), 0);
            NSFX_TEST_EXPECT_EQ(h2->GetId(), 1);
            NSFX_TEST_EXPECT_EQ(h2_1->GetId(), 2);

            NSFX_TEST_EXPECT(h1 == h1);
            NSFX_TEST_EXPECT(h1 <= h1);
            NSFX_TEST_EXPECT(h1 >= h1);
            NSFX_TEST_EXPECT(!(h1 != h1));
            NSFX_TEST_EXPECT(!(h1 < h1));
            NSFX_TEST_EXPECT(!(h1 > h1));

            NSFX_TEST_EXPECT(h1 != h2);
            NSFX_TEST_EXPECT(h2 != h1);
            NSFX_TEST_EXPECT(!(h1 == h2));
            NSFX_TEST_EXPECT(!(h2 == h1));
            NSFX_TEST_EXPECT(h1 < h2);
            NSFX_TEST_EXPECT(h2 > h1);
            NSFX_TEST_EXPECT(!(h1 > h2));
            NSFX_TEST_EXPECT(!(h2 < h1));
            NSFX_TEST_EXPECT(h1 <= h2);
            NSFX_TEST_EXPECT(h2 >= h1);
            NSFX_TEST_EXPECT(!(h1 >= h2));
            NSFX_TEST_EXPECT(!(h2 <= h1));

            NSFX_TEST_EXPECT(h2 != h2_1);
            NSFX_TEST_EXPECT(h2_1 != h2);
            NSFX_TEST_EXPECT(!(h2 == h2_1));
            NSFX_TEST_EXPECT(!(h2_1 == h2));
            NSFX_TEST_EXPECT(h2 < h2_1);
            NSFX_TEST_EXPECT(h2_1 > h2);
            NSFX_TEST_EXPECT(!(h2 > h2_1));
            NSFX_TEST_EXPECT(!(h2_1 < h2));
            NSFX_TEST_EXPECT(h2 <= h2_1);
            NSFX_TEST_EXPECT(h2_1 >= h2);
            NSFX_TEST_EXPECT(!(h2 >= h2_1));
            NSFX_TEST_EXPECT(!(h2_1 <= h2));

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

