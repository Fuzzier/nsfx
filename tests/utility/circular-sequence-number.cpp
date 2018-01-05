#include <nsfx/test.h>
#include <nsfx/utility/circular-sequence-number.h>
#include <iostream>


NSFX_TEST_SUITE(LeastInt)/*{{{*/
{
    NSFX_TEST_CASE(0)
    {
        typedef nsfx::LeastInt<0>::Type Type;
        NSFX_TEST_EXPECT((std::is_same<Type, nsfx::uint8_t>::value));
    }

    NSFX_TEST_CASE(8)
    {
        typedef nsfx::LeastInt<8>::Type Type;
        NSFX_TEST_EXPECT((std::is_same<Type, nsfx::uint8_t>::value));
    }

    NSFX_TEST_CASE(17)
    {
        typedef nsfx::LeastInt<17>::Type Type;
        NSFX_TEST_EXPECT((std::is_same<Type, nsfx::uint32_t>::value));
    }

    NSFX_TEST_CASE(32)
    {
        typedef nsfx::LeastInt<32>::Type Type;
        NSFX_TEST_EXPECT((std::is_same<Type, nsfx::uint32_t>::value));
    }

    NSFX_TEST_CASE(33)
    {
        typedef nsfx::LeastInt<33>::Type Type;
        NSFX_TEST_EXPECT((std::is_same<Type, nsfx::uint64_t>::value));
    }

    NSFX_TEST_CASE(64)
    {
        typedef nsfx::LeastInt<64>::Type Type;
        NSFX_TEST_EXPECT((std::is_same<Type, nsfx::uint64_t>::value));
    }

}/*}}}*/


NSFX_TEST_SUITE(CircularSequenceNumberTraits)/*{{{*/
{
    NSFX_TEST_CASE(2)
    {
        typedef nsfx::CircularSequenceNumberTraits<2> TraitsType;
        NSFX_TEST_EXPECT_EQ(TraitsType::MAX_GAP, 0x01);
        NSFX_TEST_EXPECT_EQ(TraitsType::MAX_VALUE, 0x03);
        NSFX_TEST_EXPECT((std::is_same<typename TraitsType::ValueType,
                          nsfx::uint8_t>::value));
    }

    NSFX_TEST_CASE(8)
    {
        typedef nsfx::CircularSequenceNumberTraits<8> TraitsType;
        NSFX_TEST_EXPECT_EQ(TraitsType::MAX_GAP, 0x7F);
        NSFX_TEST_EXPECT_EQ(TraitsType::MAX_VALUE, 0xFF);
        NSFX_TEST_EXPECT((std::is_same<typename TraitsType::ValueType,
                          nsfx::uint8_t>::value));
    }

    NSFX_TEST_CASE(9)
    {
        typedef nsfx::CircularSequenceNumberTraits<9> TraitsType;
        NSFX_TEST_EXPECT_EQ(TraitsType::MAX_GAP, 0x00FF);
        NSFX_TEST_EXPECT_EQ(TraitsType::MAX_VALUE, 0x01FF);
        NSFX_TEST_EXPECT((std::is_same<typename TraitsType::ValueType,
                          nsfx::uint16_t>::value));
    }

    NSFX_TEST_CASE(16)
    {
        typedef nsfx::CircularSequenceNumberTraits<16> TraitsType;
        NSFX_TEST_EXPECT_EQ(TraitsType::MAX_GAP, 0x7FFF);
        NSFX_TEST_EXPECT_EQ(TraitsType::MAX_VALUE, 0xFFFF);
        NSFX_TEST_EXPECT((std::is_same<typename TraitsType::ValueType,
                          nsfx::uint16_t>::value));
    }

    NSFX_TEST_CASE(17)
    {
        typedef nsfx::CircularSequenceNumberTraits<17> TraitsType;
        NSFX_TEST_EXPECT_EQ(TraitsType::MAX_GAP, 0x0000FFFF);
        NSFX_TEST_EXPECT_EQ(TraitsType::MAX_VALUE, 0x0001FFFF);
        NSFX_TEST_EXPECT((std::is_same<typename TraitsType::ValueType,
                          nsfx::uint32_t>::value));
    }

    NSFX_TEST_CASE(32)
    {
        typedef nsfx::CircularSequenceNumberTraits<32> TraitsType;
        NSFX_TEST_EXPECT_EQ(TraitsType::MAX_GAP, 0x7FFFFFFF);
        NSFX_TEST_EXPECT_EQ(TraitsType::MAX_VALUE, 0xFFFFFFFF);
        NSFX_TEST_EXPECT((std::is_same<typename TraitsType::ValueType,
                          nsfx::uint32_t>::value));
    }

    NSFX_TEST_CASE(33)
    {
        typedef nsfx::CircularSequenceNumberTraits<33> TraitsType;
        NSFX_TEST_EXPECT_EQ(TraitsType::MAX_GAP, 0x00000000FFFFFFFFULL);
        NSFX_TEST_EXPECT_EQ(TraitsType::MAX_VALUE, 0x00000001FFFFFFFFULL);
        NSFX_TEST_EXPECT((std::is_same<typename TraitsType::ValueType,
                          nsfx::uint64_t>::value));
    }

    NSFX_TEST_CASE(64)
    {
        typedef nsfx::CircularSequenceNumberTraits<64> TraitsType;
        NSFX_TEST_EXPECT_EQ(TraitsType::MAX_GAP, 0x7FFFFFFFFFFFFFFFULL);
        NSFX_TEST_EXPECT_EQ(TraitsType::MAX_VALUE, 0xFFFFFFFFFFFFFFFFULL);
        NSFX_TEST_EXPECT((std::is_same<typename TraitsType::ValueType,
                          nsfx::uint64_t>::value));
    }

}/*}}}*/


