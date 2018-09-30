/**
 * @file
 *
 * @brief Test AttributeValue.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-05-15
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/log/core/attribute/const-attribute.h>
#include <iostream>


NSFX_TEST_SUITE(ConstAttribute)
{
    NSFX_TEST_CASE(Test)
    {
        try
        {
            {
                nsfx::log::Attribute a =
                    nsfx::log::MakeConstantAttribute<int>();
                nsfx::log::AttributeValue v = a.GetValue();
                NSFX_TEST_EXPECT(v.GetTypeId() == boost::typeindex::type_id<int>());
            }

            {
                nsfx::log::Attribute a =
                    nsfx::log::MakeConstantAttribute<int>(10);
                nsfx::log::AttributeValue v = a.GetValue();
                NSFX_TEST_EXPECT(v.GetTypeId() == boost::typeindex::type_id<int>());
                NSFX_TEST_EXPECT_EQ(v.Get<int>(), 10);
            }

            struct Test
            {
                Test(int i): i_(i) {}
                int i_;
            };

            {
                nsfx::log::Attribute a =
                    nsfx::log::MakeConstantAttribute<Test>(10);
                nsfx::log::AttributeValue v = a.GetValue();
                NSFX_TEST_EXPECT(v.GetTypeId() == boost::typeindex::type_id<Test>());
                NSFX_TEST_EXPECT_EQ(v.Get<Test>().i_, 10);
            }

            {
                Test t(10);
                nsfx::log::Attribute a =
                    nsfx::log::MakeConstantAttribute<Test*>(&t);
                nsfx::log::AttributeValue v = a.GetValue();
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

