#include <nsfx/test.h>
#include <nsfx/component/uuid.h>
#include <iostream>


NSFX_TEST_SUITE(Uuid)
{
    typedef nsfx::Uuid  Uuid;

    NSFX_TEST_CASE(Ctor0)
    {
        Uuid id;
        NSFX_TEST_EXPECT(id.IsNil());
    }

    NSFX_TEST_CASE(Ctor1)
    {
        Uuid id(0x01234567, 0x89ab, 0xcdef, 0x0123456789abcdef);
        std::string s = id.ToString();
        NSFX_TEST_EXPECT_EQ(s, "01234567-89ab-cdef-0123-456789abcdef");
    }

    NSFX_TEST_CASE(Ctor2)
    {
        Uuid id1(0x01234567, 0x89ab, 0xcdef, 0x0123456789abcdef);
        Uuid id2("01234567-89ab-cdef-0123-456789abcdef");
        NSFX_TEST_EXPECT_EQ(id1, id2);
    }

    NSFX_TEST_CASE(Comparisons)
    {
        Uuid id1("01234567-89ab-cdef-0123-456789abcdef");
        Uuid id2("11234567-89ab-cdef-0123-456789abcdef");
        NSFX_TEST_EXPECT_EQ(id1, id1);
        NSFX_TEST_EXPECT_NE(id1, id2);
    }

    NSFX_TEST_CASE(Hash)
    {
        Uuid id(0x01234567, 0x89ab, 0xcdef, 0x0123456789abcdef);
        std::size_t h1 = hash_value(id);
        std::size_t h2 = boost::hash<Uuid>()(id);
        NSFX_TEST_EXPECT_EQ(h1, h2);
    }

}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

