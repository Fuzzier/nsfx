/**
 * @file
 *
 * @brief Alarm.
 *
 * @version 1.0
 * @author  Wei Tang (gauchyler@uestc.edu.cn)
 * @date    2018-04-20
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include "config.h"
#include "i-nameable.h"
#include <iostream>
#include <string>


NSFX_TEST_SUITE(Nameable)
{
    void Test(void)
    {
        Ptr<INameable> o = CreateObject<INameable>(
                            "edu.uestc.nsfx.tutorial.Nameable");
        o->SetName("Foobar");
        NSFX_TEST_EXPECT_EQ(o->GetName(), "Foobar");
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
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cerr);
    nsfx::test::runner::Run();

    return 0;
}

