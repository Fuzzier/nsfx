/**
 * @file
 *
 * @brief Test ValueVisitor.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-03-11
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/log/value-visitor.h>
#include <iostream>


NSFX_TEST_SUITE(ValueVisitor)
{
    int i = 0;
    void foo(const int& j)
    {
        i = j;
    }

    NSFX_TEST_CASE(Test)
    {
        try
        {
            nsfx::log::ValueVisitor vv =
                nsfx::log::MakeValueVisitor<int>([&] (const int& value) {
                    i = value;
                });
            NSFX_TEST_EXPECT(vv.GetTypeId() == typeid (int));
            vv.Visit(10);
            NSFX_TEST_EXPECT_EQ(i, 10);

            vv = nsfx::log::MakeValueVisitor(foo);
            NSFX_TEST_EXPECT(vv.GetTypeId() == typeid (int));
            vv.Visit(11);
            NSFX_TEST_EXPECT_EQ(i, 11);

            struct O
            {
                void foo(const int& k)
                {
                    i = k;
                }
                void bar(const int& k) const
                {
                    i = k;
                }
            } o;

            vv = nsfx::log::MakeValueVisitor(&o, &O::foo);
            NSFX_TEST_EXPECT(vv.GetTypeId() == typeid (int));
            vv.Visit(12);
            NSFX_TEST_EXPECT_EQ(i, 12);

            vv = nsfx::log::MakeValueVisitor(&o, &O::bar);
            NSFX_TEST_EXPECT(vv.GetTypeId() == typeid (int));
            vv.Visit(13);
            NSFX_TEST_EXPECT_EQ(i, 13);

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

