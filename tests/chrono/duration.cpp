#include <nsfx/test.h>
#include <nsfx/chrono/duration.h>
#include <iostream>


NSFX_TEST_SUITE(Duration)
{
    typedef nsfx::chrono::Duration Duration;

    NSFX_TEST_CASE(Constructor)
    {
        Duration td;
        NSFX_TEST_EXPECT_EQ(td.ToNanoSeconds(), 0);
    }

    NSFX_TEST_CASE(NanoSeconds)
    {
        auto value = Duration::NANO_SECOND;
        Duration td = nsfx::chrono::NanoSeconds(1);
        NSFX_TEST_EXPECT_EQ(value, 1);
        NSFX_TEST_EXPECT_EQ(td.ToNanoSeconds(), value);
    }

    NSFX_TEST_CASE(MicroSeconds)
    {
        auto value = Duration::MICRO_SECOND;
        Duration td = nsfx::chrono::MicroSeconds(1);
        NSFX_TEST_EXPECT_EQ(value, 1000);
        NSFX_TEST_EXPECT_EQ(td.ToNanoSeconds(), value);
    }

    NSFX_TEST_CASE(MilliSeconds)
    {
        auto value = Duration::MILLI_SECOND;
        Duration td = nsfx::chrono::MilliSeconds(1);
        NSFX_TEST_EXPECT_EQ(value, 1000000);
        NSFX_TEST_EXPECT_EQ(td.ToNanoSeconds(), value);
    }

    NSFX_TEST_CASE(Seconds)
    {
        auto value = Duration::SECOND;
        Duration td = nsfx::chrono::Seconds(1);
        NSFX_TEST_EXPECT_EQ(value, 1000000000);
        NSFX_TEST_EXPECT_EQ(td.ToNanoSeconds(), value);
    }

    NSFX_TEST_CASE(Minutes)
    {
        auto value = Duration::MINUTE;
        Duration td = nsfx::chrono::Minutes(1);
        NSFX_TEST_EXPECT_EQ(value, 60000000000LL);
        NSFX_TEST_EXPECT_EQ(td.ToNanoSeconds(), value);
    }

    NSFX_TEST_CASE(Hours)
    {
        auto value = Duration::HOUR;
        Duration td = nsfx::chrono::Hours(1);
        NSFX_TEST_EXPECT_EQ(value, 3600000000000LL);
        NSFX_TEST_EXPECT_EQ(td.ToNanoSeconds(), value);
        NSFX_TEST_EXPECT_EQ(td, nsfx::chrono::Hours(1));
    }

    NSFX_TEST_CASE(Days)
    {
        auto value = Duration::DAY;
        Duration td = nsfx::chrono::Days(1);
        NSFX_TEST_EXPECT_EQ(value, 86400000000000LL);
        NSFX_TEST_EXPECT_EQ(td, nsfx::chrono::Days(1));
    }

    NSFX_TEST_CASE(Comparisons)
    {
        Duration dt1(1, 2, 3, 4, 5, 6);
        Duration dt2(3, 2, 1, 4, 5, 6);

        NSFX_TEST_EXPECT(dt1 == dt1);
        NSFX_TEST_EXPECT(dt1 <= dt1);
        NSFX_TEST_EXPECT(dt1 >= dt1);

        NSFX_TEST_EXPECT(dt1 != dt2);
        NSFX_TEST_EXPECT(dt1 < dt2);
        NSFX_TEST_EXPECT(dt1 <= dt2);
        NSFX_TEST_EXPECT(dt2 > dt1);
        NSFX_TEST_EXPECT(dt2 >= dt1);
    }

    NSFX_TEST_CASE(Algorithms)
    {
        Duration dt1(1, 2, 3, 4, 5, 5);
        Duration dt2(3, 2, 1, 4, 5, 6);

        Duration dt3 = dt1 + dt2;
        NSFX_TEST_EXPECT_EQ(dt3, Duration(4, 4, 4, 8, 10, 11));
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

        Duration dt4(0, 31, 1, 502, 2, 502);
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

        NSFX_TEST_EXPECT_EQ(-dt1, Duration(-1, -2, -3, -4, -5, -5));
    }

    NSFX_TEST_CASE(Hash)
    {
        Duration d(1000);
        size_t h0 = hash_value(d);
        size_t h1 = boost::hash<Duration>()(d);
        NSFX_TEST_EXPECT_EQ(h0, h1);
    }

    NSFX_TEST_CASE(Swap)
    {
        Duration d1(1234);
        Duration d2(5678);
        swap(d1, d2);
        NSFX_TEST_EXPECT_EQ(d1, Duration(5678));
        NSFX_TEST_EXPECT_EQ(d2, Duration(1234));
    }

    NSFX_TEST_CASE(Limits)
    {
        Duration d0 = Duration::Zero();
        NSFX_TEST_EXPECT_EQ(d0.ToNanoSeconds(), 0);

        Duration dmin = Duration::Min();
        NSFX_TEST_EXPECT_EQ(dmin.ToNanoSeconds(),
                            std::numeric_limits<Duration::Rep>::min());

        Duration dmax = Duration::Max();
        NSFX_TEST_EXPECT_EQ(dmax.ToNanoSeconds(),
                            std::numeric_limits<Duration::Rep>::max());
    }

}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

