/**
 * @file
 *
 * @brief Test LexicalCompare.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2019-11-26
 *
 * @copyright Copyright (c) 2019.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/utility/lexical-compare.h>
#include <iostream>


NSFX_TEST_SUITE(LexicalCompare)
{
    NSFX_TEST_CASE(1)
    {
        struct A
        {
            int i;
        };
        A a = { 1 };
        A b = { 0 };
        NSFX_TEST_EXPECT_EQ(nsfx::LexicalCompare(a, b, &A::i), 1);
        b.i = 1;
        NSFX_TEST_EXPECT_EQ(nsfx::LexicalCompare(a, b, &A::i), 0);
        b.i = 2;
        NSFX_TEST_EXPECT_EQ(nsfx::LexicalCompare(a, b, &A::i), -1);
    }

    NSFX_TEST_CASE(2)
    {
        struct A
        {
            int i;
            double j;
        };
        A a = { 1, 1 };
        A b = { 1, 0 };
        NSFX_TEST_EXPECT_EQ(nsfx::LexicalCompare(a, b, &A::i, &A::j), 1);
        b.j = 1;
        NSFX_TEST_EXPECT_EQ(nsfx::LexicalCompare(a, b, &A::i, &A::j), 0);
        b.j = 2;
        NSFX_TEST_EXPECT_EQ(nsfx::LexicalCompare(a, b, &A::i, &A::j), -1);
    }

    NSFX_TEST_CASE(3)
    {
        struct A
        {
            int i;
            double j;
            char k;
        };
        A a = { 1, 0, 'c' };
        A b = { 1, 0, 'b' };
        NSFX_TEST_EXPECT_EQ(nsfx::LexicalCompare(a, b, &A::i, &A::j, &A::k), 1);
        b.k = 'c';
        NSFX_TEST_EXPECT_EQ(nsfx::LexicalCompare(a, b, &A::i, &A::j, &A::k), 0);
        b.k = 'd';
        NSFX_TEST_EXPECT_EQ(nsfx::LexicalCompare(a, b, &A::i, &A::j, &A::k), -1);
    }

}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cerr);
    nsfx::test::runner::Run();

    return 0;
}

