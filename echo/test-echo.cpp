/**
 * @file
 *
 * @brief Tutorials.
 *
 * @version 1.0
 * @author  Wei Tang (gauchyler@uestc.edu.cn)
 * @date    2019-05-14
 *
 * @copyright Copyright (c) 2019.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include "config.h"
#include "i-echo.h"
#include <iostream>


NSFX_TEST_SUITE(Echo)
{
    void Foo(void)
    {
        Ptr<IEcho> o = CreateObject<IEcho>("edu.uestc.nsfx.tutorial.Echo");
        NSFX_TEST_ASSERT(o);
        NSFX_TEST_EXPECT_EQ(o->Shout("Hello!"), "Hello!");
    }

    void Bar(void)
    {
        Ptr<IObject> o = CreateObject<IObject>("edu.uestc.nsfx.tutorial.Echo");
        Ptr<IEcho> p = o;
        p->Shout("Hello!");
    }

    NSFX_TEST_CASE(FooBar)
    {
        try
        {
            Foo();
            Bar();
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

