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
        rng::result_type x = r();
        r.discard(1000000);
        r.seed(2);
        for (size_t i = 0; i < 10; ++i)
        {
            x = r();
            NSFX_TEST_EXPECT_GT(x, (rng::min)());
            NSFX_TEST_EXPECT_LT(x, (rng::max)());
        }
    }

    NSFX_TEST_CASE(xoshiro128starstar)
    {
        typedef nsfx::xoshiro128starstar rng;

        NSFX_TEST_EXPECT_EQ((rng::min)(), 0);
        NSFX_TEST_EXPECT_EQ((rng::max)(),
                            (std::numeric_limits<uint32_t>::max)());
        NSFX_TEST_EXPECT_EQ(rng::default_seed, 1);
        rng r;
        rng::result_type x = r();
        r.discard(1000000);
        r.seed(2);
        for (size_t i = 0; i < 10; ++i)
        {
            x = r();
            NSFX_TEST_EXPECT_GT(x, (rng::min)());
            NSFX_TEST_EXPECT_LT(x, (rng::max)());
        }
    }

    NSFX_TEST_CASE(xoshiro256plus)
    {
        typedef nsfx::xoshiro256plus rng;

        NSFX_TEST_EXPECT_EQ((rng::min)(), 0);
        NSFX_TEST_EXPECT_EQ((rng::max)(),
                            (std::numeric_limits<uint64_t>::max)());
        NSFX_TEST_EXPECT_EQ(rng::default_seed, 1);
        rng r;
        rng::result_type x = r();
        r.discard(1000000);
        r.seed(2);
        for (size_t i = 0; i < 10; ++i)
        {
            x = r();
            NSFX_TEST_EXPECT_GT(x, (rng::min)());
            NSFX_TEST_EXPECT_LT(x, (rng::max)());
        }
    }

    NSFX_TEST_CASE(xoshiro256starstar)
    {
        typedef nsfx::xoshiro256starstar rng;

        NSFX_TEST_EXPECT_EQ((rng::min)(), 0);
        NSFX_TEST_EXPECT_EQ((rng::max)(),
                            (std::numeric_limits<uint64_t>::max)());
        NSFX_TEST_EXPECT_EQ(rng::default_seed, 1);
        rng r;
        rng::result_type x = r();
        r.discard(1000000);
        r.seed(2);
        for (size_t i = 0; i < 10; ++i)
        {
            x = r();
            NSFX_TEST_EXPECT_GT(x, (rng::min)());
            NSFX_TEST_EXPECT_LT(x, (rng::max)());
        }
    }

    NSFX_TEST_CASE(xoshiro512plus)
    {
        typedef nsfx::xoshiro512plus rng;

        NSFX_TEST_EXPECT_EQ((rng::min)(), 0);
        NSFX_TEST_EXPECT_EQ((rng::max)(),
                            (std::numeric_limits<uint64_t>::max)());
        NSFX_TEST_EXPECT_EQ(rng::default_seed, 1);
        rng r;
        rng::result_type x = r();
        r.discard(1000000);
        r.seed(2);
        for (size_t i = 0; i < 10; ++i)
        {
            x = r();
            NSFX_TEST_EXPECT_GT(x, (rng::min)());
            NSFX_TEST_EXPECT_LT(x, (rng::max)());
        }
    }

    NSFX_TEST_CASE(xoshiro512starstar)
    {
        typedef nsfx::xoshiro512starstar rng;

        NSFX_TEST_EXPECT_EQ((rng::min)(), 0);
        NSFX_TEST_EXPECT_EQ((rng::max)(),
                            (std::numeric_limits<uint64_t>::max)());
        NSFX_TEST_EXPECT_EQ(rng::default_seed, 1);
        rng r;
        rng::result_type x = r();
        r.discard(1000000);
        r.seed(2);
        for (size_t i = 0; i < 10; ++i)
        {
            x = r();
            NSFX_TEST_EXPECT_GT(x, (rng::min)());
            NSFX_TEST_EXPECT_LT(x, (rng::max)());
        }
    }

}


