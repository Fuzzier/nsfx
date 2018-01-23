/**
 * @file
 *
 * @brief Test uuid.
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
#include <nsfx/component/uuid.h>
#include <iostream>


NSFX_TEST_SUITE(uuid)
{
    typedef nsfx::uuid  uuid;

    struct Object
    {
    }* o = nullptr;
    NSFX_DEFINE_CLASS_UUID(Object, 0x01234567, 0x89ab, 0xcdef, 0x0123456789abcdefLL);

    struct Interface
    {
    }* i = nullptr;
    NSFX_DEFINE_CLASS_UUID(Interface, 0x11234567, 0x89ab, 0xcdef, 0x0123456789abcdefLL);

    NSFX_TEST_CASE(DEFINE_TYPE_UUID)
    {
        NSFX_TEST_EXPECT_EQ(to_string(nsfx::uuid_of<Object>()),
                            "01234567-89ab-cdef-0123-456789abcdef");

        NSFX_TEST_EXPECT_EQ(to_string(nsfx::uuid_of<Interface>()),
                            "11234567-89ab-cdef-0123-456789abcdef");
    }

}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

