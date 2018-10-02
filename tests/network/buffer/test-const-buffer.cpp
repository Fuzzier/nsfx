/**
 * @file
 *
 * @brief Test Buffer.
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
#include <nsfx/network/buffer/buffer.h>
#include <nsfx/network/buffer/const-buffer.h>
#include <iostream>


NSFX_TEST_SUITE(ConstBuffer)
{
    NSFX_TEST_SUITE(Ctor)
    {
        NSFX_TEST_CASE(Ctor)
        {
            nsfx::Buffer b0(1000);
            nsfx::ConstBuffer cb(b0);
        }
    }
}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cerr);
    nsfx::test::runner::Run();

    return 0;
}

