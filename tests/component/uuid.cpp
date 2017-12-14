#include <nsfx/test.h>
#include <nsfx/component/uuid.h>
#include <iostream>


NSFX_TEST_SUITE(uuid)
{
    typedef nsfx::uuid  uuid;

    NSFX_TEST_CASE(DEFINE_UUID)
    {
        NSFX_DEFINE_UUID4(id0, 0, 0, 0, 0LL);
        NSFX_TEST_EXPECT(id0.is_nil());

        NSFX_DEFINE_UUID (id1, 0x01234567, 0x89ab, 0xcdef, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef);
        NSFX_DEFINE_UUID4(id2, 0x01234567, 0x89ab, 0xcdef, 0x0123456789abcdefLL);
        NSFX_TEST_EXPECT_EQ(id1, id2);

        std::string s1 = to_string(id1);
        NSFX_TEST_EXPECT_EQ(s1, "01234567-89ab-cdef-0123-456789abcdef");
    }

    NSFX_TEST_CASE(DEFINE_TYPE_UUID)
    {
        struct Object
        {
            NSFX_DEFINE_CLASS_UUID4(0x01234567, 0x89ab, 0xcdef, 0x0123456789abcdefLL);
        }* o = nullptr;

        struct Interface
        {
            NSFX_DEFINE_CLASS_UUID4(0x11234567, 0x89ab, 0xcdef, 0x0123456789abcdefLL);
        }* i = nullptr;

        NSFX_TEST_EXPECT_EQ(to_string(nsfx::uuid_of<Object>()),
                            "01234567-89ab-cdef-0123-456789abcdef");
        NSFX_TEST_EXPECT_EQ(to_string(nsfx::uuid_of(o)),
                            "01234567-89ab-cdef-0123-456789abcdef");

        NSFX_TEST_EXPECT_EQ(to_string(nsfx::uuid_of<Interface>()),
                            "11234567-89ab-cdef-0123-456789abcdef");
        NSFX_TEST_EXPECT_EQ(to_string(nsfx::uuid_of(i)),
                            "11234567-89ab-cdef-0123-456789abcdef");
    }

}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

