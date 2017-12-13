#include <nsfx/test.h>
#include <nsfx/chrono/duration.h>
#include <iostream>


NSFX_TEST_SUITE(Duration)
{
    NSFX_TEST_CASE(Constructor)
    {
        nsfx::chrono::Duration td;
        NSFX_TEST_EXPECT_EQ(td.GetNanoSeconds(), 0);
    }

    NSFX_TEST_CASE(NanoSeconds)
    {
        auto value = nsfx::chrono::Duration::NANO_SECOND;
        nsfx::chrono::Duration td = nsfx::chrono::NanoSeconds(1);
        NSFX_TEST_EXPECT_EQ(value, 1);
        NSFX_TEST_EXPECT_EQ(td.GetNanoSeconds(), value);
    }

    NSFX_TEST_CASE(MicroSeconds)
    {
        auto value = nsfx::chrono::Duration::MICRO_SECOND;
        nsfx::chrono::Duration td = nsfx::chrono::MicroSeconds(1);
        NSFX_TEST_EXPECT_EQ(value, 1000);
        NSFX_TEST_EXPECT_EQ(td.GetNanoSeconds(), value);
    }

    NSFX_TEST_CASE(MilliSeconds)
    {
        auto value = nsfx::chrono::Duration::MILLI_SECOND;
        nsfx::chrono::Duration td = nsfx::chrono::MilliSeconds(1);
        NSFX_TEST_EXPECT_EQ(value, 1000000);
        NSFX_TEST_EXPECT_EQ(td.GetNanoSeconds(), value);
    }

    NSFX_TEST_CASE(Seconds)
    {
        auto value = nsfx::chrono::Duration::SECOND;
        nsfx::chrono::Duration td = nsfx::chrono::Seconds(1);
        NSFX_TEST_EXPECT_EQ(value, 1000000000);
        NSFX_TEST_EXPECT_EQ(td.GetNanoSeconds(), value);
    }

    NSFX_TEST_CASE(Minutes)
    {
        auto value = nsfx::chrono::Duration::MINUTE;
        nsfx::chrono::Duration td = nsfx::chrono::Minutes(1);
        NSFX_TEST_EXPECT_EQ(value, 60000000000LL);
        NSFX_TEST_EXPECT_EQ(td.GetNanoSeconds(), value);
    }

    NSFX_TEST_CASE(Hours)
    {
        auto value = nsfx::chrono::Duration::HOUR;
        nsfx::chrono::Duration td = nsfx::chrono::Hours(1);
        NSFX_TEST_EXPECT_EQ(value, 3600000000000LL);
        NSFX_TEST_EXPECT_EQ(td.GetNanoSeconds(), value);
        NSFX_TEST_EXPECT_EQ(td, nsfx::chrono::Hours(1));
    }

    NSFX_TEST_CASE(Days)
    {
        auto value = nsfx::chrono::Duration::DAY;
        nsfx::chrono::Duration td = nsfx::chrono::Days(1);
        NSFX_TEST_EXPECT_EQ(value, 86400000000000LL);
        NSFX_TEST_EXPECT_EQ(td, nsfx::chrono::Days(1));
    }

    NSFX_TEST_CASE(Comparisons)
    {
        nsfx::chrono::Duration td1(1, 2, 3, 4, 5, 6);
        nsfx::chrono::Duration td2(3, 2, 1, 4, 5, 6);

        NSFX_TEST_EXPECT(td1 == td1);
        NSFX_TEST_EXPECT(td1 <= td1);
        NSFX_TEST_EXPECT(td1 >= td1);

        NSFX_TEST_EXPECT(td1 != td2);
        NSFX_TEST_EXPECT(td1 < td2);
        NSFX_TEST_EXPECT(td1 <= td2);
        NSFX_TEST_EXPECT(td2 > td1);
        NSFX_TEST_EXPECT(td2 >= td1);
    }

    NSFX_TEST_CASE(Algorithms)
    {
        nsfx::chrono::Duration td1(1, 2, 3, 4, 5, 5);
        nsfx::chrono::Duration td2(3, 2, 1, 4, 5, 6);

        nsfx::chrono::Duration td3 = td1 + td2;
        NSFX_TEST_EXPECT_EQ(td3, nsfx::chrono::Duration(4, 4, 4, 8, 10, 11));
        NSFX_TEST_EXPECT_EQ(td3, td2 + td1);

        td3 -= td2;
        NSFX_TEST_EXPECT_EQ(td3, td1);

        td3 = td1;
        td3 += td2;
        NSFX_TEST_EXPECT_EQ(td3, td1 + td2);

        NSFX_TEST_EXPECT_EQ(td3 - td2, td1);
        NSFX_TEST_EXPECT_EQ(td3 - td1, td2);

        NSFX_TEST_EXPECT_EQ(td1 * 2, td1 + td1);
        NSFX_TEST_EXPECT_EQ(2 * td1, td1 + td1);

        td3 = td1;
        td3 *= 2;
        NSFX_TEST_EXPECT_EQ(td3, td1 + td1);

        nsfx::chrono::Duration td4(0, 31, 1, 502, 2, 502);
        NSFX_TEST_EXPECT_EQ(td1 / 2, td4);

        NSFX_TEST_EXPECT_EQ(td1 / td4, 2);
        NSFX_TEST_EXPECT_EQ(td4 / td1, 0);

        td3 = td1;
        td3 /= 2;
        NSFX_TEST_EXPECT_EQ(td3, td4);

        NSFX_TEST_EXPECT_EQ(td4 % td1, td4);
        NSFX_TEST_EXPECT_EQ(td1 % td4, nsfx::chrono::NanoSeconds(1));

        td3 = td1;
        td3 %= td4;
        NSFX_TEST_EXPECT_EQ(td3, nsfx::chrono::NanoSeconds(1));

        NSFX_TEST_EXPECT_EQ(-td1, nsfx::chrono::Duration(-1, -2, -3, -4, -5, -5));
    }
}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

