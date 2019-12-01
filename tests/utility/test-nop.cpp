/**
 * @file
 *
 * @brief Test nop.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2019-12-01
 *
 * @copyright Copyright (c) 2019.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/utility/nop.h>
#include <iostream>


NSFX_TEST_SUITE(nop)
{
    NSFX_TEST_CASE(0)
    {
        nsfx::nop<> x;
        x();
        x(1);
        x(1, 2);
    }

    NSFX_TEST_CASE(1)
    {
        nsfx::nop<int> x;
        x();
        x(1);
        x(1, 2);
    }

    NSFX_TEST_CASE(2)
    {
        nsfx::nop<int, double> x;
        x();
        x(1);
        x(1, 2);
    }
}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cerr);
    nsfx::test::runner::Run();

    return 0;
}

