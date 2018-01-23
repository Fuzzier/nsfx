/**
 * @file
 *
 * @brief Test TestSuite.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-07
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <iostream>

#define SHOULD_NOT_SEE  "This message should not be displayed. Something is wrong."
#define SHOULD_SEE      "This message is displayed as intended."

NSFX_TEST_SUITE(suite1)
{
    NSFX_TEST_CASE(case0)
    {
        int i = 1;
        int j = 1;
        NSFX_TEST_EXPECT(i == j) << SHOULD_NOT_SEE;
        NSFX_TEST_EXPECT(i != j) << SHOULD_SEE;
    }

    NSFX_TEST_CASE(case1)
    {
        int i = 1;
        int j = 1;
        NSFX_TEST_EXPECT_EQ(i, j) << SHOULD_NOT_SEE;
    }

    NSFX_TEST_CASE(case2)
    {
        int i = 1;
        int j = 2;
        NSFX_TEST_EXPECT_EQ(i, j) << SHOULD_SEE;
    }

    NSFX_TEST_SUITE(suite2)
    {
        NSFX_TEST_CASE(case1)
        {
            int i = 1;
            int j = 1;
            NSFX_TEST_EXPECT_EQ(i, j) << SHOULD_NOT_SEE;
        }

        NSFX_TEST_CASE(case2)
        {
            int i = 1;
            int j = 2;
            NSFX_TEST_EXPECT_EQ(i, j) << SHOULD_SEE;
        }

        NSFX_TEST_CASE(case3)
        {
            double i = 5;
            double j = 6;
            double k = 0.1;
            NSFX_TEST_EXPECT_AC(i, j, k) << SHOULD_SEE;
            NSFX_TEST_EXPECT_RC(i, j, k) << SHOULD_SEE;
        }

    }

}

void case4(void)
{
    [] {
        double i = 5;
        double j = 6;
        double k = 0.1;
        NSFX_TEST_EXPECT_AC(i, j, k) << SHOULD_SEE;
        NSFX_TEST_EXPECT_RC(i, j, k) << SHOULD_SEE;
    } ();
}

NSFX_TEST_SUITE(ValueFormat)
{
    NSFX_TEST_CASE(Bool)
    {
        bool i = true;
        bool j = false;
        NSFX_TEST_EXPECT_EQ(i, j) << SHOULD_SEE;
    }

    NSFX_TEST_CASE(Pointer)
    {
        int* i = (int*)(0x01234567);
        int* j = (int*)(0x89abcdef);
        NSFX_TEST_EXPECT_EQ(i, j) << SHOULD_SEE;
    }

    NSFX_TEST_CASE(Char)
    {
        char i = -1;
        char j = -2;
        NSFX_TEST_EXPECT_EQ(i, j) << SHOULD_SEE;
    }

    NSFX_TEST_CASE(Wchar)
    {
        wchar_t i = -1;
        wchar_t j = -2;
        NSFX_TEST_EXPECT_EQ(i, j) << SHOULD_SEE;
    }

    NSFX_TEST_CASE(Short)
    {
        short i = -1;
        short j = -2;
        NSFX_TEST_EXPECT_EQ(i, j) << SHOULD_SEE;
    }

    NSFX_TEST_CASE(Long)
    {
        long i = -1;
        long j = -2;
        NSFX_TEST_EXPECT_EQ(i, j) << SHOULD_SEE;
    }

    NSFX_TEST_CASE(Longlong)
    {
        long long i = -1;
        long long j = -2;
        NSFX_TEST_EXPECT_EQ(i, j) << SHOULD_SEE;
    }

    NSFX_TEST_CASE(Float)
    {
        float i = -1;
        float j = -2;
        float k = 0.1;
        NSFX_TEST_EXPECT_RC(i, j, k) << SHOULD_SEE;
    }

    NSFX_TEST_CASE(Double)
    {
        double i = -1;
        double j = -2;
        double k = 0.1;
        NSFX_TEST_EXPECT_RC(i, j, k) << SHOULD_SEE;
    }

}


/**
 * @brief Test suite.
 *
 * 1. Auto test suite "suite1".
 * 2. Auto test suite "suite2" defined inside "suite1".
 * 3. Manually add test suite "suite3".
 *
 * Also tested test loggers.
 * 1. std::cerr.
 * 2. file "test.log".
 */
int main(void)
{
    nsfx::test::Suite* s =
        nsfx::test::runner::GetMasterSuite()->AddSuite("suite3");
    s->AddCase("case4", &case4);

    nsfx::test::runner::GetLogger()->AddStreamSink(std::cerr);
    nsfx::test::runner::GetLogger()->AddFileSink("test.log");
    nsfx::test::runner::Run();

    std::cout << "The total number of test failures should be 7." << std::endl;

    return 0;
}

