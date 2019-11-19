/**
 * @file
 *
 * @brief Test LollipopSequenceNumber.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    date
 *
 * @copyright Copyright (c) 2019.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/utility/lollipop-sequence-number.h>
#include <nsfx/network/buffer/buffer.h>
#include <iostream>


NSFX_TEST_SUITE(LollipopSequenceNumberTraits)/*{{{*/
{
    NSFX_TEST_CASE(1)
    {
        typedef nsfx::LollipopSequenceNumber<1>::TraitsType TraitsType;
        NSFX_TEST_EXPECT_EQ((TraitsType::ValueType)TraitsType::START_VALUE, 0x1);
        NSFX_TEST_EXPECT_EQ((TraitsType::ValueType)TraitsType::MAX_VALUE,   0x1);
        NSFX_TEST_EXPECT_EQ((TraitsType::ValueType)TraitsType::GAP_VALUE,   0x0);
        NSFX_TEST_EXPECT((std::is_same<typename TraitsType::ValueType,
                          nsfx::uint8_t>::value));
    }

    NSFX_TEST_CASE(2)
    {
        typedef nsfx::LollipopSequenceNumber<2>::TraitsType TraitsType;
        NSFX_TEST_EXPECT_EQ((TraitsType::ValueType)TraitsType::START_VALUE, 0x2);
        NSFX_TEST_EXPECT_EQ((TraitsType::ValueType)TraitsType::MAX_VALUE,   0x03);
        NSFX_TEST_EXPECT_EQ((TraitsType::ValueType)TraitsType::GAP_VALUE,   0x00);
        NSFX_TEST_EXPECT((std::is_same<typename TraitsType::ValueType,
                          nsfx::uint8_t>::value));
    }

    NSFX_TEST_CASE(8)
    {
        typedef nsfx::LollipopSequenceNumber<8>::TraitsType TraitsType;
        NSFX_TEST_EXPECT_EQ((TraitsType::ValueType)TraitsType::START_VALUE, 0x80);
        NSFX_TEST_EXPECT_EQ((TraitsType::ValueType)TraitsType::MAX_VALUE, 0xFF);
        NSFX_TEST_EXPECT_EQ((TraitsType::ValueType)TraitsType::GAP_VALUE, 0x3F);
        NSFX_TEST_EXPECT((std::is_same<typename TraitsType::ValueType,
                          nsfx::uint8_t>::value));
    }

    NSFX_TEST_CASE(9)
    {
        typedef nsfx::LollipopSequenceNumber<9>::TraitsType TraitsType;
        NSFX_TEST_EXPECT_EQ((TraitsType::ValueType)TraitsType::START_VALUE, 0x0100);
        NSFX_TEST_EXPECT_EQ((TraitsType::ValueType)TraitsType::MAX_VALUE,   0x01FF);
        NSFX_TEST_EXPECT_EQ((TraitsType::ValueType)TraitsType::GAP_VALUE,   0x007F);
        NSFX_TEST_EXPECT((std::is_same<typename TraitsType::ValueType,
                          nsfx::uint16_t>::value));
    }

    NSFX_TEST_CASE(16)
    {
        typedef nsfx::LollipopSequenceNumber<16>::TraitsType TraitsType;
        NSFX_TEST_EXPECT_EQ((TraitsType::ValueType)TraitsType::START_VALUE, 0x8000);
        NSFX_TEST_EXPECT_EQ((TraitsType::ValueType)TraitsType::MAX_VALUE,   0xFFFF);
        NSFX_TEST_EXPECT_EQ((TraitsType::ValueType)TraitsType::GAP_VALUE,   0x3FFF);
        NSFX_TEST_EXPECT((std::is_same<typename TraitsType::ValueType,
                          nsfx::uint16_t>::value));
    }

    NSFX_TEST_CASE(17)
    {
        typedef nsfx::LollipopSequenceNumber<17>::TraitsType TraitsType;
        NSFX_TEST_EXPECT_EQ((TraitsType::ValueType)TraitsType::START_VALUE, 0x00010000U);
        NSFX_TEST_EXPECT_EQ((TraitsType::ValueType)TraitsType::MAX_VALUE,   0x0001FFFFU);
        NSFX_TEST_EXPECT_EQ((TraitsType::ValueType)TraitsType::GAP_VALUE,   0x00007FFFU);
        NSFX_TEST_EXPECT((std::is_same<typename TraitsType::ValueType,
                          nsfx::uint32_t>::value));
    }

    NSFX_TEST_CASE(32)
    {
        typedef nsfx::LollipopSequenceNumber<32>::TraitsType TraitsType;
        NSFX_TEST_EXPECT_EQ((TraitsType::ValueType)TraitsType::START_VALUE, 0x80000000U);
        NSFX_TEST_EXPECT_EQ((TraitsType::ValueType)TraitsType::MAX_VALUE,   0xFFFFFFFFU);
        NSFX_TEST_EXPECT_EQ((TraitsType::ValueType)TraitsType::GAP_VALUE,   0x3FFFFFFFU);
        NSFX_TEST_EXPECT((std::is_same<typename TraitsType::ValueType,
                          nsfx::uint32_t>::value));
    }

    NSFX_TEST_CASE(33)
    {
        typedef nsfx::LollipopSequenceNumber<33>::TraitsType TraitsType;
        NSFX_TEST_EXPECT_EQ((TraitsType::ValueType)TraitsType::START_VALUE, 0x0000000100000000U);
        NSFX_TEST_EXPECT_EQ((TraitsType::ValueType)TraitsType::MAX_VALUE,   0x00000001FFFFFFFFU);
        NSFX_TEST_EXPECT_EQ((TraitsType::ValueType)TraitsType::GAP_VALUE,   0x000000007FFFFFFFU);
        NSFX_TEST_EXPECT((std::is_same<typename TraitsType::ValueType,
                          nsfx::uint64_t>::value));
    }

    NSFX_TEST_CASE(64)
    {
        typedef nsfx::LollipopSequenceNumber<64>::TraitsType TraitsType;
        NSFX_TEST_EXPECT_EQ((TraitsType::ValueType)TraitsType::START_VALUE, 0x8000000000000000U);
        NSFX_TEST_EXPECT_EQ((TraitsType::ValueType)TraitsType::MAX_VALUE,   0xFFFFFFFFFFFFFFFFU);
        NSFX_TEST_EXPECT_EQ((TraitsType::ValueType)TraitsType::GAP_VALUE,   0x3FFFFFFFFFFFFFFFU);
        NSFX_TEST_EXPECT((std::is_same<typename TraitsType::ValueType,
                          nsfx::uint64_t>::value));
    }

}/*}}}*/


