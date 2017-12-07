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
        nsfx::test::runner::getMasterSuite()->addSuite("suite3");
    s->addCase("case4", &case4);

    nsfx::test::runner::getLogger()->addStreamSink(std::cerr);
    nsfx::test::runner::getLogger()->addFileSink("test.log");
    nsfx::test::runner::run();

    std::cout << "The total number of test failures should be 7." << std::endl;

    return 0;
}