NSFX_TEST_SUITE(xoshiro_01)
{
    NSFX_TEST_CASE(xoshiro128plus_01)
    {
        typedef nsfx::xoshiro128plus_01 rng;

        NSFX_TEST_EXPECT_EQ((rng::min)(), 0);
        NSFX_TEST_EXPECT_EQ((rng::max)(), 1);
        NSFX_TEST_EXPECT_EQ(rng::default_seed, 1);
        rng r;
        rng::result_type x = r();
        r.discard(1000000);
        r.seed(2);
        for (size_t i = 0; i < 10; ++i)
        {
            x = r();
            NSFX_TEST_EXPECT_GT(x, (rng::min)());
            NSFX_TEST_EXPECT_LT(x, (rng::max)());
        }
    }

    NSFX_TEST_CASE(xoshiro128starstar_01)
    {
        typedef nsfx::xoshiro128starstar_01 rng;

        NSFX_TEST_EXPECT_EQ((rng::min)(), 0);
        NSFX_TEST_EXPECT_EQ((rng::max)(), 1);
        NSFX_TEST_EXPECT_EQ(rng::default_seed, 1);
        rng r;
        rng::result_type x = r();
        r.discard(1000000);
        r.seed(2);
        for (size_t i = 0; i < 10; ++i)
        {
            x = r();
            NSFX_TEST_EXPECT_GT(x, (rng::min)());
            NSFX_TEST_EXPECT_LT(x, (rng::max)());
        }
    }

    NSFX_TEST_CASE(xoshiro256plus_01)
    {
        typedef nsfx::xoshiro256plus_01 rng;

        NSFX_TEST_EXPECT_EQ((rng::min)(), 0);
        NSFX_TEST_EXPECT_EQ((rng::max)(), 1);
        NSFX_TEST_EXPECT_EQ(rng::default_seed, 1);
        rng r;
        rng::result_type x = r();
        r.discard(1000000);
        r.seed(2);
        for (size_t i = 0; i < 10; ++i)
        {
            x = r();
            NSFX_TEST_EXPECT_GT(x, (rng::min)());
            NSFX_TEST_EXPECT_LT(x, (rng::max)());
        }
    }

    NSFX_TEST_CASE(xoshiro256starstar_01)
    {
        typedef nsfx::xoshiro256starstar_01 rng;

        NSFX_TEST_EXPECT_EQ((rng::min)(), 0);
        NSFX_TEST_EXPECT_EQ((rng::max)(), 1);
        NSFX_TEST_EXPECT_EQ(rng::default_seed, 1);
        rng r;
        rng::result_type x = r();
        r.discard(1000000);
        r.seed(2);
        for (size_t i = 0; i < 10; ++i)
        {
            x = r();
            NSFX_TEST_EXPECT_GT(x, (rng::min)());
            NSFX_TEST_EXPECT_LT(x, (rng::max)());
        }
    }

    NSFX_TEST_CASE(xoshiro512plus_01)
    {
        typedef nsfx::xoshiro512plus_01 rng;

        NSFX_TEST_EXPECT_EQ((rng::min)(), 0);
        NSFX_TEST_EXPECT_EQ((rng::max)(), 1);
        NSFX_TEST_EXPECT_EQ(rng::default_seed, 1);
        rng r;
        rng::result_type x = r();
        r.discard(1000000);
        r.seed(2);
        for (size_t i = 0; i < 10; ++i)
        {
            x = r();
            NSFX_TEST_EXPECT_GT(x, (rng::min)());
            NSFX_TEST_EXPECT_LT(x, (rng::max)());
        }
    }

    NSFX_TEST_CASE(xoshiro512starstar_01)
    {
        typedef nsfx::xoshiro512starstar_01 rng;

        NSFX_TEST_EXPECT_EQ((rng::min)(), 0);
        NSFX_TEST_EXPECT_EQ((rng::max)(), 1);
        NSFX_TEST_EXPECT_EQ(rng::default_seed, 1);
        rng r;
        rng::result_type x = r();
        r.discard(1000000);
        r.seed(2);
        for (size_t i = 0; i < 10; ++i)
        {
            x = r();
            NSFX_TEST_EXPECT_GT(x, (rng::min)());
            NSFX_TEST_EXPECT_LT(x, (rng::max)());
        }
    }

}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cerr);
    nsfx::test::runner::Run();

    return 0;
}