NSFX_TEST_SUITE(LollipopSequenceNumber)/*{{{*/
{
    NSFX_TEST_CASE(1)
    {
        typedef nsfx::LollipopSequenceNumber<1> S;
        typedef S::TraitsType TraitsType;
        typedef TraitsType::ValueType V;
        S s(0);
        NSFX_TEST_EXPECT_EQ(s, S(0));
        NSFX_TEST_EXPECT_EQ(s++, S(0));
        NSFX_TEST_EXPECT_EQ(s, S(1));
        s = S(1);
        NSFX_TEST_EXPECT_EQ(++s, S(1));
        NSFX_TEST_EXPECT_EQ(s, S(1));
        s = S(0);
        NSFX_TEST_EXPECT(s == s);
        NSFX_TEST_EXPECT(!(s != s));
        NSFX_TEST_EXPECT(s <= s);
        NSFX_TEST_EXPECT(s >= s);
        NSFX_TEST_EXPECT(!(s < s));
        NSFX_TEST_EXPECT(!(s > s));
        s = S(1);
        NSFX_TEST_EXPECT(s == s);
        NSFX_TEST_EXPECT(!(s != s));
        NSFX_TEST_EXPECT(s <= s);
        NSFX_TEST_EXPECT(s >= s);
        NSFX_TEST_EXPECT(!(s < s));
        NSFX_TEST_EXPECT(!(s > s));
        NSFX_TEST_EXPECT(S(0) < S(1));
        NSFX_TEST_EXPECT(S(1) > S(0));
        NSFX_TEST_EXPECT(!(S(1) < S(0)));
        NSFX_TEST_EXPECT(!(S(0) > S(1)));
    }

    NSFX_TEST_CASE(2)
    {
        typedef nsfx::LollipopSequenceNumber<2> S;
        typedef S::TraitsType TraitsType;
        typedef TraitsType::ValueType V;
        V F = TraitsType::START_VALUE;
        V M = TraitsType::MAX_VALUE;
        V G = TraitsType::GAP_VALUE;
        S s(0);
        NSFX_TEST_EXPECT_EQ(s++, S(0));
        NSFX_TEST_EXPECT_EQ(s, S(1));
        s = S(1);
        NSFX_TEST_EXPECT_EQ(++s, S(2));
        s = S(2);
        NSFX_TEST_EXPECT_EQ(++s, S(3));
        s = S(3);
        NSFX_TEST_EXPECT_EQ(++s, S(2));
        s = S(0);
        NSFX_TEST_EXPECT(s == s);
        NSFX_TEST_EXPECT(!(s != s));
        NSFX_TEST_EXPECT(s <= s);
        NSFX_TEST_EXPECT(s >= s);
        NSFX_TEST_EXPECT(!(s < s));
        NSFX_TEST_EXPECT(!(s > s));
        s = S(1);
        NSFX_TEST_EXPECT(s == s);
        NSFX_TEST_EXPECT(!(s != s));
        NSFX_TEST_EXPECT(s <= s);
        NSFX_TEST_EXPECT(s >= s);
        NSFX_TEST_EXPECT(!(s < s));
        NSFX_TEST_EXPECT(!(s > s));
        //
        NSFX_TEST_EXPECT(S(0) < S(1));
        NSFX_TEST_EXPECT(S(0) <= S(1));
        NSFX_TEST_EXPECT(S(1) > S(0));
        NSFX_TEST_EXPECT(S(1) >= S(0));
        //
        NSFX_TEST_EXPECT(S(1) < S(2));
        NSFX_TEST_EXPECT(S(1) <= S(2));
        NSFX_TEST_EXPECT(S(2) > S(1));
        NSFX_TEST_EXPECT(S(2) >= S(1));
        //
        NSFX_TEST_EXPECT(S(2) < S(3));
        NSFX_TEST_EXPECT(S(2) <= S(3));
        NSFX_TEST_EXPECT(S(2) > S(3));
        NSFX_TEST_EXPECT(S(2) >= S(3));
        //
        NSFX_TEST_EXPECT(S(3) < S(2));
        NSFX_TEST_EXPECT(S(3) <= S(2));
        NSFX_TEST_EXPECT(S(3) > S(2));
        NSFX_TEST_EXPECT(S(3) >= S(2));
    }

    NSFX_TEST_CASE(8)
    {
        typedef nsfx::LollipopSequenceNumber<8> S;
        typedef S::TraitsType TraitsType;
        typedef TraitsType::ValueType V;
        V F = TraitsType::START_VALUE;
        V M = TraitsType::MAX_VALUE;
        V G = TraitsType::GAP_VALUE;
        S s(M);
        NSFX_TEST_EXPECT_EQ(s++, S(M));
        NSFX_TEST_EXPECT_EQ(s, S(F));
        s = S(M);
        NSFX_TEST_EXPECT_EQ(++s, S(F));
        S n(F);
        s = S(F+G);
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

    // bits = 16, start = 1.
    NSFX_TEST_CASE(16_1)
    {
        typedef nsfx::LollipopSequenceNumber<16, 1> S;
        typedef S::TraitsType TraitsType;
        typedef TraitsType::ValueType V;
        V F = TraitsType::START_VALUE;
        V M = TraitsType::MAX_VALUE;
        V G = TraitsType::GAP_VALUE;
        NSFX_TEST_EXPECT_EQ((TraitsType::ValueType)TraitsType::START_VALUE, 1);
        NSFX_TEST_EXPECT_EQ((TraitsType::ValueType)TraitsType::MAX_VALUE,   0xFFFF);
        NSFX_TEST_EXPECT_EQ((TraitsType::ValueType)TraitsType::GAP_VALUE,   0x7FFF);
        S s(0);
        NSFX_TEST_EXPECT_EQ(s++, S(0));
        NSFX_TEST_EXPECT_EQ(s, S(1));
        s = S(M);
        NSFX_TEST_EXPECT_EQ(++s, S(F));
        //
        NSFX_TEST_EXPECT(S(0) < S(F));
        NSFX_TEST_EXPECT(S(0) < S(M));
        NSFX_TEST_EXPECT(S(F) > S(0));
        NSFX_TEST_EXPECT(S(M) > S(0));
        S n(F);
        s = S(F+G);
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

    // bits = 32, start = 0
    NSFX_TEST_CASE(32_0)
    {
        typedef nsfx::LollipopSequenceNumber<32, 0> S;
        typedef S::TraitsType TraitsType;
        typedef TraitsType::ValueType V;
        V F = TraitsType::START_VALUE;
        V M = TraitsType::MAX_VALUE;
        V G = TraitsType::GAP_VALUE;
        NSFX_TEST_EXPECT_EQ((TraitsType::ValueType)TraitsType::START_VALUE, 0);
        NSFX_TEST_EXPECT_EQ((TraitsType::ValueType)TraitsType::MAX_VALUE,   0xFFFFFFFF);
        NSFX_TEST_EXPECT_EQ((TraitsType::ValueType)TraitsType::GAP_VALUE,   0x7FFFFFFF);
        S s(0);
        NSFX_TEST_EXPECT_EQ(s++, S(0));
        NSFX_TEST_EXPECT_EQ(s, S(1));
        s = S(M);
        NSFX_TEST_EXPECT_EQ(++s, S(F));
        S n(F);
        s = S(F+G);
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
        typedef nsfx::LollipopSequenceNumber<64> S;
        typedef S::TraitsType TraitsType;
        typedef TraitsType::ValueType V;
        V F = TraitsType::START_VALUE;
        V M = TraitsType::MAX_VALUE;
        V G = TraitsType::GAP_VALUE;
        S s(M);
        NSFX_TEST_EXPECT_EQ(s++, S(M));
        NSFX_TEST_EXPECT_EQ(s, S(F));
        s = S(M);
        NSFX_TEST_EXPECT_EQ(++s, S(F));
        S n(F);
        s = S(F+G);
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

    NSFX_TEST_CASE(Swap)
    {
        typedef nsfx::LollipopSequenceNumber<23> S;
        S s0(0x12345);
        S s1(0x6789a);
        swap(s0, s1);
        NSFX_TEST_EXPECT_EQ(s0, S(0x6789a));
        NSFX_TEST_EXPECT_EQ(s1, S(0x12345));
    }

    NSFX_TEST_CASE(Hash)
    {
        typedef nsfx::LollipopSequenceNumber<23> S;
        S s(0x12345);
        size_t h = boost::hash<S>()(s);
    }

    NSFX_TEST_CASE(IO32)
    {
        typedef nsfx::LollipopSequenceNumber<23> S;
        S s0(0x12345);
        S s1;
        nsfx::Buffer buffer;
        NSFX_TEST_EXPECT_EQ(S::GetSize(), 3);
        buffer.AddAtStart(S::GetSize());
        auto it = buffer.begin();
        ////////////////////
        s0.Write(it);
        it -= 3;
        s1.Read(it);
        it -= 3;
        NSFX_TEST_EXPECT_EQ(s0, S(0x12345));
        s0.WriteL(it);
        it -= 3;
        s1.ReadL(it);
        it -= 3;
        NSFX_TEST_EXPECT_EQ(s0, S(0x12345));
        s0.WriteB(it);
        it -= 3;
        s1.ReadB(it);
        it -= 3;
        NSFX_TEST_EXPECT_EQ(s0, S(0x12345));
        ////////////////////
        Write(it, s0);
        it -= 3;
        Read(it, &s1);
        it -= 3;
        NSFX_TEST_EXPECT_EQ(s0, S(0x12345));
        WriteL(it, s0);
        it -= 3;
        ReadL(it, &s1);
        it -= 3;
        NSFX_TEST_EXPECT_EQ(s0, S(0x12345));
        WriteB(it, s0);
        it -= 3;
        ReadB(it, &s1);
        it -= 3;
        NSFX_TEST_EXPECT_EQ(s0, S(0x12345));
    }

    NSFX_TEST_CASE(IO64)
    {
        typedef nsfx::LollipopSequenceNumber<55> S;
        S s0(0x123456789abcdef);
        S s1;
        nsfx::Buffer buffer;
        NSFX_TEST_EXPECT_EQ(S::GetSize(), 7);
        buffer.AddAtStart(S::GetSize());
        auto it = buffer.begin();
        ////////////////////
        s0.Write(it);
        it -= 7;
        s1.Read(it);
        it -= 7;
        NSFX_TEST_EXPECT_EQ(s0, S(0x123456789abcdef));
        s0.WriteL(it);
        it -= 7;
        s1.ReadL(it);
        it -= 7;
        NSFX_TEST_EXPECT_EQ(s0, S(0x123456789abcdef));
        s0.WriteB(it);
        it -= 7;
        s1.ReadB(it);
        it -= 7;
        NSFX_TEST_EXPECT_EQ(s0, S(0x123456789abcdef));
        ////////////////////
        Write(it, s0);
        it -= 7;
        Read(it, &s1);
        it -= 7;
        NSFX_TEST_EXPECT_EQ(s0, S(0x123456789abcdef));
        WriteL(it, s0);
        it -= 7;
        ReadL(it, &s1);
        it -= 7;
        NSFX_TEST_EXPECT_EQ(s0, S(0x123456789abcdef));
        WriteB(it, s0);
        it -= 7;
        ReadB(it, &s1);
        it -= 7;
        NSFX_TEST_EXPECT_EQ(s0, S(0x123456789abcdef));
    }

}/*}}}*/


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cerr);
    nsfx::test::runner::Run();

    return 0;
}

