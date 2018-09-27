/**
 * @file
 *
 * @brief Test rot.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-09-27
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/random/detail/rot.h>
#include <iostream>


NSFX_TEST_SUITE(rot)
{
    NSFX_TEST_CASE(rotl)
    {
        NSFX_TEST_EXPECT_EQ(nsfx::random::aux::rotl(
                0x15555555UL, 1),
                0x2aaaaaaaUL);
        NSFX_TEST_EXPECT_EQ(nsfx::random::aux::rotl(
                0x15555555UL, 2),
                0x55555554UL);
        NSFX_TEST_EXPECT_EQ(nsfx::random::aux::rotl(
                0x15555555UL, 3),
                0xaaaaaaa8UL);
        NSFX_TEST_EXPECT_EQ(nsfx::random::aux::rotl(
                0x15555555UL, 4),
                0x55555551UL);
        NSFX_TEST_EXPECT_EQ(nsfx::random::aux::rotl(
                0x15555555UL, 5),
                0xaaaaaaa2UL);
        NSFX_TEST_EXPECT_EQ(nsfx::random::aux::rotl(
                0x15555555UL, 6),
                0x55555545UL);
        NSFX_TEST_EXPECT_EQ(nsfx::random::aux::rotl(
                0x15555555UL, 32),
                0x15555555UL);

        NSFX_TEST_EXPECT_EQ(nsfx::random::aux::rotl(
                0x1555555555555555ULL, 1),
                0x2aaaaaaaaaaaaaaaULL);
        NSFX_TEST_EXPECT_EQ(nsfx::random::aux::rotl(
                0x1555555555555555ULL, 2),
                0x5555555555555554ULL);
        NSFX_TEST_EXPECT_EQ(nsfx::random::aux::rotl(
                0x1555555555555555ULL, 3),
                0xaaaaaaaaaaaaaaa8ULL);
        NSFX_TEST_EXPECT_EQ(nsfx::random::aux::rotl(
                0x1555555555555555ULL, 4),
                0x5555555555555551ULL);
        NSFX_TEST_EXPECT_EQ(nsfx::random::aux::rotl(
                0x1555555555555555ULL, 5),
                0xaaaaaaaaaaaaaaa2ULL);
        NSFX_TEST_EXPECT_EQ(nsfx::random::aux::rotl(
                0x1555555555555555ULL, 6),
                0x5555555555555545ULL);
        NSFX_TEST_EXPECT_EQ(nsfx::random::aux::rotl(
                0x1555555555555555ULL, 64),
                0x1555555555555555ULL);
    }

    NSFX_TEST_CASE(rotr)
    {
        NSFX_TEST_EXPECT_EQ(nsfx::random::aux::rotr(
                0x15555555UL, 1),
                0x8aaaaaaaUL);
        NSFX_TEST_EXPECT_EQ(nsfx::random::aux::rotr(
                0x15555555UL, 2),
                0x45555555UL);
        NSFX_TEST_EXPECT_EQ(nsfx::random::aux::rotr(
                0x15555555UL, 3),
                0xa2aaaaaaUL);
        NSFX_TEST_EXPECT_EQ(nsfx::random::aux::rotr(
                0x15555555UL, 4),
                0x51555555UL);
        NSFX_TEST_EXPECT_EQ(nsfx::random::aux::rotr(
                0x15555555UL, 5),
                0xa8aaaaaaUL);
        NSFX_TEST_EXPECT_EQ(nsfx::random::aux::rotr(
                0x15555555UL, 6),
                0x54555555UL);
        NSFX_TEST_EXPECT_EQ(nsfx::random::aux::rotr(
                0x15555555UL, 32),
                0x15555555UL);

        NSFX_TEST_EXPECT_EQ(nsfx::random::aux::rotr(
                0x1555555555555555ULL, 1),
                0x8aaaaaaaaaaaaaaaULL);
        NSFX_TEST_EXPECT_EQ(nsfx::random::aux::rotr(
                0x1555555555555555ULL, 2),
                0x4555555555555555ULL);
        NSFX_TEST_EXPECT_EQ(nsfx::random::aux::rotr(
                0x1555555555555555ULL, 3),
                0xa2aaaaaaaaaaaaaaULL);
        NSFX_TEST_EXPECT_EQ(nsfx::random::aux::rotr(
                0x1555555555555555ULL, 4),
                0x5155555555555555ULL);
        NSFX_TEST_EXPECT_EQ(nsfx::random::aux::rotr(
                0x1555555555555555ULL, 5),
                0xa8aaaaaaaaaaaaaaULL);
        NSFX_TEST_EXPECT_EQ(nsfx::random::aux::rotr(
                0x1555555555555555ULL, 6),
                0x5455555555555555ULL);
        NSFX_TEST_EXPECT_EQ(nsfx::random::aux::rotr(
                0x1555555555555555ULL, 64),
                0x1555555555555555ULL);

        for (size_t k = 0; k <= 32; ++k)
        {
            NSFX_TEST_EXPECT_EQ(
                nsfx::random::aux::rotl(0x9abcdefUL, k),
                nsfx::random::aux::rotr(0x9abcdefUL, 32-k));
        }

        for (size_t k = 0; k <= 64; ++k)
        {
            NSFX_TEST_EXPECT_EQ(
                nsfx::random::aux::rotl(0x123456789abcdef0ULL, k),
                nsfx::random::aux::rotr(0x123456789abcdef0ULL, 64-k));
        }
    }

}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

