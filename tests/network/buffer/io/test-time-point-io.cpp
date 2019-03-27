/**
 * @file
 *
 * @brief Test TimePoint IO.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2019-03-25
 *
 * @copyright Copyright (c) 2019.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/network/buffer.h>
#include <nsfx/network/buffer/io/time-point-io.h>
#include <nsfx/chrono/virtual-time-point.h>
#include <iostream>


NSFX_TEST_SUITE(TimePointIo)
{
    using namespace nsfx;
    using namespace nsfx::chrono;
    typedef VirtualTimePoint  TimePoint;

    NSFX_TEST_CASE(Native)
    {
        Buffer buffer;
        buffer.AddAtStart(TimePoint::GetSize());

        TimePoint t0(Seconds(10));
        auto itw = buffer.begin();
        Write(itw, t0);

        TimePoint t1;
        auto itr = buffer.cbegin();
        Read(itr, &t1);

        NSFX_TEST_EXPECT_EQ(t0, t1);
    }

    NSFX_TEST_CASE(Big)
    {
        Buffer buffer;
        buffer.AddAtStart(TimePoint::GetSize());

        TimePoint t0(Seconds(10));
        auto itw = buffer.begin();
        WriteB(itw, t0);

        TimePoint t1;
        auto itr = buffer.cbegin();
        ReadB(itr, &t1);

        NSFX_TEST_EXPECT_EQ(t0, t1);
    }

    NSFX_TEST_CASE(Little)
    {
        Buffer buffer;
        buffer.AddAtStart(TimePoint::GetSize());

        TimePoint t0(Seconds(10));
        auto itw = buffer.begin();
        WriteL(itw, t0);

        TimePoint t1;
        auto itr = buffer.cbegin();
        ReadL(itr, &t1);

        NSFX_TEST_EXPECT_EQ(t0, t1);
    }

}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cerr);
    nsfx::test::runner::Run();

    return 0;
}

