/**
 * @file
 *
 * @brief Test Address.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2019-03-24
 *
 * @copyright Copyright (c) 2019.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/network/address.h>
#include <iostream>


NSFX_TEST_SUITE(Address)
{
    NSFX_TEST_SUITE(28)/*{{{*/
    {
        typedef nsfx::Address<28>  Address;

        NSFX_TEST_SUITE(Ctor)
        {
            NSFX_TEST_CASE(0)
            {
                NSFX_TEST_EXPECT_EQ((size_t)(Address::NUM_BITS),  28);
                NSFX_TEST_EXPECT_EQ(Address::GetBitSize(), 28);

                NSFX_TEST_EXPECT_EQ((size_t)(Address::NUM_BYTES), 4);
                NSFX_TEST_EXPECT_EQ(Address::GetSize(), 4);

                NSFX_TEST_EXPECT_EQ(Address::Zero(), Address());
                NSFX_TEST_EXPECT_EQ(Address::Mask(), Address(-1L));

                {
                    Address a;
                    NSFX_TEST_EXPECT(a.IsZero());
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000000");
                }
            }

            NSFX_TEST_CASE(8)
            {
                // uint8_t array, shorter than addr
                {
                    uint8_t x[] = { 0x12, 0x34, 0x56 };
                    Address a(x, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00123456");
                }

                {
                    uint8_t x[] = { 0x12, 0x34, 0x56 };
                    Address a(x, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00563412");
                }

                // uint8_t array, longer than addr
                {
                    uint8_t x[] = { 0x12, 0x34, 0x56, 0x78, 0x9a };
                    Address a(x, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0456789a");
                }

                {
                    uint8_t x[] = { 0x12, 0x34, 0x56, 0x78, 0x9a };
                    Address a(x, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "08563412");
                }

                // uint8_t buffer, shorter than addr
                {
                    uint8_t x[] = { 0x12, 0x34, 0x56 };
                    Address a(x, sizeof (x), nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00123456");
                }

                {
                    uint8_t x[] = { 0x12, 0x34, 0x56 };
                    Address a(x, sizeof (x), nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00563412");
                }

                // uint8_t buffer, longer than addr
                {
                    uint8_t x[] = { 0x12, 0x34, 0x56, 0x78, 0x9a };
                    Address a(x, sizeof (x), nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0456789a");
                }

                {
                    uint8_t x[] = { 0x12, 0x34, 0x56, 0x78, 0x9a };
                    Address a(x, sizeof (x), nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "08563412");
                }

                // uint8_t pack, shorter than addr
                {
                    Address a((uint8_t)0x12, (uint8_t)0x34,
                              (uint8_t)0x56, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00123456");
                }

                {
                    Address a((uint8_t)0x12, (uint8_t)0x34,
                              (uint8_t)0x56, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00563412");
                }

                // uint8_t pack, longer than addr
                {
                    Address a((uint8_t)0x12, (uint8_t)0x34,
                              (uint8_t)0x56, (uint8_t)0x78,
                              (uint8_t)0x9a, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0456789a");
                }

                {
                    Address a((uint8_t)0x12, (uint8_t)0x34,
                              (uint8_t)0x56, (uint8_t)0x78,
                              (uint8_t)0x9a, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "08563412");
                }

                // uint8_t
                {
                    uint8_t x = 0x12;
                    Address a(x);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000012");
                }

                // int8_t
                {
                    int8_t x = (int8_t)0xf1;
                    Address a(x);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0ffffff1");
                }
            }

            NSFX_TEST_CASE(16)
            {
                // uint16_t array, shorter than addr
                {
                    uint16_t x[] = { 0x1234 };
                    Address a(x, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00001234");
                }

                {
                    uint16_t x[] = { 0x1234 };
                    Address a(x, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00001234");
                }

                // uint16_t array, longer than addr
                {
                    uint16_t x[] = { 0x1234, 0x5678, 0x9abc };
                    Address a(x, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "06789abc");
                }

                {
                    uint16_t x[] = { 0x1234, 0x5678, 0x9abc };
                    Address a(x, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "06781234");
                }

                // uint16_t buffer, shorter than addr
                {
                    uint16_t x[] = { 0x1234 };
                    Address a(x, 1, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00001234");
                }

                {
                    uint16_t x[] = { 0x1234 };
                    Address a(x, 1, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00001234");
                }

                // uint16_t buffer, longer than addr
                {
                    uint16_t x[] = { 0x1234, 0x5678, 0x9abc };
                    Address a(x, 3, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "06789abc");
                }

                {
                    uint16_t x[] = { 0x1234, 0x5678, 0x9abc };
                    Address a(x, 3, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "06781234");
                }

                // uint16_t pack, shorter than addr
                {
                    Address a((uint16_t)0x1234, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00001234");
                }

                {
                    Address a((uint16_t)0x1234, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00001234");
                }

                // uint16_t pack, longer than addr
                {
                    Address a((uint16_t)0x1234, (uint16_t)0x5678,
                              (uint16_t)0x9abc, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "06789abc");
                }

                {
                    Address a((uint16_t)0x1234, (uint16_t)0x5678,
                              (uint16_t)0x9abc, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "06781234");
                }

                // uint16_t
                {
                    uint16_t x = 0x1234;
                    Address a(x);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00001234");
                }

                // int16_t
                {
                    int16_t x = (int16_t)0x8765;
                    Address a(x);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0fff8765");
                }
            }

            NSFX_TEST_CASE(32)
            {
                // uint32_t array, longer than addr
                {
                    uint32_t x[] = { 0x12345678, 0x9abcdef1 };
                    Address a(x, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0abcdef1");
                }

                {
                    uint32_t x[] = { 0x12345678, 0x9abcdef1 };
                    Address a(x, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "02345678");
                }

                // uint32_t buffer, longer than addr
                {
                    uint32_t x[] = { 0x12345678, 0x9abcdef1 };
                    Address a(x, 2, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0abcdef1");
                }

                {
                    uint32_t x[] = { 0x12345678, 0x9abcdef1 };
                    Address a(x, 2, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "02345678");
                }

                // uint32_t pack, longer than addr
                {
                    Address a(0x12345678, 0x9abcdef1, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0abcdef1");
                }

                {
                    Address a(0x12345678, 0x9abcdef1, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "02345678");
                }

                // uint32_t
                {
                    uint32_t x = 0x12345678;
                    Address a(x);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "02345678");
                }

                // int32_t
                {
                    int32_t x = 0x87654321;
                    Address a(x);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "07654321");
                }
            }

            NSFX_TEST_CASE(64)
            {
                // uint64_t array, longer than addr
                {
                    uint64_t x[] = { 0x123456789abcdef1, 0x1fedcba987654321 };
                    Address a(x, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "07654321");
                }

                {
                    uint64_t x[] = { 0x123456789abcdef1, 0x1fedcba987654321 };
                    Address a(x, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0abcdef1");
                }

                // uint64_t buffer, longer than addr
                {
                    uint64_t x[] = { 0x123456789abcdef1, 0x1fedcba987654321 };
                    Address a(x, 2, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "07654321");
                }

                {
                    uint64_t x[] = { 0x123456789abcdef1, 0x1fedcba987654321 };
                    Address a(x, 2, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0abcdef1");
                }

                // uint64_t pack, longer than addr
                {
                    Address a(0x123456789abcdef1ULL,
                              0x1fedcba987654321ULL, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "07654321");
                }

                {
                    Address a(0x123456789abcdef1ULL,
                              0x1fedcba987654321ULL, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0abcdef1");
                }

                // uint64_t
                {
                    uint64_t x = 0x123456789abcdef1;
                    Address a(x);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0abcdef1");
                }

                // int64_t
                {
                    int64_t x = 0xf123456789abcdef;
                    Address a(x);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "09abcdef");
                }
            }
        }

        NSFX_TEST_CASE(Comparison)
        {
            Address a(0x02345678);
            Address b(0x09abcdef);

            NSFX_TEST_EXPECT(a == a);
            NSFX_TEST_EXPECT(!(a != a));

            NSFX_TEST_EXPECT(a != b);
            NSFX_TEST_EXPECT(b != a);

            NSFX_TEST_EXPECT(a < b);
            NSFX_TEST_EXPECT(a <= a);
            NSFX_TEST_EXPECT(a <= b);

            NSFX_TEST_EXPECT(b > a);
            NSFX_TEST_EXPECT(b >= a);
            NSFX_TEST_EXPECT(b >= b);
        }

        NSFX_TEST_SUITE(Arithmetics)
        {
            // ++
            NSFX_TEST_CASE(Increment)
            {
                Address a(0x0ffffffe);
                Address b(0x0fffffff);
                Address c(0x00000000);
                NSFX_TEST_EXPECT_EQ(++a, b);
                NSFX_TEST_EXPECT_EQ(a++, b);
                NSFX_TEST_EXPECT_EQ(a, c);
            }

            // --
            NSFX_TEST_CASE(Decrement)
            {
                Address a(0x00000000);
                Address b(0x0fffffff);
                Address c(0x0ffffffe);
                NSFX_TEST_EXPECT_EQ(--a, b);
                NSFX_TEST_EXPECT_EQ(a--, b);
                NSFX_TEST_EXPECT_EQ(a, c);
            }

            // +
            NSFX_TEST_CASE(Mirror)
            {
                Address a(0x0ffffff0);
                NSFX_TEST_EXPECT_EQ(+a, a);
            }

            // -
            NSFX_TEST_CASE(Negate)
            {
                Address a(0x0ffffff0);
                NSFX_TEST_EXPECT_EQ(-a, Address(0x00000010));
            }

            // +
            NSFX_TEST_CASE(Plus)
            {
                Address a              (0x0ffffff0);
                NSFX_TEST_EXPECT_EQ(a + 0x1000000f, Address(0x0fffffff));
                NSFX_TEST_EXPECT_EQ(a + 0x100000ff, Address(0x000000ef));
                NSFX_TEST_EXPECT_EQ(0x100000ff + a, Address(0x000000ef));
                NSFX_TEST_EXPECT_EQ(a +=0x100000ff, Address(0x000000ef));
            }

            // -
            NSFX_TEST_CASE(Minus)
            {
                Address a              (0x000000ff);
                NSFX_TEST_EXPECT_EQ(a - 0x1000000f, Address(0x000000f0));
                NSFX_TEST_EXPECT_EQ(a - 0x10000ff0, Address(0x0ffff10f));
                NSFX_TEST_EXPECT_EQ(a -=0x10000ff0, Address(0x0ffff10f));
            }

            // -
            NSFX_TEST_CASE(Difference)
            {
                // 0
                Address a(0x08ffffff);
                NSFX_TEST_EXPECT_EQ(a - a, 0);

                // +
                Address b(0x0000000f);
                NSFX_TEST_EXPECT_EQ(a - b, 0x0000000008fffff0LL);

                // -
                NSFX_TEST_EXPECT_EQ(b - a, 0x00000000f7000010LL);
            }

            // *
            NSFX_TEST_CASE(Multiply)
            {
                Address a(0x00ffffff);
                NSFX_TEST_EXPECT_EQ(a * 0, Address(0));
                NSFX_TEST_EXPECT_EQ(a * 1, Address(0x00ffffff));
                NSFX_TEST_EXPECT_EQ(a * 2, Address(0x01fffffe));
                NSFX_TEST_EXPECT_EQ(a * 0xff, Address(0x0effff01));
                NSFX_TEST_EXPECT_EQ(0xff * a, Address(0x0effff01));
                NSFX_TEST_EXPECT_EQ(a *=0xff, Address(0x0effff01));
            }

            // ~
            NSFX_TEST_CASE(Not)
            {
                Address a(0x00ffffff);
                NSFX_TEST_EXPECT_EQ(~a, Address(0x0f000000));
            }

            // &
            NSFX_TEST_CASE(And)
            {
                Address a(0x01234567);
                Address b(0x0000ffff);
                NSFX_TEST_EXPECT_EQ(a & b, Address(0x00004567));
                NSFX_TEST_EXPECT_EQ(a &=b, Address(0x00004567));
            }

            // |
            NSFX_TEST_CASE(Or)
            {
                Address a(0x01230000);
                Address b(0x00004567);
                NSFX_TEST_EXPECT_EQ(a | b, Address(0x01234567));
                NSFX_TEST_EXPECT_EQ(a |=b, Address(0x01234567));
            }

            // ^
            NSFX_TEST_CASE(Xor)
            {
                Address a(0x01234567);
                Address b(0x0000ffff);
                NSFX_TEST_EXPECT_EQ(a ^ b, Address(0x0123ba98));
                NSFX_TEST_EXPECT_EQ(a ^=b, Address(0x0123ba98));
            }

            // <<
            NSFX_TEST_CASE(LeftShift)
            {
                Address a(0x00ffffff);
                NSFX_TEST_EXPECT_EQ(a << 0, a);
                NSFX_TEST_EXPECT_EQ(a << 28,  Address(0));
                NSFX_TEST_EXPECT_EQ(a << 900, Address(0));
                NSFX_TEST_EXPECT_EQ(a << 1,   Address(0x01fffffe));
                NSFX_TEST_EXPECT_EQ(a << 15,  Address(0x0fff8000));
                NSFX_TEST_EXPECT_EQ(a <<=15,  Address(0x0fff8000));
            }

            // >>
            NSFX_TEST_CASE(RightShift)
            {
                Address a(0x0ffffff0);
                NSFX_TEST_EXPECT_EQ(a >> 0, a);
                NSFX_TEST_EXPECT_EQ(a >> 28,  Address(0));
                NSFX_TEST_EXPECT_EQ(a >> 900, Address(0));
                NSFX_TEST_EXPECT_EQ(a >> 1,   Address(0x07fffff8));
                NSFX_TEST_EXPECT_EQ(a >> 15,  Address(0x00001fff));
                NSFX_TEST_EXPECT_EQ(a >>=15,  Address(0x00001fff));
            }

            // bool
            NSFX_TEST_CASE(Bool)
            {
                Address a;
                Address b(0x0000ffff);
                NSFX_TEST_EXPECT(!a);
                NSFX_TEST_EXPECT(b);
                NSFX_TEST_EXPECT(!!b);
            }

            // hash
            NSFX_TEST_CASE(Hash)
            {
                Address a;
                Address b(0x0000ffff);
                NSFX_TEST_EXPECT_NE(boost::hash<Address>()(a),
                                    boost::hash<Address>()(b));
            }

            // swap
            NSFX_TEST_CASE(Swap)
            {
                Address a;
                Address b(0x0000ffff);
                boost::swap(a, b);
                NSFX_TEST_EXPECT_EQ(a, Address(0x0000ffff));
                NSFX_TEST_EXPECT_EQ(b, Address());
            }
        }

    }/*}}}*/

    NSFX_TEST_SUITE(52)/*{{{*/
    {
        typedef nsfx::Address<52>  Address;

        NSFX_TEST_SUITE(Ctor)
        {
            NSFX_TEST_CASE(0)
            {
                NSFX_TEST_EXPECT_EQ((size_t)(Address::NUM_BITS),  52);
                NSFX_TEST_EXPECT_EQ(Address::GetBitSize(), 52);

                NSFX_TEST_EXPECT_EQ((size_t)(Address::NUM_BYTES), 7);
                NSFX_TEST_EXPECT_EQ(Address::GetSize(), 7);

                NSFX_TEST_EXPECT_EQ(Address::Zero(), Address());
                NSFX_TEST_EXPECT_EQ(Address::Mask(), Address(-1L));
                NSFX_TEST_EXPECT_EQ(Address::Mask().ToString(), "0fffffffffffff");

                {
                    Address a;
                    NSFX_TEST_EXPECT(a.IsZero());
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000000000000");
                }
            }

            NSFX_TEST_CASE(8)
            {
                // uint8_t array, shorter than addr
                {
                    uint8_t x[] = { 0x12, 0x34, 0x56 };
                    Address a(x, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000000123456");
                }

                {
                    uint8_t x[] = { 0x12, 0x34, 0x56 };
                    Address a(x, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000000563412");
                }

                // uint8_t array, longer than addr
                {
                    uint8_t x[] = { 0x12, 0x34, 0x56, 0x78,
                                    0x9a, 0xbc, 0xde, 0xf1 };
                    Address a(x, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0456789abcdef1");
                }

                {
                    uint8_t x[] = { 0x12, 0x34, 0x56, 0x78,
                                    0x9a, 0xbc, 0xde, 0xf1 };
                    Address a(x, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0ebc9a78563412");
                }

                // uint8_t buffer, shorter than addr
                {
                    uint8_t x[] = { 0x12, 0x34, 0x56 };
                    Address a(x, sizeof (x), nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000000123456");
                }

                {
                    uint8_t x[] = { 0x12, 0x34, 0x56 };
                    Address a(x, sizeof (x), nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000000563412");
                }

                // uint8_t buffer, longer than addr
                {
                    uint8_t x[] = { 0x12, 0x34, 0x56, 0x78,
                                    0x9a, 0xbc, 0xde, 0xf1 };
                    Address a(x, sizeof (x), nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0456789abcdef1");
                }

                {
                    uint8_t x[] = { 0x12, 0x34, 0x56, 0x78,
                                    0x9a, 0xbc, 0xde, 0xf1 };
                    Address a(x, sizeof (x), nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0ebc9a78563412");
                }

                // uint8_t pack, shorter than addr
                {
                    Address a((uint8_t)0x12, (uint8_t)0x34,
                              (uint8_t)0x56, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000000123456");
                }

                {
                    Address a((uint8_t)0x12, (uint8_t)0x34,
                              (uint8_t)0x56, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000000563412");
                }

                // uint8_t
                {
                    uint8_t x = 0x12;
                    Address a(x);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000000000012");
                }

                // int8_t
                {
                    int8_t x = (int8_t)0xf1;
                    Address a(x);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0ffffffffffff1");
                }
            }

            NSFX_TEST_CASE(16)
            {
                // uint16_t array, shorter than addr
                {
                    uint16_t x[] = { 0x1234, 0x5678 };
                    Address a(x, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000012345678");
                }

                {
                    uint16_t x[] = { 0x1234, 0x5678 };
                    Address a(x, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000056781234");
                }

                // uint16_t array, longer than addr
                {
                    uint16_t x[] = { 0x1234, 0x5678, 0x9abc, 0xdef1 };
                    Address a(x, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0456789abcdef1");
                }

                {
                    uint16_t x[] = { 0x1234, 0x5678, 0x9abc, 0xdef1 };
                    Address a(x, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "019abc56781234");
                }

                // uint16_t buffer, shorter than addr
                {
                    uint16_t x[] = { 0x1234, 0x5678 };
                    Address a(x, 2, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000012345678");
                }

                {
                    uint16_t x[] = { 0x1234, 0x5678 };
                    Address a(x, 2, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000056781234");
                }

                // uint16_t buffer, longer than addr
                {
                    uint16_t x[] = { 0x1234, 0x5678, 0x9abc, 0xdef1 };
                    Address a(x, 4, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0456789abcdef1");
                }

                {
                    uint16_t x[] = { 0x1234, 0x5678, 0x9abc, 0xdef1 };
                    Address a(x, 4, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "019abc56781234");
                }

                // uint16_t pack, shorter than addr
                {
                    Address a((uint16_t)0x1234, (uint16_t)0x5678,
                              nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000012345678");
                }

                {
                    Address a((uint16_t)0x1234, (uint16_t)0x5678,
                              nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000056781234");
                }

                // uint16_t pack, longer than addr
                {
                    Address a((uint16_t)0x1234, (uint16_t)0x5678,
                              (uint16_t)0x9abc, (uint16_t)0xdef1,
                              nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0456789abcdef1");
                }

                {
                    Address a((uint16_t)0x1234, (uint16_t)0x5678,
                              (uint16_t)0x9abc, (uint16_t)0xdef1,
                              nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "019abc56781234");
                }

                // uint16_t
                {
                    uint16_t x = 0x1234;
                    Address a(x);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000000001234");
                }

                // int16_t
                {
                    int16_t x = (int16_t)0x8765;
                    Address a(x);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0fffffffff8765");
                }
            }

            NSFX_TEST_CASE(32)
            {
                // uint32_t array, short than addr
                {
                    uint32_t x[] = { 0x12345678 };
                    Address a(x, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000012345678");
                }

                {
                    uint32_t x[] = { 0x12345678 };
                    Address a(x, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000012345678");
                }

                // uint32_t array, longer than addr
                {
                    uint32_t x[] = { 0x12345678, 0x9abcdef1 };
                    Address a(x, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0456789abcdef1");
                }

                {
                    uint32_t x[] = { 0x12345678, 0x9abcdef1 };
                    Address a(x, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0cdef112345678");
                }

                // uint32_t array, short than addr
                {
                    uint32_t x[] = { 0x12345678 };
                    Address a(x, 1, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000012345678");
                }

                {
                    uint32_t x[] = { 0x12345678 };
                    Address a(x, 1, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000012345678");
                }

                // uint32_t buffer, longer than addr
                {
                    uint32_t x[] = { 0x12345678, 0x9abcdef1 };
                    Address a(x, 2, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0456789abcdef1");
                }

                {
                    uint32_t x[] = { 0x12345678, 0x9abcdef1 };
                    Address a(x, 2, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0cdef112345678");
                }

                // uint32_t pack, short than addr
                {
                    Address a(0x12345678, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000012345678");
                }

                {
                    Address a(0x12345678, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000012345678");
                }

                // uint32_t pack, longer than addr
                {
                    Address a(0x12345678, 0x9abcdef1, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0456789abcdef1");
                }

                {
                    Address a(0x12345678, 0x9abcdef1, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0cdef112345678");
                }

                // uint32_t
                {
                    uint32_t x = 0x12345678;
                    Address a(x);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000012345678");
                }

                // int32_t
                {
                    int32_t x = 0x87654321;
                    Address a(x);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0fffff87654321");
                }
            }

            NSFX_TEST_CASE(64)
            {
                // uint64_t array, longer than addr
                {
                    uint64_t x[] = { 0x123456789abcdef1, 0x1fedcba987654321 };
                    Address a(x, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0dcba987654321");
                }

                {
                    uint64_t x[] = { 0x123456789abcdef1, 0x1fedcba987654321 };
                    Address a(x, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0456789abcdef1");
                }

                // uint64_t buffer, longer than addr
                {
                    uint64_t x[] = { 0x123456789abcdef1, 0x1fedcba987654321 };
                    Address a(x, 2, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0dcba987654321");
                }

                {
                    uint64_t x[] = { 0x123456789abcdef1, 0x1fedcba987654321 };
                    Address a(x, 2, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0456789abcdef1");
                }

                // uint64_t pack, longer than addr
                {
                    Address a(0x123456789abcdef1ULL,
                              0x1fedcba987654321ULL, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0dcba987654321");
                }

                {
                    Address a(0x123456789abcdef1ULL,
                              0x1fedcba987654321ULL, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0456789abcdef1");
                }

                // uint64_t
                {
                    uint64_t x = 0x123456789abcdef1;
                    Address a(x);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0456789abcdef1");
                }

                // int64_t
                {
                    int64_t x = 0xf123456789abcdef;
                    Address a(x);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "03456789abcdef");
                }
            }
        }

        NSFX_TEST_CASE(Comparison)
        {
            Address a(0x00000000000001ULL);
            Address b(0x0f000000000000ULL);

            NSFX_TEST_EXPECT(a == a);
            NSFX_TEST_EXPECT(!(a != a));

            NSFX_TEST_EXPECT(a != b);
            NSFX_TEST_EXPECT(b != a);

            NSFX_TEST_EXPECT(a < b);
            NSFX_TEST_EXPECT(a <= a);
            NSFX_TEST_EXPECT(a <= b);

            NSFX_TEST_EXPECT(b > a);
            NSFX_TEST_EXPECT(b >= a);
            NSFX_TEST_EXPECT(b >= b);
        }

        NSFX_TEST_SUITE(Arithmetics)
        {
            // ++
            NSFX_TEST_CASE(Increment)
            {
                Address a(0x0ffffffffffffeULL);
                Address b(0x0fffffffffffffULL);
                Address c(0x00000000000000ULL);
                NSFX_TEST_EXPECT_EQ(++a, b);
                NSFX_TEST_EXPECT_EQ(a++, b);
                NSFX_TEST_EXPECT_EQ(a, c);
            }

            // --
            NSFX_TEST_CASE(Decrement)
            {
                Address a(0x00000000000000ULL);
                Address b(0x0fffffffffffffULL);
                Address c(0x0ffffffffffffeULL);
                NSFX_TEST_EXPECT_EQ(--a, b);
                NSFX_TEST_EXPECT_EQ(a--, b);
                NSFX_TEST_EXPECT_EQ(a, c);
            }

            // +
            NSFX_TEST_CASE(Mirror)
            {
                Address a(0x0ffffffffffff0ULL);
                NSFX_TEST_EXPECT_EQ(+a, a);
            }

            // -
            NSFX_TEST_CASE(Negate)
            {
                Address a(0x0ffffffffffff0ULL);
                NSFX_TEST_EXPECT_EQ(-a, Address(0x00000000000010ULL));
            }

            // +
            NSFX_TEST_CASE(Plus)
            {
                Address a              (0x0ffffffffffff0ULL);
                NSFX_TEST_EXPECT_EQ(a + 0x1000000000000fULL, Address(0x0fffffffffffffULL));
                NSFX_TEST_EXPECT_EQ(a + 0x100000000000ffULL, Address(0x000000000000efULL));
                NSFX_TEST_EXPECT_EQ(0x100000000000ffULL + a, Address(0x000000000000efULL));
                NSFX_TEST_EXPECT_EQ(a +=0x100000000000ffULL, Address(0x000000000000efULL));
            }

            // -
            NSFX_TEST_CASE(Minus)
            {
                Address a              (0x000000000000ffULL);
                NSFX_TEST_EXPECT_EQ(a - 0x1000000000000fULL, Address(0x000000000000f0ULL));
                NSFX_TEST_EXPECT_EQ(a - 0x10000000000ff0ULL, Address(0x0ffffffffff10fULL));
                NSFX_TEST_EXPECT_EQ(a -=0x10000000000ff0ULL, Address(0x0ffffffffff10fULL));
            }

            // -
            NSFX_TEST_CASE(Difference)
            {
                // 0
                Address a(0x08ffffffffffffULL);
                NSFX_TEST_EXPECT_EQ(a - a, 0);

                // +
                Address b(0x0000000000000fULL);
                NSFX_TEST_EXPECT_EQ(a - b, 0x0008fffffffffff0LL);

                // -
                NSFX_TEST_EXPECT_EQ(b - a, 0xfff7000000000010LL);
            }

            // *
            NSFX_TEST_CASE(Multiply)
            {
                Address a(0x00ffffffffffffULL);
                NSFX_TEST_EXPECT_EQ(a * 0, Address(0));
                NSFX_TEST_EXPECT_EQ(a * 1, Address(0x00ffffffffffffULL));
                NSFX_TEST_EXPECT_EQ(a * 2, Address(0x01fffffffffffeULL));
                NSFX_TEST_EXPECT_EQ(a * 0xffU, Address(0x0effffffffff01ULL));
                NSFX_TEST_EXPECT_EQ(0xffU * a, Address(0x0effffffffff01ULL));
                NSFX_TEST_EXPECT_EQ(a *=0xffU, Address(0x0effffffffff01ULL));
            }

            // ~
            NSFX_TEST_CASE(Not)
            {
                Address a(0x00ffffffffffffULL);
                NSFX_TEST_EXPECT_EQ(~a, Address(0x0f000000000000ULL));
            }

            // &
            NSFX_TEST_CASE(And)
            {
                Address a(0x01234567abcdefULL);
                Address b(0x0000000fffffffULL);
                NSFX_TEST_EXPECT_EQ(a & b, Address(0x00000007abcdefULL));
                NSFX_TEST_EXPECT_EQ(a &=b, Address(0x00000007abcdefULL));
            }

            // |
            NSFX_TEST_CASE(Or)
            {
                Address a(0x01234560000000ULL);
                Address b(0x0000000789abcdULL);
                NSFX_TEST_EXPECT_EQ(a | b, Address(0x0123456789abcdULL));
                NSFX_TEST_EXPECT_EQ(a |=b, Address(0x0123456789abcdULL));
            }

            // ^
            NSFX_TEST_CASE(Xor)
            {
                Address a(0x0123456789abcdULL);
                Address b(0x0000000fffffffULL);
                NSFX_TEST_EXPECT_EQ(a ^ b, Address(0x01234568765432ULL));
                NSFX_TEST_EXPECT_EQ(a ^=b, Address(0x01234568765432ULL));
            }

            // <<
            NSFX_TEST_CASE(LeftShift)
            {
                Address a(0x00ffffffffffffULL);
                NSFX_TEST_EXPECT_EQ(a << 0, a);
                NSFX_TEST_EXPECT_EQ(a << 52,  Address(0));
                NSFX_TEST_EXPECT_EQ(a << 900, Address(0));
                NSFX_TEST_EXPECT_EQ(a << 1,   Address(0x01fffffffffffeULL));
                NSFX_TEST_EXPECT_EQ(a << 15,  Address(0x0fffffffff8000ULL));
                NSFX_TEST_EXPECT_EQ(a <<=15,  Address(0x0fffffffff8000ULL));
            }

            // >>
            NSFX_TEST_CASE(RightShift)
            {
                Address a(0x0ffffffffffff0ULL);
                NSFX_TEST_EXPECT_EQ(a >> 0, a);
                NSFX_TEST_EXPECT_EQ(a >> 52,  Address(0));
                NSFX_TEST_EXPECT_EQ(a >> 900, Address(0));
                NSFX_TEST_EXPECT_EQ(a >> 1,   Address(0x07fffffffffff8ULL));
                NSFX_TEST_EXPECT_EQ(a >> 15,  Address(0x00001fffffffffULL));
                NSFX_TEST_EXPECT_EQ(a >>=15,  Address(0x00001fffffffffULL));
            }

            // bool
            NSFX_TEST_CASE(Bool)
            {
                Address a;
                Address b(0x0000000fffffffULL);
                NSFX_TEST_EXPECT(!a);
                NSFX_TEST_EXPECT(b);
                NSFX_TEST_EXPECT(!!b);
            }

            // hash
            NSFX_TEST_CASE(Hash)
            {
                Address a;
                Address b(0x0000000fffffffULL);
                NSFX_TEST_EXPECT_NE(boost::hash<Address>()(a),
                                    boost::hash<Address>()(b));
            }

            // swap
            NSFX_TEST_CASE(Swap)
            {
                Address a;
                Address b(0x0000000fffffffULL);
                boost::swap(a, b);
                NSFX_TEST_EXPECT_EQ(a, Address(0x0000000fffffffULL));
                NSFX_TEST_EXPECT_EQ(b, Address());
            }
        }

    }/*}}}*/

    NSFX_TEST_SUITE(124)/*{{{*/
    {
        typedef nsfx::Address<124>  Address;

        NSFX_TEST_SUITE(Ctor)
        {
            NSFX_TEST_CASE(0)
            {
                NSFX_TEST_EXPECT_EQ((size_t)(Address::NUM_BITS),  124);
                NSFX_TEST_EXPECT_EQ(Address::GetBitSize(), 124);

                NSFX_TEST_EXPECT_EQ((size_t)(Address::NUM_BYTES), 16);
                NSFX_TEST_EXPECT_EQ(Address::GetSize(), 16);

                NSFX_TEST_EXPECT_EQ(Address::Zero(), Address());
                NSFX_TEST_EXPECT_EQ(Address::Mask(), Address(-1L));
                NSFX_TEST_EXPECT_EQ(Address::Mask().ToString(), "0fffffffffffffffffffffffffffffff");
                                                              // 0       8       0       8

                {
                    Address a;
                    NSFX_TEST_EXPECT(a.IsZero());
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000000000000000000000000000000");
                                                    // 0       8       0       8
                }
            }

            NSFX_TEST_CASE(8)
            {
                // uint8_t array, shorter than addr
                {
                    uint8_t x[] = { 0x12, 0x34, 0x56 };
                    Address a(x, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000000000000000000000000123456");
                                                    // 0       8       0       8
                }

                {
                    uint8_t x[] = { 0x12, 0x34, 0x56 };
                    Address a(x, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000000000000000000000000563412");
                                                    // 0       8       0       8
                }

                // uint8_t array, longer than addr
                {
                    uint8_t x[] = { 0x12, 0x34, 0x56, 0x78,
                                    0x9a, 0xbc, 0xde, 0xf1,
                                    0x12, 0x34, 0x56, 0x78,
                                    0x9a, 0xbc, 0xde, 0xf1,
                                    0x12, 0x34, 0x56, 0x78 };
                    Address a(x, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0abcdef1123456789abcdef112345678");
                                                    // 0       8       0       8
                }

                {
                    uint8_t x[] = { 0x12, 0x34, 0x56, 0x78,
                                    0x9a, 0xbc, 0xde, 0xf1,
                                    0x12, 0x34, 0x56, 0x78,
                                    0x9a, 0xbc, 0xde, 0xf1,
                                    0x12, 0x34, 0x56, 0x78 };
                    Address a(x, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "01debc9a78563412f1debc9a78563412");
                                                    // 0       8       0       8
                }

                // uint8_t buffer, shorter than addr
                {
                    uint8_t x[] = { 0x12, 0x34, 0x56 };
                    Address a(x, sizeof (x), nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000000000000000000000000123456");
                                                    // 0       8       0       8
                }

                {
                    uint8_t x[] = { 0x12, 0x34, 0x56 };
                    Address a(x, sizeof (x), nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000000000000000000000000563412");
                                                    // 0       8       0       8
                }

                // uint8_t buffer, longer than addr
                {
                    uint8_t x[] = { 0x12, 0x34, 0x56, 0x78,
                                    0x9a, 0xbc, 0xde, 0xf1,
                                    0x12, 0x34, 0x56, 0x78,
                                    0x9a, 0xbc, 0xde, 0xf1,
                                    0x12, 0x34, 0x56, 0x78 };
                    Address a(x, sizeof (x), nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0abcdef1123456789abcdef112345678");
                                                    // 0       8       0       8
                }

                {
                    uint8_t x[] = { 0x12, 0x34, 0x56, 0x78,
                                    0x9a, 0xbc, 0xde, 0xf1,
                                    0x12, 0x34, 0x56, 0x78,
                                    0x9a, 0xbc, 0xde, 0xf1,
                                    0x12, 0x34, 0x56, 0x78 };
                    Address a(x, sizeof (x), nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "01debc9a78563412f1debc9a78563412");
                                                    // 0       8       0       8
                }

                // uint8_t pack, shorter than addr
                {
                    Address a((uint8_t)0x12, (uint8_t)0x34,
                              (uint8_t)0x56, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000000000000000000000000123456");
                                                    // 0       8       0       8
                }

                {
                    Address a((uint8_t)0x12, (uint8_t)0x34,
                              (uint8_t)0x56, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000000000000000000000000563412");
                                                    // 0       8       0       8
                }

                // uint8_t
                {
                    uint8_t x = 0x12;
                    Address a(x);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000000000000000000000000000012");
                                                    // 0       8       0       8
                }

                // int8_t
                {
                    int8_t x = (int8_t)0xf1;
                    Address a(x);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0ffffffffffffffffffffffffffffff1");
                                                    // 0       8       0       8
                }
            }

            NSFX_TEST_CASE(16)
            {
                // uint16_t array, shorter than addr
                {
                    uint16_t x[] = { 0x1234, 0x5678 };
                    Address a(x, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000000000000000000000012345678");
                                                    // 0       8       0       8
                }

                {
                    uint16_t x[] = { 0x1234, 0x5678 };
                    Address a(x, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000000000000000000000056781234");
                                                    // 0       8       0       8
                }

                // uint16_t array, longer than addr
                {
                    uint16_t x[] = { 0x1234, 0x5678, 0x9abc, 0xdef1,
                                     0x1234, 0x5678, 0x9abc, 0xdef1,
                                     0x1234, 0x5678 };
                    Address a(x, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0abcdef1123456789abcdef112345678");
                                                    // 0       8       0       8
                }

                {
                    uint16_t x[] = { 0x1234, 0x5678, 0x9abc, 0xdef1,
                                     0x1234, 0x5678, 0x9abc, 0xdef1,
                                     0x1234, 0x5678 };
                    Address a(x, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0ef19abc56781234def19abc56781234");
                                                    // 0       8       0       8
                }

                // uint16_t buffer, shorter than addr
                {
                    uint16_t x[] = { 0x1234, 0x5678 };
                    Address a(x, 2, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000000000000000000000012345678");
                                                    // 0       8       0       8
                }

                {
                    uint16_t x[] = { 0x1234, 0x5678 };
                    Address a(x, 2, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000000000000000000000056781234");
                                                    // 0       8       0       8
                }

                // uint16_t buffer, longer than addr
                {
                    uint16_t x[] = { 0x1234, 0x5678, 0x9abc, 0xdef1,
                                     0x1234, 0x5678, 0x9abc, 0xdef1,
                                     0x1234, 0x5678 };
                    Address a(x, 10, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0abcdef1123456789abcdef112345678");
                                                    // 0       8       0       8
                }

                {
                    uint16_t x[] = { 0x1234, 0x5678, 0x9abc, 0xdef1,
                                     0x1234, 0x5678, 0x9abc, 0xdef1,
                                     0x1234, 0x5678 };
                    Address a(x, 10, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0ef19abc56781234def19abc56781234");
                                                    // 0       8       0       8
                }

                // uint16_t pack, shorter than addr
                {
                    Address a((uint16_t)0x1234, (uint16_t)0x5678, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000000000000000000000012345678");
                                                    // 0       8       0       8
                }

                {
                    Address a((uint16_t)0x1234, (uint16_t)0x5678, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000000000000000000000056781234");
                                                    // 0       8       0       8
                }

                // uint16_t
                {
                    uint16_t x = 0x1234;
                    Address a(x);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000000000000000000000000001234");
                                                    // 0       8       0       8
                }

                // int16_t
                {
                    int16_t x = (int16_t)0x8765;
                    Address a(x);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0fffffffffffffffffffffffffff8765");
                                                    // 0       8       0       8
                }
            }

            NSFX_TEST_CASE(32)
            {
                // uint32_t array, short than addr
                {
                    uint32_t x[] = { 0x12345678 };
                    Address a(x, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000000000000000000000012345678");
                                                    // 0       8       0       8
                }

                {
                    uint32_t x[] = { 0x12345678 };
                    Address a(x, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000000000000000000000012345678");
                                                    // 0       8       0       8
                }

                // uint32_t array, longer than addr
                {
                    uint32_t x[] = { 0x12345678, 0x9abcdef1,
                                     0x9abcdef1, 0x12345678,
                                     0x12345678 };
                    Address a(x, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0abcdef19abcdef11234567812345678");
                                                    // 0       8       0       8
                }

                {
                    uint32_t x[] = { 0x12345678, 0x9abcdef1,
                                     0x9abcdef1, 0x12345678,
                                     0x12345678 };
                    Address a(x, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "023456789abcdef19abcdef112345678");
                                                    // 0       8       0       8
                }

                // uint32_t array, short than addr
                {
                    uint32_t x[] = { 0x12345678 };
                    Address a(x, 1, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000000000000000000000012345678");
                                                    // 0       8       0       8
                }

                {
                    uint32_t x[] = { 0x12345678 };
                    Address a(x, 1, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000000000000000000000012345678");
                                                    // 0       8       0       8
                }

                // uint32_t buffer, longer than addr
                {
                    uint32_t x[] = { 0x12345678, 0x9abcdef1,
                                     0x9abcdef1, 0x12345678,
                                     0x12345678 };
                    Address a(x, 5, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0abcdef19abcdef11234567812345678");
                                                    // 0       8       0       8
                }

                {
                    uint32_t x[] = { 0x12345678, 0x9abcdef1,
                                     0x9abcdef1, 0x12345678,
                                     0x12345678 };
                    Address a(x, 5, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "023456789abcdef19abcdef112345678");
                                                    // 0       8       0       8
                }

                // uint32_t pack, short than addr
                {
                    Address a(0x12345678, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000000000000000000000012345678");
                                                    // 0       8       0       8
                }

                {
                    Address a(0x12345678, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000000000000000000000012345678");
                                                    // 0       8       0       8
                }

                // uint32_t pack, longer than addr
                {
                    Address a(0x12345678, 0x9abcdef1,
                              0x9abcdef1, 0x12345678,
                              0x12345678, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0abcdef19abcdef11234567812345678");
                                                    // 0       8       0       8
                }

                {
                    Address a(0x12345678, 0x9abcdef1,
                              0x9abcdef1, 0x12345678,
                              0x12345678, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "023456789abcdef19abcdef112345678");
                                                    // 0       8       0       8
                }

                // uint32_t
                {
                    uint32_t x = 0x12345678;
                    Address a(x);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "00000000000000000000000012345678");
                                                    // 0       8       0       8
                }

                // int32_t
                {
                    int32_t x = 0x87654321;
                    Address a(x);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0fffffffffffffffffffffff87654321");
                                                    // 0       8       0       8
                }
            }

            NSFX_TEST_CASE(64)
            {
                // uint64_t array, shorter than addr
                {
                    uint64_t x[] = { 0x123456789abcdef1 };
                    Address a(x, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0000000000000000123456789abcdef1");
                                                    // 0       8       0       8
                }

                {
                    uint64_t x[] = { 0x123456789abcdef1 };
                    Address a(x, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0000000000000000123456789abcdef1");
                                                    // 0       8       0       8
                }

                // uint64_t array, longer than addr
                {
                    uint64_t x[] = { 0x123456789abcdef1, 0x1fedcba987654321,
                                     0x89abcdef17654321 };
                    Address a(x, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0fedcba98765432189abcdef17654321");
                                                    // 0       8       0       8
                }

                {
                    uint64_t x[] = { 0x123456789abcdef1, 0x1fedcba987654321,
                                     0x89abcdef17654321 };
                    Address a(x, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0fedcba987654321123456789abcdef1");
                                                    // 0       8       0       8
                }

                // uint64_t buffer, shorter than addr
                {
                    uint64_t x[] = { 0x123456789abcdef1 };
                    Address a(x, 1, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0000000000000000123456789abcdef1");
                                                    // 0       8       0       8
                }

                {
                    uint64_t x[] = { 0x123456789abcdef1 };
                    Address a(x, 1, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0000000000000000123456789abcdef1");
                                                    // 0       8       0       8
                }

                // uint64_t buffer, longer than addr
                {
                    uint64_t x[] = { 0x123456789abcdef1, 0x1fedcba987654321,
                                     0x89abcdef17654321 };
                    Address a(x, 3, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0fedcba98765432189abcdef17654321");
                                                    // 0       8       0       8
                }

                {
                    uint64_t x[] = { 0x123456789abcdef1, 0x1fedcba987654321,
                                     0x89abcdef17654321 };
                    Address a(x, 3, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0fedcba987654321123456789abcdef1");
                                                    // 0       8       0       8
                }

                // uint64_t pack, shorter than addr
                {
                    Address a(0x123456789abcdef1ULL, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0000000000000000123456789abcdef1");
                                                    // 0       8       0       8
                }

                {
                    Address a(0x123456789abcdef1ULL, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0000000000000000123456789abcdef1");
                                                    // 0       8       0       8
                }

                // uint64_t pack, longer than addr
                {
                    Address a(0x123456789abcdef1ULL, 0x1fedcba987654321ULL,
                              0x89abcdef17654321ULL, nsfx::big_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0fedcba98765432189abcdef17654321");
                                                    // 0       8       0       8
                }

                {
                    Address a(0x123456789abcdef1ULL, 0x1fedcba987654321ULL,
                              0x89abcdef17654321ULL, nsfx::little_endian);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0fedcba987654321123456789abcdef1");
                                                    // 0       8       0       8
                }

                // uint64_t
                {
                    uint64_t x = 0x123456789abcdef1;
                    Address a(x);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0000000000000000123456789abcdef1");
                                                    // 0       8       0       8
                }

                // int64_t
                {
                    int64_t x = 0xf123456789abcdef;
                    Address a(x);
                    NSFX_TEST_EXPECT_EQ(a.ToString(), "0ffffffffffffffff123456789abcdef");
                                                    // 0       8       0       8
                }
            }
        }

        NSFX_TEST_CASE(Comparison)
        {
            Address a(0x0000000000000000ULL, 0x00000000000001ULL, nsfx::big_endian);
            Address b(0x0f00000000000000ULL, 0x00000000000000ULL, nsfx::big_endian);

            NSFX_TEST_EXPECT(a == a);
            NSFX_TEST_EXPECT(!(a != a));

            NSFX_TEST_EXPECT(a != b);
            NSFX_TEST_EXPECT(b != a);

            NSFX_TEST_EXPECT(a < b);
            NSFX_TEST_EXPECT(a <= a);
            NSFX_TEST_EXPECT(a <= b);

            NSFX_TEST_EXPECT(b > a);
            NSFX_TEST_EXPECT(b >= a);
            NSFX_TEST_EXPECT(b >= b);
        }

        NSFX_TEST_SUITE(Arithmetics)
        {
            // ++
            NSFX_TEST_CASE(Increment)
            {
                Address a(0x0fffffffffffffffULL, 0xfffffffffffffffeULL, nsfx::big_endian);
                Address b(0x0fffffffffffffffULL, 0xffffffffffffffffULL, nsfx::big_endian);
                Address c(0x0000000000000000ULL, 0x0000000000000000ULL, nsfx::big_endian);
                NSFX_TEST_EXPECT_EQ(++a, b);
                NSFX_TEST_EXPECT_EQ(a++, b);
                NSFX_TEST_EXPECT_EQ(a, c);
            }

            // --
            NSFX_TEST_CASE(Decrement)
            {
                Address a(0x0000000000000000ULL, 0x0000000000000000ULL, nsfx::big_endian);
                Address b(0x0fffffffffffffffULL, 0xffffffffffffffffULL, nsfx::big_endian);
                Address c(0x0fffffffffffffffULL, 0xfffffffffffffffeULL, nsfx::big_endian);
                NSFX_TEST_EXPECT_EQ(--a, b);
                NSFX_TEST_EXPECT_EQ(a--, b);
                NSFX_TEST_EXPECT_EQ(a, c);
            }

            // +
            NSFX_TEST_CASE(Mirror)
            {
                Address a(0x0fffffffffffffffULL, 0xfffffffffffffff0ULL, nsfx::big_endian);
                NSFX_TEST_EXPECT_EQ(+a, a);
            }

            // -
            NSFX_TEST_CASE(Negate)
            {
                Address a(0x0fffffffffffffffULL, 0xfffffffffffffff0ULL, nsfx::big_endian);
                NSFX_TEST_EXPECT_EQ(-a, Address(0x0000000000000000, 0x0000000000000010, nsfx::big_endian));
            }

            // +
            NSFX_TEST_CASE(Plus)
            {
                Address a(0x0effffffffffffffULL, 0xfffffffffffffff0ULL, nsfx::big_endian);
                NSFX_TEST_EXPECT_EQ(a + 0x100000000000000f, Address(0x0f00000000000000ULL, 0x0fffffffffffffffULL, nsfx::big_endian));
                NSFX_TEST_EXPECT_EQ(a + 0x10000000000000ff, Address(0x0f00000000000000ULL, 0x10000000000000efULL, nsfx::big_endian));
                NSFX_TEST_EXPECT_EQ(0x10000000000000ff + a, Address(0x0f00000000000000ULL, 0x10000000000000efULL, nsfx::big_endian));
                NSFX_TEST_EXPECT_EQ(a +=0x10000000000000ff, Address(0x0f00000000000000ULL, 0x10000000000000efULL, nsfx::big_endian));
            }

            // -
            NSFX_TEST_CASE(Minus)
            {
                Address a(0x0000000000000000ULL, 0x00000000000000ffULL, nsfx::big_endian);
                NSFX_TEST_EXPECT_EQ(a - 0x000000000000000f, Address(0x0000000000000000ULL, 0x00000000000000f0ULL, nsfx::big_endian));
                NSFX_TEST_EXPECT_EQ(a - 0x0000000000000ff0, Address(0x0fffffffffffffffULL, 0xfffffffffffff10fULL, nsfx::big_endian));
                NSFX_TEST_EXPECT_EQ(a -=0x0000000000000ff0, Address(0x0fffffffffffffffULL, 0xfffffffffffff10fULL, nsfx::big_endian));
            }

            // -
            NSFX_TEST_CASE(Difference)
            {
                // 0
                Address a(0x08ffffffffffffffULL, 0xffffffffffffffffULL, nsfx::big_endian);
                NSFX_TEST_EXPECT_EQ(a - a, 0);

                // +
                Address b(0x08ffffffffffffffULL, 0x8000000000000001ULL, nsfx::big_endian);
                NSFX_TEST_EXPECT_EQ(a - b, 0x7ffffffffffffffeLL);
                --b;
                NSFX_TEST_EXPECT_EQ(a - b, 0x7fffffffffffffffLL);

                // +inf
                --b;
                NSFX_TEST_EXPECT_EQ(a - b, 0x7fffffffffffffffLL);

                // -
                Address c(0x0900000000000000ULL, 0x7ffffffffffffffeULL, nsfx::big_endian);
                NSFX_TEST_EXPECT_EQ(a - c, 0x8000000000000001LL);
                ++c;
                NSFX_TEST_EXPECT_EQ(a - c, 0x8000000000000000LL);

                // -inf
                ++c;
                NSFX_TEST_EXPECT_EQ(a - c, 0x8000000000000000LL);

            }

            // *
            NSFX_TEST_CASE(Multiply)
            {
                Address a(0x00ffffffffffffffULL, 0xffffffffffffffffULL, nsfx::big_endian);
                NSFX_TEST_EXPECT_EQ(a * 0, Address(0));
                NSFX_TEST_EXPECT_EQ(a * 1, Address(0x00ffffffffffffffULL, 0xffffffffffffffffULL, nsfx::big_endian));
                NSFX_TEST_EXPECT_EQ(a * 2, Address(0x01ffffffffffffffULL, 0xfffffffffffffffeULL, nsfx::big_endian));
                NSFX_TEST_EXPECT_EQ(a * 0xff, Address(0x0effffffffffffffULL, 0xffffffffffffff01ULL, nsfx::big_endian));
                NSFX_TEST_EXPECT_EQ(0xff * a, Address(0x0effffffffffffffULL, 0xffffffffffffff01ULL, nsfx::big_endian));
                NSFX_TEST_EXPECT_EQ(a *=0xff, Address(0x0effffffffffffffULL, 0xffffffffffffff01ULL, nsfx::big_endian));
            }

            // ~
            NSFX_TEST_CASE(Not)
            {
                Address a(0x00ffffffffffffffULL, 0xffffffffffffffffULL, nsfx::big_endian);
                NSFX_TEST_EXPECT_EQ(~a, Address(0x0f00000000000000ULL, 0x0000000000000000ULL, nsfx::big_endian));
            }

            // &
            NSFX_TEST_CASE(And)
            {
                Address a(0x0fedcba987654321ULL, 0x123456789abcdef1ULL, nsfx::big_endian);
                Address b(0x00000000ffffffffULL, 0x00000000ffffffffULL, nsfx::big_endian);
                NSFX_TEST_EXPECT_EQ(a & b, Address(0x0000000087654321ULL, 0x000000009abcdef1ULL, nsfx::big_endian));
                NSFX_TEST_EXPECT_EQ(a &=b, Address(0x0000000087654321ULL, 0x000000009abcdef1ULL, nsfx::big_endian));
            }

            // |
            NSFX_TEST_CASE(Or)
            {
                Address a(0x0fedcba900000000ULL, 0x1234567800000000ULL, nsfx::big_endian);
                Address b(0x0000000087654321ULL, 0x000000009abcdef1ULL, nsfx::big_endian);
                NSFX_TEST_EXPECT_EQ(a | b, Address(0x0fedcba987654321ULL, 0x123456789abcdef1ULL, nsfx::big_endian));
                NSFX_TEST_EXPECT_EQ(a |=b, Address(0x0fedcba987654321ULL, 0x123456789abcdef1ULL, nsfx::big_endian));
            }

            // ^
            NSFX_TEST_CASE(Xor)
            {
                Address a(0x0fedcba987654321ULL, 0x123456789abcdef1ULL, nsfx::big_endian);
                Address b(0x00000000ffffffffULL, 0x00000000ffffffffULL, nsfx::big_endian);
                NSFX_TEST_EXPECT_EQ(a ^ b, Address(0x0fedcba9789abcdeULL, 0x123456786543210eULL, nsfx::big_endian));
                NSFX_TEST_EXPECT_EQ(a ^=b, Address(0x0fedcba9789abcdeULL, 0x123456786543210eULL, nsfx::big_endian));
            }

            // <<
            NSFX_TEST_CASE(LeftShift)
            {
                Address a(0x00ffffffffffffffULL, 0xffffffffffffffffULL, nsfx::big_endian);
                NSFX_TEST_EXPECT_EQ(a << 0, a);
                NSFX_TEST_EXPECT_EQ(a << 124,  Address(0));
                NSFX_TEST_EXPECT_EQ(a << 900, Address(0));
                NSFX_TEST_EXPECT_EQ(a << 1,   Address(0x01ffffffffffffffULL, 0xfffffffffffffffeULL, nsfx::big_endian));
                NSFX_TEST_EXPECT_EQ(a << 15,  Address(0x0fffffffffffffffULL, 0xffffffffffff8000ULL, nsfx::big_endian));
                NSFX_TEST_EXPECT_EQ(a <<=15,  Address(0x0fffffffffffffffULL, 0xffffffffffff8000ULL, nsfx::big_endian));
            }

            // >>
            NSFX_TEST_CASE(RightShift)
            {
                Address a(0x0fffffffffffffffULL, 0xfffffffffffffff0ULL, nsfx::big_endian);
                NSFX_TEST_EXPECT_EQ(a >> 0, a);
                NSFX_TEST_EXPECT_EQ(a >> 124,  Address(0));
                NSFX_TEST_EXPECT_EQ(a >> 900, Address(0));
                NSFX_TEST_EXPECT_EQ(a >> 1,   Address(0x07ffffffffffffffULL, 0xfffffffffffffff8ULL, nsfx::big_endian));
                NSFX_TEST_EXPECT_EQ(a >> 15,  Address(0x00001fffffffffffULL, 0xffffffffffffffffULL, nsfx::big_endian));
                NSFX_TEST_EXPECT_EQ(a >>=15,  Address(0x00001fffffffffffULL, 0xffffffffffffffffULL, nsfx::big_endian));
            }

            // bool
            NSFX_TEST_CASE(Bool)
            {
                Address a;
                Address b(0x000000000fffffffULL, 0x000000000fffffffULL, nsfx::big_endian);
                NSFX_TEST_EXPECT(!a);
                NSFX_TEST_EXPECT(b);
                NSFX_TEST_EXPECT(!!b);
            }

            // hash
            NSFX_TEST_CASE(Hash)
            {
                Address a;
                Address b(0x000000000fffffffULL, 0x000000000fffffffULL, nsfx::big_endian);
                NSFX_TEST_EXPECT_NE(boost::hash<Address>()(a),
                                    boost::hash<Address>()(b));
            }

            // swap
            NSFX_TEST_CASE(Swap)
            {
                Address a;
                Address b(0x000000000fffffffULL, 0x000000000fffffffULL, nsfx::big_endian);
                boost::swap(a, b);
                NSFX_TEST_EXPECT_EQ(a, Address(0x000000000fffffffULL, 0x000000000fffffffULL, nsfx::big_endian));
                NSFX_TEST_EXPECT_EQ(b, Address());
            }
        }

    }/*}}}*/

}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cerr);
    nsfx::test::runner::Run();

    return 0;
}