NSFX_TEST_SUITE(CircularSequenceNumber)/*{{{*/
{
    NSFX_TEST_CASE(2)
    {
        typedef nsfx::CircularSequenceNumber<2> S;
        typedef nsfx::CircularSequenceNumberTraits<2>::ValueType V;
        V M = nsfx::CircularSequenceNumberTraits<2>::MAX_VALUE;
        V G = nsfx::CircularSequenceNumberTraits<2>::MAX_GAP;
        S s(M);
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(M));
        NSFX_TEST_EXPECT_EQ(s++, static_cast<V>(M));
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(0));
        s = S(M);
        NSFX_TEST_EXPECT_EQ(++s, static_cast<V>(0));
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(0));
        s = S(0);
        NSFX_TEST_EXPECT_EQ(s--, static_cast<V>(0));
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(M));
        s = S(0);
        NSFX_TEST_EXPECT_EQ(--s, static_cast<V>(M));
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(M));
        NSFX_TEST_EXPECT(s == s);
        NSFX_TEST_EXPECT(s <= s);
        NSFX_TEST_EXPECT(s >= s);
        NSFX_TEST_EXPECT(!(s < s));
        NSFX_TEST_EXPECT(!(s > s));
        S n = S(0);
        s = S(G);
        NSFX_TEST_EXPECT(n != s);
        NSFX_TEST_EXPECT(n <= s);
        NSFX_TEST_EXPECT(n < s);
        NSFX_TEST_EXPECT(s > n);
        NSFX_TEST_EXPECT(s >= n);
        ++s;
        NSFX_TEST_EXPECT(n != s);
        NSFX_TEST_EXPECT(n >= s);
        NSFX_TEST_EXPECT(n > s);
        NSFX_TEST_EXPECT(s < n);
        NSFX_TEST_EXPECT(s <= n);
    }

    NSFX_TEST_CASE(8)
    {
        typedef nsfx::CircularSequenceNumber<8> S;
        typedef nsfx::CircularSequenceNumberTraits<8>::ValueType V;
        V M = nsfx::CircularSequenceNumberTraits<8>::MAX_VALUE;
        V G = nsfx::CircularSequenceNumberTraits<8>::MAX_GAP;
        S s(M);
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(M));
        NSFX_TEST_EXPECT_EQ(s++, static_cast<V>(M));
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(0));
        s = S(M);
        NSFX_TEST_EXPECT_EQ(++s, static_cast<V>(0));
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(0));
        s = S(0);
        NSFX_TEST_EXPECT_EQ(s--, static_cast<V>(0));
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(M));
        s = S(0);
        NSFX_TEST_EXPECT_EQ(--s, static_cast<V>(M));
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(M));
        NSFX_TEST_EXPECT(s == s);
        NSFX_TEST_EXPECT(s <= s);
        NSFX_TEST_EXPECT(s >= s);
        NSFX_TEST_EXPECT(!(s < s));
        NSFX_TEST_EXPECT(!(s > s));
        S n = S(1);
        s = S(1 + G);
        NSFX_TEST_EXPECT(n != s);
        NSFX_TEST_EXPECT(n <= s);
        NSFX_TEST_EXPECT(n < s);
        NSFX_TEST_EXPECT(s > n);
        NSFX_TEST_EXPECT(s >= n);
        ++s;
        NSFX_TEST_EXPECT(n != s);
        NSFX_TEST_EXPECT(n >= s);
        NSFX_TEST_EXPECT(n > s);
        NSFX_TEST_EXPECT(s < n);
        NSFX_TEST_EXPECT(s <= n);
    }

    NSFX_TEST_CASE(9)
    {
        typedef nsfx::CircularSequenceNumber<9> S;
        typedef nsfx::CircularSequenceNumberTraits<9>::ValueType V;
        V M = nsfx::CircularSequenceNumberTraits<9>::MAX_VALUE;
        V G = nsfx::CircularSequenceNumberTraits<9>::MAX_GAP;
        S s(M);
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(M));
        NSFX_TEST_EXPECT_EQ(s++, static_cast<V>(M));
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(0));
        s = S(M);
        NSFX_TEST_EXPECT_EQ(++s, static_cast<V>(0));
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(0));
        s = S(0);
        NSFX_TEST_EXPECT_EQ(s--, static_cast<V>(0));
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(M));
        s = S(0);
        NSFX_TEST_EXPECT_EQ(--s, static_cast<V>(M));
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(M));
        NSFX_TEST_EXPECT(s == s);
        NSFX_TEST_EXPECT(s <= s);
        NSFX_TEST_EXPECT(s >= s);
        NSFX_TEST_EXPECT(!(s < s));
        NSFX_TEST_EXPECT(!(s > s));
        S n = S(1);
        s = S(1 + G);
        NSFX_TEST_EXPECT(n != s);
        NSFX_TEST_EXPECT(n <= s);
        NSFX_TEST_EXPECT(n < s);
        NSFX_TEST_EXPECT(s > n);
        NSFX_TEST_EXPECT(s >= n);
        ++s;
        NSFX_TEST_EXPECT(n != s);
        NSFX_TEST_EXPECT(n >= s);
        NSFX_TEST_EXPECT(n > s);
        NSFX_TEST_EXPECT(s < n);
        NSFX_TEST_EXPECT(s <= n);
    }

    NSFX_TEST_CASE(16)
    {
        typedef nsfx::CircularSequenceNumber<16> S;
        typedef nsfx::CircularSequenceNumberTraits<16>::ValueType V;
        V M = nsfx::CircularSequenceNumberTraits<16>::MAX_VALUE;
        V G = nsfx::CircularSequenceNumberTraits<16>::MAX_GAP;
        S s(M);
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(M));
        NSFX_TEST_EXPECT_EQ(s++, static_cast<V>(M));
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(0));
        s = S(M);
        NSFX_TEST_EXPECT_EQ(++s, static_cast<V>(0));
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(0));
        s = S(0);
        NSFX_TEST_EXPECT_EQ(s--, static_cast<V>(0));
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(M));
        s = S(0);
        NSFX_TEST_EXPECT_EQ(--s, static_cast<V>(M));
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(M));
        NSFX_TEST_EXPECT(s == s);
        NSFX_TEST_EXPECT(s <= s);
        NSFX_TEST_EXPECT(s >= s);
        NSFX_TEST_EXPECT(!(s < s));
        NSFX_TEST_EXPECT(!(s > s));
        S n = S(1);
        s = S(1 + G);
        NSFX_TEST_EXPECT(n != s);
        NSFX_TEST_EXPECT(n <= s);
        NSFX_TEST_EXPECT(n < s);
        NSFX_TEST_EXPECT(s > n);
        NSFX_TEST_EXPECT(s >= n);
        ++s;
        NSFX_TEST_EXPECT(n != s);
        NSFX_TEST_EXPECT(n >= s);
        NSFX_TEST_EXPECT(n > s);
        NSFX_TEST_EXPECT(s < n);
        NSFX_TEST_EXPECT(s <= n);
    }

    NSFX_TEST_CASE(17)
    {
        typedef nsfx::CircularSequenceNumber<17> S;
        typedef nsfx::CircularSequenceNumberTraits<17>::ValueType V;
        V M = nsfx::CircularSequenceNumberTraits<17>::MAX_VALUE;
        V G = nsfx::CircularSequenceNumberTraits<17>::MAX_GAP;
        S s(M);
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(M));
        NSFX_TEST_EXPECT_EQ(s++, static_cast<V>(M));
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(0));
        s = S(M);
        NSFX_TEST_EXPECT_EQ(++s, static_cast<V>(0));
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(0));
        s = S(0);
        NSFX_TEST_EXPECT_EQ(s--, static_cast<V>(0));
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(M));
        s = S(0);
        NSFX_TEST_EXPECT_EQ(--s, static_cast<V>(M));
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(M));
        NSFX_TEST_EXPECT(s == s);
        NSFX_TEST_EXPECT(s <= s);
        NSFX_TEST_EXPECT(s >= s);
        NSFX_TEST_EXPECT(!(s < s));
        NSFX_TEST_EXPECT(!(s > s));
        S n = S(1);
        s = S(1 + G);
        NSFX_TEST_EXPECT(n != s);
        NSFX_TEST_EXPECT(n <= s);
        NSFX_TEST_EXPECT(n < s);
        NSFX_TEST_EXPECT(s > n);
        NSFX_TEST_EXPECT(s >= n);
        ++s;
        NSFX_TEST_EXPECT(n != s);
        NSFX_TEST_EXPECT(n >= s);
        NSFX_TEST_EXPECT(n > s);
        NSFX_TEST_EXPECT(s < n);
        NSFX_TEST_EXPECT(s <= n);
    }

    NSFX_TEST_CASE(32)
    {
        typedef nsfx::CircularSequenceNumber<32> S;
        typedef nsfx::CircularSequenceNumberTraits<32>::ValueType V;
        V M = nsfx::CircularSequenceNumberTraits<32>::MAX_VALUE;
        V G = nsfx::CircularSequenceNumberTraits<32>::MAX_GAP;
        S s(M);
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(M));
        NSFX_TEST_EXPECT_EQ(s++, static_cast<V>(M));
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(0));
        s = S(M);
        NSFX_TEST_EXPECT_EQ(++s, static_cast<V>(0));
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(0));
        s = S(0);
        NSFX_TEST_EXPECT_EQ(s--, static_cast<V>(0));
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(M));
        s = S(0);
        NSFX_TEST_EXPECT_EQ(--s, static_cast<V>(M));
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(M));
        NSFX_TEST_EXPECT(s == s);
        NSFX_TEST_EXPECT(s <= s);
        NSFX_TEST_EXPECT(s >= s);
        NSFX_TEST_EXPECT(!(s < s));
        NSFX_TEST_EXPECT(!(s > s));
        S n = S(1);
        s = S(1 + G);
        NSFX_TEST_EXPECT(n != s);
        NSFX_TEST_EXPECT(n <= s);
        NSFX_TEST_EXPECT(n < s);
        NSFX_TEST_EXPECT(s > n);
        NSFX_TEST_EXPECT(s >= n);
        ++s;
        NSFX_TEST_EXPECT(n != s);
        NSFX_TEST_EXPECT(n >= s);
        NSFX_TEST_EXPECT(n > s);
        NSFX_TEST_EXPECT(s < n);
        NSFX_TEST_EXPECT(s <= n);
    }

    NSFX_TEST_CASE(33)
    {
        typedef nsfx::CircularSequenceNumber<33> S;
        typedef nsfx::CircularSequenceNumberTraits<33>::ValueType V;
        V M = nsfx::CircularSequenceNumberTraits<33>::MAX_VALUE;
        V G = nsfx::CircularSequenceNumberTraits<33>::MAX_GAP;
        S s(M);
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(M));
        NSFX_TEST_EXPECT_EQ(s++, static_cast<V>(M));
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(0));
        s = S(M);
        NSFX_TEST_EXPECT_EQ(++s, static_cast<V>(0));
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(0));
        s = S(0);
        NSFX_TEST_EXPECT_EQ(s--, static_cast<V>(0));
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(M));
        s = S(0);
        NSFX_TEST_EXPECT_EQ(--s, static_cast<V>(M));
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(M));
        NSFX_TEST_EXPECT(s == s);
        NSFX_TEST_EXPECT(s <= s);
        NSFX_TEST_EXPECT(s >= s);
        NSFX_TEST_EXPECT(!(s < s));
        NSFX_TEST_EXPECT(!(s > s));
        S n = S(1);
        s = S(1 + G);
        NSFX_TEST_EXPECT(n != s);
        NSFX_TEST_EXPECT(n <= s);
        NSFX_TEST_EXPECT(n < s);
        NSFX_TEST_EXPECT(s > n);
        NSFX_TEST_EXPECT(s >= n);
        ++s;
        NSFX_TEST_EXPECT(n != s);
        NSFX_TEST_EXPECT(n >= s);
        NSFX_TEST_EXPECT(n > s);
        NSFX_TEST_EXPECT(s < n);
        NSFX_TEST_EXPECT(s <= n);
    }

    NSFX_TEST_CASE(64)
    {
        typedef nsfx::CircularSequenceNumber<64> S;
        typedef nsfx::CircularSequenceNumberTraits<64>::ValueType V;
        V M = nsfx::CircularSequenceNumberTraits<64>::MAX_VALUE;
        V G = nsfx::CircularSequenceNumberTraits<64>::MAX_GAP;
        S s(M);
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(M));
        NSFX_TEST_EXPECT_EQ(s++, static_cast<V>(M));
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(0));
        s = S(M);
        NSFX_TEST_EXPECT_EQ(++s, static_cast<V>(0));
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(0));
        s = S(0);
        NSFX_TEST_EXPECT_EQ(s--, static_cast<V>(0));
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(M));
        s = S(0);
        NSFX_TEST_EXPECT_EQ(--s, static_cast<V>(M));
        NSFX_TEST_EXPECT_EQ(s, static_cast<V>(M));
        NSFX_TEST_EXPECT(s == s);
        NSFX_TEST_EXPECT(s <= s);
        NSFX_TEST_EXPECT(s >= s);
        NSFX_TEST_EXPECT(!(s < s));
        NSFX_TEST_EXPECT(!(s > s));
        S n = S(1);
        s = S(1 + G);
        NSFX_TEST_EXPECT(n != s);
        NSFX_TEST_EXPECT(n <= s);
        NSFX_TEST_EXPECT(n < s);
        NSFX_TEST_EXPECT(s > n);
        NSFX_TEST_EXPECT(s >= n);
        ++s;
        NSFX_TEST_EXPECT(n != s);
        NSFX_TEST_EXPECT(n >= s);
        NSFX_TEST_EXPECT(n > s);
        NSFX_TEST_EXPECT(s < n);
        NSFX_TEST_EXPECT(s <= n);
    }

    NSFX_TEST_CASE(Hash)
    {
        typedef nsfx::CircularSequenceNumber<23> S;
        S s = S(1234);
        size_t h = boost::hash<S>()(s);
    }

    NSFX_TEST_CASE(Swap)
    {
        typedef nsfx::CircularSequenceNumber<23> S;
        S s0 = S(1234);
        S s1 = S(5678);
        swap(s0, s1);
        NSFX_TEST_EXPECT_EQ(s0, 5678);
        NSFX_TEST_EXPECT_EQ(s1, 1234);
    }

}/*}}}*/


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

