/**
 * @file
 *
 * @brief Test splitmix pseudo-random number generator.
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
#include <nsfx/random/engine/splitmix-engine.h>
#include <iostream>


NSFX_TEST_SUITE(splitmix)
{
    typedef nsfx::splitmix64 rng;

    NSFX_TEST_CASE(Test)
    {
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


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cerr);
    nsfx::test::runner::Run();

    return 0;
}

