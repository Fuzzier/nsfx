/**
 * @file
 *
 * @brief Test uid.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-14
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/component/uid.h>
#include <iostream>


NSFX_TEST_SUITE(uid)
{
    struct Object
    {
    }* o = nullptr;
    NSFX_DEFINE_CLASS_UID(Object, "edu.uestc.nsfx.test.Object");

    struct Interface
    {
    }* i = nullptr;
    NSFX_DEFINE_CLASS_UID(Interface, "edu.uestc.nsfx.test.Interface");

    NSFX_TEST_CASE(DEFINE_TYPE_UID)
    {
        NSFX_TEST_EXPECT(nsfx::uid_of<Object>() == "edu.uestc.nsfx.test.Object");
        NSFX_TEST_EXPECT(nsfx::uid_of<Interface>() == "edu.uestc.nsfx.test.Interface");
    }

}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cerr);
    nsfx::test::runner::Run();

    return 0;
}

