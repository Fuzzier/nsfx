/**
 * @file
 *
 * @brief Test VirtualTimePoint.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-13
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */


#include <nsfx/test.h>
#include <nsfx/chrono/virtual-time-point.h>
#include <iostream>


NSFX_TEST_SUITE(VirtualTimePoint)
{
    typedef nsfx::chrono::VirtualTimePoint  TimePoint;
    typedef TimePoint::Duration             Duration;

    NSFX_TEST_CASE(Ctor0)
    {
        TimePoint t;
        NSFX_TEST_EXPECT_EQ(t.GetDuration().GetCount(), 0);
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

    NSFX_TEST_CASE(Swap)
    {
        TimePoint t1(Duration(1234));
        TimePoint t2(Duration(5678));
        swap(t1, t2);
        NSFX_TEST_EXPECT_EQ(t1, TimePoint(Duration(5678)));
        NSFX_TEST_EXPECT_EQ(t2, TimePoint(Duration(1234)));
    }

    NSFX_TEST_CASE(Limits)
    {
        TimePoint tmin = TimePoint::Min();
        NSFX_TEST_EXPECT_EQ(tmin.GetDuration(), Duration::Min());

        TimePoint tmax = TimePoint::Max();
        NSFX_TEST_EXPECT_EQ(tmax.GetDuration(), Duration::Max());
    }

}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

