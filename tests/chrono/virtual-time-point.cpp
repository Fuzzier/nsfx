#include <nsfx/test.h>
#include <nsfx/chrono/virtual-time-point.h>
#include <iostream>


NSFX_TEST_SUITE(VirtualTimePoint)
{
    typedef nsfx::chrono::VirtualTimePoint TimePoint;
    typedef nsfx::chrono::Duration         Duration;

    NSFX_TEST_CASE(Ctor0)
    {
        TimePoint t;
        NSFX_TEST_EXPECT_EQ(t.GetDuration().GetNanoSeconds(), 0);
    }

    NSFX_TEST_CASE(Ctor1)
    {
        Duration  dt(100);
        TimePoint t(dt);
        NSFX_TEST_EXPECT_EQ(t.GetDuration(), dt);
    }

    NSFX_TEST_CASE(Comparisons)
    {
        TimePoint t1(Duration(10));
        TimePoint t2(Duration(20));

        NSFX_TEST_EXPECT(t1 == t1);
        NSFX_TEST_EXPECT(!(t1 != t1));
        NSFX_TEST_EXPECT(t1 <= t1);
        NSFX_TEST_EXPECT(!(t1 < t1));
        NSFX_TEST_EXPECT(t1 >= t1);
        NSFX_TEST_EXPECT(!(t1 > t1));

        NSFX_TEST_EXPECT(t1 < t2);
        NSFX_TEST_EXPECT(t1 <= t2);
        NSFX_TEST_EXPECT(!(t1 > t2));
        NSFX_TEST_EXPECT(!(t1 >= t2));
        NSFX_TEST_EXPECT(t1 != t2);
        NSFX_TEST_EXPECT(!(t1 == t2));
        NSFX_TEST_EXPECT(t2 > t1);
        NSFX_TEST_EXPECT(t2 >= t1);
        NSFX_TEST_EXPECT(!(t2 < t1));
        NSFX_TEST_EXPECT(!(t2 <= t1));
    }

    NSFX_TEST_CASE(Algorithms)
    {
        TimePoint t1(Duration(10));
        Duration  td(20);

        TimePoint t2 = t1 + td;
        NSFX_TEST_EXPECT_EQ(t2, TimePoint(Duration(30)));

        NSFX_TEST_EXPECT_EQ(t2, td + t1);

        NSFX_TEST_EXPECT_EQ(t2 - td, t1);

        NSFX_TEST_EXPECT_EQ(t2 - t1, td);

        t2 -= td;
        NSFX_TEST_EXPECT_EQ(t2, t1);

        t2 += td;
        NSFX_TEST_EXPECT_EQ(t2, t1 + td);
    }

    NSFX_TEST_CASE(Hash)
    {
        TimePoint t(Duration(10));
        size_t h0 = hash_value(t);
        size_t h1 = boost::hash<TimePoint>()(t);
        NSFX_TEST_EXPECT_EQ(h0, h1);
    }

}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

