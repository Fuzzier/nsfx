/**
 * @file
 *
 * @brief Test Address IO.
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
#include <nsfx/network/buffer/io/address-io.h>
#include <iostream>


NSFX_TEST_SUITE(AddressIo)
{
    using namespace nsfx;
    typedef Address<116>  Address;

    NSFX_TEST_CASE(Native)
    {
        Buffer buffer;
        buffer.AddAtStart(Address::GetSize());

        Address a0(0x000dcba987654321ULL, 0x00000000ffffffffULL, big_endian);
        auto itw = buffer.begin();
        Write(itw, a0);

        Address a1;
        auto itr = buffer.cbegin();
        Read(itr, &a1);

        NSFX_TEST_EXPECT_EQ(a0, a1);
    }

    NSFX_TEST_CASE(Big)
    {
        Buffer buffer;
        buffer.AddAtStart(Address::GetSize());

        Address a0(0x000dcba987654321ULL, 0x00000000ffffffffULL, big_endian);
        auto itw = buffer.begin();
        WriteB(itw, a0);

        Address a1;
        auto itr = buffer.cbegin();
        ReadB(itr, &a1);

        NSFX_TEST_EXPECT_EQ(a0, a1);
    }

    NSFX_TEST_CASE(Little)
    {
        Buffer buffer;
        buffer.AddAtStart(Address::GetSize());

        Address a0(0x000dcba987654321ULL, 0x00000000ffffffffULL, big_endian);
        auto itw = buffer.begin();
        WriteL(itw, a0);

        Address a1;
        auto itr = buffer.cbegin();
        ReadL(itr, &a1);

        NSFX_TEST_EXPECT_EQ(a0, a1);
    }

}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cerr);
    nsfx::test::runner::Run();

    return 0;
}

