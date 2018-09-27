/**
 * @file
 *
 * @brief Test xoshiro pseudo-random number generator.
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
#include <nsfx/random/engine/xoshiro-engine.h>
#include <iostream>


NSFX_TEST_SUITE(xoshiro)
{
    NSFX_TEST_CASE(xoshiro128plus)
    {
        typedef nsfx::xoshiro128plus rng;

        NSFX_TEST_EXPECT_EQ((rng::min)(), 0);
        NSFX_TEST_EXPECT_EQ((rng::max)(),
                            (std::numeric_limits<uint32_t>::max)());
        NSFX_TEST_EXPECT_EQ(rng::default_seed, 1);
        rng r;
        r();
        r.discard(1000000);
        r.seed(2);
        std::cout << "xoshiro128+" << std::endl;
        for (size_t i = 0; i < 10; ++i)
        {
            std::cout << "0x" << std::hex << std::setw(8) << std::setfill('0')
                << r() << std::endl;
        }
        std::cout << std::endl;
    }

    NSFX_TEST_CASE(xoshiro128starstar)
    {
        typedef nsfx::xoshiro128starstar rng;

        NSFX_TEST_EXPECT_EQ((rng::min)(), 0);
        NSFX_TEST_EXPECT_EQ((rng::max)(),
                            (std::numeric_limits<uint32_t>::max)());
        NSFX_TEST_EXPECT_EQ(rng::default_seed, 1);
        rng r;
        r();
        r.discard(1000000);
        r.seed(2);
        std::cout << "xoshiro128**" << std::endl;
        for (size_t i = 0; i < 10; ++i)
        {
            std::cout << "0x" << std::hex << std::setw(8) << std::setfill('0')
                << r() << std::endl;
        }
        std::cout << std::endl;
    }

    NSFX_TEST_CASE(xoshiro256plus)
    {
        typedef nsfx::xoshiro256plus rng;

        NSFX_TEST_EXPECT_EQ((rng::min)(), 0);
        NSFX_TEST_EXPECT_EQ((rng::max)(),
                            (std::numeric_limits<uint64_t>::max)());
        NSFX_TEST_EXPECT_EQ(rng::default_seed, 1);
        rng r;
        r();
        r.discard(1000000);
        r.seed(2);
        std::cout << "xoshiro256+" << std::endl;
        for (size_t i = 0; i < 10; ++i)
        {
            std::cout << "0x" << std::hex << std::setw(8) << std::setfill('0')
                << r() << std::endl;
        }
        std::cout << std::endl;
    }

    NSFX_TEST_CASE(xoshiro256starstar)
    {
        typedef nsfx::xoshiro256starstar rng;

        NSFX_TEST_EXPECT_EQ((rng::min)(), 0);
        NSFX_TEST_EXPECT_EQ((rng::max)(),
                            (std::numeric_limits<uint64_t>::max)());
        NSFX_TEST_EXPECT_EQ(rng::default_seed, 1);
        rng r;
        r();
        r.discard(1000000);
        r.seed(2);
        std::cout << "xoshiro256**" << std::endl;
        for (size_t i = 0; i < 10; ++i)
        {
            std::cout << "0x" << std::hex << std::setw(8) << std::setfill('0')
                << r() << std::endl;
        }
        std::cout << std::endl;
    }

    NSFX_TEST_CASE(xoshiro512plus)
    {
        typedef nsfx::xoshiro512plus rng;

        NSFX_TEST_EXPECT_EQ((rng::min)(), 0);
        NSFX_TEST_EXPECT_EQ((rng::max)(),
                            (std::numeric_limits<uint64_t>::max)());
        NSFX_TEST_EXPECT_EQ(rng::default_seed, 1);
        rng r;
        r();
        r.discard(1000000);
        r.seed(2);
        std::cout << "xoshiro512+" << std::endl;
        for (size_t i = 0; i < 10; ++i)
        {
            std::cout << "0x" << std::hex << std::setw(8) << std::setfill('0')
                << r() << std::endl;
        }
        std::cout << std::endl;
    }

    NSFX_TEST_CASE(xoshiro512starstar)
    {
        typedef nsfx::xoshiro512starstar rng;

        NSFX_TEST_EXPECT_EQ((rng::min)(), 0);
        NSFX_TEST_EXPECT_EQ((rng::max)(),
                            (std::numeric_limits<uint64_t>::max)());
        NSFX_TEST_EXPECT_EQ(rng::default_seed, 1);
        rng r;
        r();
        r.discard(1000000);
        r.seed(2);
        std::cout << "xoshiro512**" << std::endl;
        for (size_t i = 0; i < 10; ++i)
        {
            std::cout << "0x" << std::hex << std::setw(8) << std::setfill('0')
                << r() << std::endl;
        }
        std::cout << std::endl;
    }

}


NSFX_TEST_SUITE(xoshiro_01)
{
    /* NSFX_TEST_CASE(xoshiro64star_01) */
    /* { */
    /*     typedef nsfx::xoshiro64star_01 rng; */
    /*  */
    /*     NSFX_TEST_EXPECT_EQ((rng::min)(), 0); */
    /*     NSFX_TEST_EXPECT_EQ((rng::max)(), 1); */
    /*     NSFX_TEST_EXPECT_EQ(rng::default_seed, 1); */
    /*     rng r; */
    /*     r(); */
    /*     r.discard(1000000); */
    /*     r.seed(2); */
    /*     std::cout << "xoshiro64*_01" << std::endl; */
    /*     for (size_t i = 0; i < 10; ++i) */
    /*     { */
    /*         std::cout << r() << std::endl; */
    /*     } */
    /*     std::cout << std::endl; */
    /* } */
    /*  */
}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

