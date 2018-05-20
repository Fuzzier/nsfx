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
#include <nsfx/log/core/attribute-value/const-attribute-value.h>
#include <iostream>


NSFX_TEST_SUITE(ConstAttributeValue)
{
    NSFX_TEST_CASE(Test)
    {
        try
        {
            {
                nsfx::log::AttributeValue v =
                    nsfx::log::MakeConstantAttributeValue<int>();
                NSFX_TEST_EXPECT(v.GetTypeId() == boost::typeindex::type_id<int>());
            }

            {
                nsfx::log::AttributeValue v =
                    nsfx::log::MakeConstantAttributeValue<int>(10);
                NSFX_TEST_EXPECT(v.GetTypeId() == boost::typeindex::type_id<int>());
                NSFX_TEST_EXPECT_EQ(v.Get<int>(), 10);
            }

            struct Test
            {
                Test(int i): i_(i) {}
                int i_;
            };

            {
                nsfx::log::AttributeValue v =
                    nsfx::log::MakeConstantAttributeValue<Test>(10);
                NSFX_TEST_EXPECT(v.GetTypeId() == boost::typeindex::type_id<Test>());
                NSFX_TEST_EXPECT_EQ(v.Get<Test>().i_, 10);
            }

            {
                Test t(10);
                nsfx::log::AttributeValue v =
                    nsfx::log::MakeConstantAttributeValue<Test*>(&t);
                NSFX_TEST_EXPECT(v.GetTypeId() == boost::typeindex::type_id<Test*>());
                NSFX_TEST_EXPECT_EQ(v.Get<Test*>()->i_, 10);
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

