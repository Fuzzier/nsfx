/**
 * @file
 *
 * @brief Test xoroshiro pseudo-random number generator.
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
#include <nsfx/random/engine/xoroshiro-engine.h>
#include <iostream>


NSFX_TEST_SUITE(xoroshiro)
{
    NSFX_TEST_CASE(xoroshiro64star)
    {
        typedef nsfx::xoroshiro64star rng;

        NSFX_TEST_EXPECT_EQ((rng::min)(), 0);
        NSFX_TEST_EXPECT_EQ((rng::max)(),
                            (std::numeric_limits<uint32_t>::max)());
        NSFX_TEST_EXPECT_EQ(rng::default_seed, 1);
        rng r;
        r();
        r.discard(1000000);
        r.seed(2);
        std::cout << "xoroshiro64*" << std::endl;
        for (size_t i = 0; i < 10; ++i)
        {
            std::cout << "0x" << std::hex << std::setw(8) << std::setfill('0')
                << r() << std::endl;
        }
        std::cout << std::endl;
    }

    NSFX_TEST_CASE(xoroshiro64starstar)
    {
        typedef nsfx::xoroshiro64starstar rng;

        NSFX_TEST_EXPECT_EQ((rng::min)(), 0);
        NSFX_TEST_EXPECT_EQ((rng::max)(),
                            (std::numeric_limits<uint32_t>::max)());
        NSFX_TEST_EXPECT_EQ(rng::default_seed, 1);
        rng r;
        r();
        r.discard(1000000);
        r.seed(2);
        std::cout << "xoroshiro64**" << std::endl;
        for (size_t i = 0; i < 10; ++i)
        {
            std::cout << "0x" << std::hex << std::setw(8) << std::setfill('0')
                << r() << std::endl;
        }
        std::cout << std::endl;
    }

    NSFX_TEST_CASE(xoroshiro128plus)
    {
        typedef nsfx::xoroshiro128plus rng;

        NSFX_TEST_EXPECT_EQ((rng::min)(), 0);
        NSFX_TEST_EXPECT_EQ((rng::max)(),
                            (std::numeric_limits<uint64_t>::max)());
        NSFX_TEST_EXPECT_EQ(rng::default_seed, 1);
        rng r;
        r();
        r.discard(1000000);
        r.seed(2);
        std::cout << "xoroshiro128+" << std::endl;
        for (size_t i = 0; i < 10; ++i)
        {
            std::cout << "0x" << std::hex << std::setw(16) << std::setfill('0')
                << r() << std::endl;
        }
        std::cout << std::endl;
    }

    NSFX_TEST_CASE(xoroshiro128starstar)
    {
        typedef nsfx::xoroshiro128starstar rng;

        NSFX_TEST_EXPECT_EQ((rng::min)(), 0);
        NSFX_TEST_EXPECT_EQ((rng::max)(),
                            (std::numeric_limits<uint64_t>::max)());
        NSFX_TEST_EXPECT_EQ(rng::default_seed, 1);
        rng r;
        r();
        r.discard(1000000);
        r.seed(2);
        std::cout << "xoroshiro128**" << std::endl;
        for (size_t i = 0; i < 10; ++i)
        {
            std::cout << "0x" << std::hex << std::setw(16) << std::setfill('0')
                << r() << std::endl;
        }
        std::cout << std::endl;
    }

    NSFX_TEST_CASE(xoroshiro1024plus)
    {
        typedef nsfx::xoroshiro1024plus rng;

        NSFX_TEST_EXPECT_EQ((rng::min)(), 0);
        NSFX_TEST_EXPECT_EQ((rng::max)(),
                            (std::numeric_limits<uint64_t>::max)());
        NSFX_TEST_EXPECT_EQ(rng::default_seed, 1);
        rng r;
        r();
        r.discard(1000000);
        r.seed(2);
        std::cout << "xoroshiro1024+" << std::endl;
        for (size_t i = 0; i < 10; ++i)
        {
            std::cout << "0x" << std::hex << std::setw(16) << std::setfill('0')
                << r() << std::endl;
        }
        std::cout << std::endl;
    }

}


NSFX_TEST_SUITE(xoroshiro_01)
{
    NSFX_TEST_CASE(xoroshiro64star_01)
    {
        typedef nsfx::xoroshiro64star_01 rng;

        NSFX_TEST_EXPECT_EQ((rng::min)(), 0);
        NSFX_TEST_EXPECT_EQ((rng::max)(), 1);
        NSFX_TEST_EXPECT_EQ(rng::default_seed, 1);
        rng r;
        r();
        r.discard(1000000);
        r.seed(2);
        std::cout << "xoroshiro64*_01" << std::endl;
        for (size_t i = 0; i < 10; ++i)
        {
            std::cout << r() << std::endl;
        }
        std::cout << std::endl;
    }

    NSFX_TEST_CASE(xoroshiro64starstar_01)
    {
        typedef nsfx::xoroshiro64starstar_01 rng;

        NSFX_TEST_EXPECT_EQ((rng::min)(), 0);
        NSFX_TEST_EXPECT_EQ((rng::max)(), 1);
        NSFX_TEST_EXPECT_EQ(rng::default_seed, 1);
        rng r;
        r();
        r.discard(1000000);
        r.seed(2);
        std::cout << "xoroshiro64**_01" << std::endl;
        for (size_t i = 0; i < 10; ++i)
        {
            std::cout << r() << std::endl;
        }
        std::cout << std::endl;
    }

    NSFX_TEST_CASE(xoroshiro128plus_01)
    {
        typedef nsfx::xoroshiro128plus_01 rng;

        NSFX_TEST_EXPECT_EQ((rng::min)(), 0);
        NSFX_TEST_EXPECT_EQ((rng::max)(), 1);
        NSFX_TEST_EXPECT_EQ(rng::default_seed, 1);
        rng r;
        r();
        r.discard(1000000);
        r.seed(2);
        std::cout << "xoroshiro128+_01" << std::endl;
        for (size_t i = 0; i < 10; ++i)
        {
            std::cout << r() << std::endl;
        }
        std::cout << std::endl;
    }

    NSFX_TEST_CASE(xoroshiro128starstar_01)
    {
        typedef nsfx::xoroshiro128starstar_01 rng;

        NSFX_TEST_EXPECT_EQ((rng::min)(), 0);
        NSFX_TEST_EXPECT_EQ((rng::max)(), 1);
        NSFX_TEST_EXPECT_EQ(rng::default_seed, 1);
        rng r;
        r();
        r.discard(1000000);
        r.seed(2);
        std::cout << "xoroshiro128**_01" << std::endl;
        for (size_t i = 0; i < 10; ++i)
        {
            std::cout << r() << std::endl;
        }
        std::cout << std::endl;
    }

}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

