/**
 * @file
 *
 * @brief Test Duration.
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
#include <nsfx/chrono/duration.h>
#include <iostream>


NSFX_TEST_SUITE(Duration)
{
    typedef nsfx::chrono::Duration<nsfx::nano> Duration;

    NSFX_TEST_CASE(Constructor)
    {
        Duration dt;
        NSFX_TEST_EXPECT_EQ(dt.GetCount(), 0);
        NSFX_TEST_EXPECT(!dt);
    }

    NSFX_TEST_CASE(NanoSeconds)
    {
        auto dt = nsfx::chrono::NanoSeconds(1);
        NSFX_TEST_EXPECT_EQ(dt.GetCount(), 1);
        NSFX_TEST_EXPECT_EQ(dt.GetResolution(), 1e-9);
        Duration dt2 = dt;
        NSFX_TEST_EXPECT_EQ(dt2.GetCount(), 1);
        NSFX_TEST_EXPECT(dt);
    }

    NSFX_TEST_CASE(MicroSeconds)
    {
        auto dt = nsfx::chrono::MicroSeconds(1);
        NSFX_TEST_EXPECT_EQ(dt.GetCount(), 1);
        NSFX_TEST_EXPECT_EQ(dt.GetResolution(), 1e-6);
        Duration dt2 = dt;
        NSFX_TEST_EXPECT_EQ(dt2.GetCount(), 1000);
    }

    NSFX_TEST_CASE(MilliSeconds)
    {
        auto dt = nsfx::chrono::MilliSeconds(1);
        NSFX_TEST_EXPECT_EQ(dt.GetCount(), 1);
        NSFX_TEST_EXPECT_EQ(dt.GetResolution(), 1e-3);
        Duration dt2 = dt;
        NSFX_TEST_EXPECT_EQ(dt2.GetCount(), 1000000);
    }

    NSFX_TEST_CASE(Seconds)
    {
        auto dt = nsfx::chrono::Seconds(1);
        NSFX_TEST_EXPECT_EQ(dt.GetCount(), 1);
        NSFX_TEST_EXPECT_EQ(dt.GetResolution(), 1);
        Duration dt2 = dt;
        NSFX_TEST_EXPECT_EQ(dt2.GetCount(), 1000000000);
    }

    NSFX_TEST_CASE(Minutes)
    {
        auto dt = nsfx::chrono::Minutes(1);
        NSFX_TEST_EXPECT_EQ(dt.GetCount(), 60);
        NSFX_TEST_EXPECT_EQ(dt.GetResolution(), 1);
        Duration dt2 = dt;
        NSFX_TEST_EXPECT_EQ(dt2.GetCount(), 60000000000LL);
    }

    NSFX_TEST_CASE(Hours)
    {
        auto dt = nsfx::chrono::Hours(1);
        NSFX_TEST_EXPECT_EQ(dt.GetCount(), 3600);
        NSFX_TEST_EXPECT_EQ(dt.GetResolution(), 1);
        Duration dt2 = dt;
        NSFX_TEST_EXPECT_EQ(dt2.GetCount(), 3600000000000LL);
    }

    NSFX_TEST_CASE(Days)
    {
        auto dt = nsfx::chrono::Days(1);
        NSFX_TEST_EXPECT_EQ(dt.GetCount(), 86400);
        NSFX_TEST_EXPECT_EQ(dt.GetResolution(), 1);
        Duration dt2 = dt;
        NSFX_TEST_EXPECT_EQ(dt2.GetCount(), 86400000000000LL);
    }

    NSFX_TEST_CASE(Comparison)
    {
        nsfx::chrono::Duration<nsfx::nano> dt1(1);
        nsfx::chrono::Duration<nsfx::nano> dt2(3);
        NSFX_TEST_EXPECT(dt1 == dt1);
        NSFX_TEST_EXPECT(dt1 <= dt1);
        NSFX_TEST_EXPECT(dt1 >= dt1);

        NSFX_TEST_EXPECT(dt1 != dt2);
        NSFX_TEST_EXPECT(dt1 < dt2);
        NSFX_TEST_EXPECT(dt1 <= dt2);
        NSFX_TEST_EXPECT(dt2 > dt1);
        NSFX_TEST_EXPECT(dt2 >= dt1);

        nsfx::chrono::Duration<nsfx::micro> dt3(999);
        NSFX_TEST_EXPECT(!(dt1 == dt3));
        NSFX_TEST_EXPECT(dt1 != dt3);
        NSFX_TEST_EXPECT(dt1 <= dt3);
        NSFX_TEST_EXPECT(dt1 < dt3);
        NSFX_TEST_EXPECT(!(dt1 >= dt3));
        NSFX_TEST_EXPECT(!(dt1 > dt3));

        NSFX_TEST_EXPECT(!(dt3 == dt1));
        NSFX_TEST_EXPECT(dt3 != dt1);
        NSFX_TEST_EXPECT(dt3 >= dt1);
        NSFX_TEST_EXPECT(dt3 > dt1);
        NSFX_TEST_EXPECT(!(dt3 <= dt1));
        NSFX_TEST_EXPECT(!(dt3 < dt1));
    }

    NSFX_TEST_CASE(Arithmetic)
    {
        Duration dt1(5);
        Duration dt2(7);

        Duration dt3 = dt1 + dt2;
        NSFX_TEST_EXPECT_EQ(dt3, Duration(12));
        NSFX_TEST_EXPECT_EQ(dt3, dt2 + dt1);

        dt3 -= dt2;
        NSFX_TEST_EXPECT_EQ(dt3, dt1);

        dt3 = dt1;
        dt3 += dt2;
        NSFX_TEST_EXPECT_EQ(dt3, dt1 + dt2);

        NSFX_TEST_EXPECT_EQ(dt3 - dt2, dt1);
        NSFX_TEST_EXPECT_EQ(dt3 - dt1, dt2);

        NSFX_TEST_EXPECT_EQ(dt1 * 2, dt1 + dt1);
        NSFX_TEST_EXPECT_EQ(2 * dt1, dt1 + dt1);

        dt3 = dt1;
        dt3 *= 2;
        NSFX_TEST_EXPECT_EQ(dt3, dt1 + dt1);

        Duration dt4(2);
        NSFX_TEST_EXPECT_EQ(dt1 / 2, dt4);

        NSFX_TEST_EXPECT_EQ(dt1 / dt4, 2);
        NSFX_TEST_EXPECT_EQ(dt4 / dt1, 0);

        dt3 = dt1;
        dt3 /= 2;
        NSFX_TEST_EXPECT_EQ(dt3, dt4);

        NSFX_TEST_EXPECT_EQ(dt4 % dt1, dt4);
        NSFX_TEST_EXPECT_EQ(dt1 % dt4, nsfx::chrono::NanoSeconds(1));

        dt3 = dt1;
        dt3 %= dt4;
        NSFX_TEST_EXPECT_EQ(dt3, nsfx::chrono::NanoSeconds(1));

        NSFX_TEST_EXPECT_EQ(-dt1, Duration(-5));
    }

    NSFX_TEST_CASE(Conversion)
    {
        auto dt1 = nsfx::chrono::Days(2)
                 + nsfx::chrono::Hours(2)
                 + nsfx::chrono::Minutes(2)
                 + nsfx::chrono::Seconds(2);
        NSFX_TEST_EXPECT_EQ(dt1,
                            nsfx::chrono::Duration<nsfx::nano>(
                                172800000000000LL +
                                  7200000000000LL +
                                   120000000000LL +
                                     2000000000LL
                            ));

        auto dt2 = nsfx::chrono::Days(2)
                 + nsfx::chrono::MilliSeconds(2);
        NSFX_TEST_EXPECT_EQ(dt2,
                            nsfx::chrono::Duration<nsfx::nano>(
                                172800000000000LL +
                                        2000000LL
                            ));

        auto dt3 = nsfx::chrono::Days(2)
                 + nsfx::chrono::Hours(2)
                 + nsfx::chrono::Minutes(2)
                 + nsfx::chrono::Seconds(2)
                 + nsfx::chrono::MilliSeconds(2)
                 + nsfx::chrono::MicroSeconds(2)
                 + nsfx::chrono::NanoSeconds(2);
        NSFX_TEST_EXPECT_EQ(dt3,
                            nsfx::chrono::Duration<nsfx::nano>(
                                172800000000000LL +
                                  7200000000000LL +
                                   120000000000LL +
                                     2000000000LL +
                                        2000000LL +
                                           2000LL +
                                              2LL
                            ));

        auto dt4 = nsfx::chrono::NanoSeconds(2)
                 + nsfx::chrono::PicoSeconds(2)
                 + nsfx::chrono::FemtoSeconds(2)
                 + nsfx::chrono::AttoSeconds(2);
        NSFX_TEST_EXPECT_EQ(dt4,
                            nsfx::chrono::Duration<nsfx::atto>(
                             2000000000 +
                                2000000 +
                                   2000 +
                                      2
                                ));
        Duration dt5 = dt4;
        NSFX_TEST_EXPECT_EQ(dt5, nsfx::chrono::Duration<nsfx::nano>(2));
    }

    NSFX_TEST_CASE(Hash)
    {
        Duration dt(1000);
        size_t h0 = hash_value(dt);
        size_t h1 = boost::hash<Duration>()(dt);
        NSFX_TEST_EXPECT_EQ(h0, h1);
    }

    NSFX_TEST_CASE(Swap)
    {
        Duration dt1(1234);
        Duration dt2(5678);
        swap(dt1, dt2);
        NSFX_TEST_EXPECT_EQ(dt1, Duration(5678));
        NSFX_TEST_EXPECT_EQ(dt2, Duration(1234));
    }

    NSFX_TEST_CASE(Limits)
    {
        Duration dt0 = Duration::Zero();
        NSFX_TEST_EXPECT_EQ(dt0.GetCount(), 0);

        Duration dmin = Duration::Min();
        NSFX_TEST_EXPECT_EQ(dmin.GetCount(),
                            std::numeric_limits<Duration::Rep>::min());

        Duration dmax = Duration::Max();
        NSFX_TEST_EXPECT_EQ(dmax.GetCount(),
                            std::numeric_limits<Duration::Rep>::max());
    }

}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

