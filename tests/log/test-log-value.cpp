/**
 * @file
 *
 * @brief Test LogValue.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-03-20
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/log/make-log-value.h>
#include <iostream>


NSFX_TEST_SUITE(LogValue)
{
    int Foo(void)
    {
        return 10;
    }

    struct F
    {
        int operator()(void) { return 10; }
    };

    struct V
    {
        V(int a, int b) : v_(a + b) {}
        int v_;
    };

    NSFX_TEST_CASE(Test)
    {
        try
        {
            // Lambda expression.
            {
                nsfx::LogValue v = nsfx::MakeLogValue<int>([] { return 10; });
                NSFX_TEST_EXPECT(v.GetTypeId() == boost::typeindex::type_id<int>());
                NSFX_TEST_EXPECT_EQ(v.Get<int>(), 10);
            }

            // Function pointer.
            {
                nsfx::LogValue v = nsfx::MakeLogValue<int>(&Foo);
                NSFX_TEST_EXPECT(v.GetTypeId() == boost::typeindex::type_id<int>());
                NSFX_TEST_EXPECT_EQ(v.Get<int>(), 10);
            }

            // Callable object.
            {
                nsfx::LogValue v = nsfx::MakeLogValue<int>(F());
                NSFX_TEST_EXPECT(v.GetTypeId() == boost::typeindex::type_id<int>());
                NSFX_TEST_EXPECT_EQ(v.Get<int>(), 10);
            }

            // Value 0.
            {
                nsfx::LogValue v = nsfx::MakeConstantLogValue<int>();
                NSFX_TEST_EXPECT(v.GetTypeId() == boost::typeindex::type_id<int>());
            }

            // Value 1.
            {
                nsfx::LogValue v = nsfx::MakeConstantLogValue<int>(10);
                NSFX_TEST_EXPECT(v.GetTypeId() == boost::typeindex::type_id<int>());
                NSFX_TEST_EXPECT_EQ(v.Get<int>(), 10);
            }

            // Value 2.
            {
                nsfx::LogValue v = nsfx::MakeConstantLogValue<V>(1, 9);
                NSFX_TEST_EXPECT(v.GetTypeId() == boost::typeindex::type_id<V>());
                NSFX_TEST_EXPECT_EQ(v.Get<V>().v_, 10);
            }

        }
        catch (boost::exception& e)
        {
            std::cerr << diagnostic_information(e) << std::endl;
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cerr);
    nsfx::test::runner::Run();

    return 0;
}

