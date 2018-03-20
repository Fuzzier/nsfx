/**
 * @file
 *
 * @brief Test AttributeValue.
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
#include <nsfx/log/attribute-value.h>
#include <iostream>


NSFX_TEST_SUITE(AttributeValue)
{
    NSFX_TEST_CASE(Test)
    {
        try
        {
            {
                nsfx::log::AttributeValue v =
                    nsfx::log::MakeConstantAttributeValue<int>();
                NSFX_TEST_EXPECT(v.GetTypeId() == typeid (int));
            }

            {
                nsfx::log::AttributeValue v =
                    nsfx::log::MakeConstantAttributeValue<int>(10);
                NSFX_TEST_EXPECT(v.GetTypeId() == typeid (int));
                NSFX_TEST_EXPECT_EQ(v.Get<int>(), 10);
            }

            struct A
            {
                A(int i): i_(i) {}
                int i_;
            };

            {
                nsfx::log::AttributeValue v =
                    nsfx::log::MakeConstantAttributeValue<A>(10);
                NSFX_TEST_EXPECT(v.GetTypeId() == typeid (A));
                NSFX_TEST_EXPECT_EQ(v.Get<A>().i_, 10);
            }

            {
                A a(10);
                nsfx::log::AttributeValue v =
                    nsfx::log::MakeConstantAttributeValue<A*>(&a);
                NSFX_TEST_EXPECT(v.GetTypeId() == typeid (A*));
                NSFX_TEST_EXPECT_EQ(v.Get<A*>()->i_, 10);
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
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

