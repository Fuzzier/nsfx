/**
 * @file
 *
 * @brief Test ZcBuffer.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-05-26
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/network/buffer.h>
#include <iostream>


NSFX_TEST_SUITE(ConstZcBuffer)
{
    NSFX_TEST_CASE(Ctor)
    {
        nsfx::ZcBuffer b0(1000);
        nsfx::ConstZcBuffer cb(b0);
    }
}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

