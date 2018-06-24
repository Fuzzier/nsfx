/**
 * @file
 *
 * @brief Test Interoperability.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-06-19
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/interop/return-type-checker.h>
#include <iostream>


NSFX_TEST_SUITE(Try1)
{
    NSFX_TEST_CASE(1)
    {
        nsfx::Ptr<nsfx::ITry> intf(new nsfx::Object<nsfx::Try>());
        nsfx::Ptr<nsfx::IMethodInfo> m =
            nsfx::CreateMethodInfo(intf, &nsfx::ITry::Foo);

        NSFX_TEST_EXPECT(!strcmp(m->GetName(), "Foo"));
        NSFX_TEST_EXPECT_EQ(m->GetNumArguments(), 1);
        NSFX_TEST_EXPECT(!strcmp(m->GetReturnType(), "void")) << m->GetReturnType();
        NSFX_TEST_EXPECT(!strcmp(m->GetArgumentType(0), "bool"));
    }
}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

