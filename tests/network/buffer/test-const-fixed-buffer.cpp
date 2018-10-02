/**
 * @file
 *
 * @brief Test FixedBuffer.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-06-06
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/network/buffer.h>
#include <iostream>


NSFX_TEST_SUITE(ConstFixedBuffer)
{
    NSFX_TEST_CASE(Ctor)
    {
        nsfx::FixedBuffer b0(300);
        nsfx::ConstFixedBuffer cb(b0);
    }
}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cerr);
    nsfx::test::runner::Run();

    return 0;
}

