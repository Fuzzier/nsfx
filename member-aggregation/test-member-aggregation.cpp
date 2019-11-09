/**
 * @file
 *
 * @brief MemberAggregation.
 *
 * @version 1.0
 * @author  Wei Tang (gauchyler@uestc.edu.cn)
 * @date    2019-05-17
 *
 * @copyright Copyright (c) 2019.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include "config.h"
// 包含所使用的接口。
#include "../nameable/i-nameable.h"
#include <iostream>
#include <string>


NSFX_TEST_SUITE(Aggregation)
{
    using namespace nsfx;

    void Test(void)
    {
        ////////////////////
        // MemberAggregation
        Ptr<INameable> o = CreateObject<INameable>(
                            "edu.uestc.nsfx.tutorial.MemberAggregation");

        o->SetName("MemberAggregated");
        NSFX_TEST_EXPECT_EQ(o->GetName(), "MemberAggregated");
    }

    NSFX_TEST_CASE(Test)
    {
        try
        {
            Test();
        }
        catch (boost::exception& e)
        {
            std::cout << diagnostic_information(e) << std::endl;
        }
        catch (std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }
    }
}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

