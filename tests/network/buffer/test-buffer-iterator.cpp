/**
 * @file
 *
 * @brief Test BufferIterator.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-05-27
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/network/buffer/buffer.h>
#include <nsfx/network/buffer/iterator/buffer-iterator.h>
#include <iostream>


NSFX_TEST_SUITE(BufferIterator)
{
    NSFX_TEST_CASE(Ctor)/*{{{*/
    {
        nsfx::Buffer b0(1000, 700, 300);
        // [700 s zs 300 ze e 300]
        b0.AddAtStart(100);
        b0.AddAtEnd(100);
        // [600 s 100 zs 300 ze 100 e 200]
        nsfx::BufferIterator it0 = b0.begin();
        nsfx::BufferIterator it1 = b0.end();

        NSFX_TEST_EXPECT_EQ(it0.GetStart(),  b0.GetStart());
        NSFX_TEST_EXPECT_EQ(it0.GetEnd(),    b0.GetEnd());
        NSFX_TEST_EXPECT_EQ(it0.GetCursor(), b0.GetStart());

        NSFX_TEST_EXPECT_EQ(it1.GetStart(),  b0.GetStart());
        NSFX_TEST_EXPECT_EQ(it1.GetEnd(),    b0.GetEnd());
        NSFX_TEST_EXPECT_EQ(it1.GetCursor(), b0.GetEnd());
    }/*}}}*/

    NSFX_TEST_CASE(CopyCtor)/*{{{*/
    {
        nsfx::Buffer b0(1000, 700, 300);
        // [700 s zs 300 ze e 300]
        b0.AddAtStart(100);
        b0.AddAtEnd(100);
        // [600 s 100 zs 300 ze 100 e 200]
        nsfx::BufferIterator it0 = b0.begin();
        nsfx::BufferIterator it1(it0);

        NSFX_TEST_EXPECT_EQ(it1.GetStart(),  b0.GetStart());
        NSFX_TEST_EXPECT_EQ(it1.GetEnd(),    b0.GetEnd());
        NSFX_TEST_EXPECT_EQ(it1.GetCursor(), b0.GetStart());
    }/*}}}*/

    NSFX_TEST_CASE(CopyAssign)/*{{{*/
    {
        nsfx::Buffer b0(1000, 700, 300);
        // [700 s zs 300 ze e 300]
        b0.AddAtStart(100);
        b0.AddAtEnd(100);
        // [600 s 100 zs 300 ze 100 e 200]
        nsfx::BufferIterator it0 = b0.begin();
        nsfx::BufferIterator it1 = b0.end();
        it1 = it0;

        NSFX_TEST_EXPECT_EQ(it1.GetStart(),  b0.GetStart());
        NSFX_TEST_EXPECT_EQ(it1.GetEnd(),    b0.GetEnd());
        NSFX_TEST_EXPECT_EQ(it1.GetCursor(), b0.GetStart());
    }/*}}}*/

    NSFX_TEST_CASE(Compare)/*{{{*/
    {
        nsfx::Buffer b0(1000, 700, 300);
        // [700 s zs 300 ze e 300]
        nsfx::BufferIterator it0 = b0.begin();
        nsfx::BufferIterator it1 = b0.end();

        NSFX_TEST_EXPECT(it0 == it0);
        NSFX_TEST_EXPECT(it0 <= it0);
        NSFX_TEST_EXPECT(it0 >= it0);

        NSFX_TEST_EXPECT(it1 == it1);
        NSFX_TEST_EXPECT(it1 <= it1);
        NSFX_TEST_EXPECT(it1 >= it1);

        NSFX_TEST_EXPECT(it0 != it1);
        NSFX_TEST_EXPECT(it1 != it0);

        NSFX_TEST_EXPECT(it0 < it1);
        NSFX_TEST_EXPECT(it0 <= it1);

        NSFX_TEST_EXPECT(it1 > it0);
        NSFX_TEST_EXPECT(it1 >= it0);
    }/*}}}*/

    NSFX_TEST_CASE(MoveAround)/*{{{*/
    {
        nsfx::Buffer b0(1000, 700, 300);
        // [700 s zs 300 ze e 300]
        nsfx::BufferIterator it = b0.begin();

        ++it;
        NSFX_TEST_EXPECT_EQ(it.GetCursor(), 701);
        --it;
        NSFX_TEST_EXPECT_EQ(it.GetCursor(), 700);

        auto it1 = it++;
        NSFX_TEST_EXPECT_EQ(it.GetCursor(), 701);
        NSFX_TEST_EXPECT_EQ(it1.GetCursor(), 700);

        it1 = it--;
        NSFX_TEST_EXPECT_EQ(it.GetCursor(), 700);
        NSFX_TEST_EXPECT_EQ(it1.GetCursor(), 701);

        it += 300;
        NSFX_TEST_EXPECT_EQ(it.GetCursor(), 1000);
        it -= 300;
        NSFX_TEST_EXPECT_EQ(it.GetCursor(), 700);

        it = it + 300;
        NSFX_TEST_EXPECT_EQ(it.GetCursor(), 1000);
        it = it - 300;
        NSFX_TEST_EXPECT_EQ(it.GetCursor(), 700);
    }/*}}}*/

    NSFX_TEST_SUITE(IO)/*{{{*/
    {
        NSFX_TEST_SUITE(Cursor)
        {
            NSFX_TEST_CASE(InHeaderArea)
            {
                nsfx::Buffer b0(1000, 700, 300);
                // [700 s zs 300 ze e 300]
                b0.AddAtStart(100);
                b0.AddAtEnd(100);
                // [600 s 100 zs 300 ze 100 e 200]
                nsfx::BufferIterator it = b0.begin();
                it += 100;
                size_t cursor = it.GetCursor();

                it -= 1;
                it.Write<uint8_t>(1);
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 2;
                it.Write<uint16_t>(1);
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 4;
                it.Write<uint32_t>(1);
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 8;
                it.Write<uint64_t>(1);
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 1;
                it.Read<uint8_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 2;
                it.Read<uint16_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 4;
                it.Read<uint32_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 8;
                it.Read<uint64_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 4;
                it.Read<float>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 8;
                it.Read<double>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                uint64_t t64;
                it -= 8;
                it.Read(reinterpret_cast<uint8_t*>(&t64), sizeof (uint64_t));
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());
            }

            NSFX_TEST_CASE(HeaderToZeroArea)
            {
                nsfx::Buffer b0(1000, 700, 300);
                // [700 s zs 300 ze e 300]
                b0.AddAtStart(100);
                b0.AddAtEnd(100);
                // [600 s 100 zs 300 ze 100 e 200]
                nsfx::BufferIterator it = b0.begin();
                it += 101;
                size_t cursor = it.GetCursor();

                it -= 1;
                it.Read<uint8_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 2;
                it.Read<uint16_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 4;
                it.Read<uint32_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 8;
                it.Read<uint64_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 4;
                it.Read<float>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 8;
                it.Read<double>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                uint64_t t64;
                it -= 8;
                it.Read(reinterpret_cast<uint8_t*>(&t64), sizeof (uint64_t));
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());
            }

            NSFX_TEST_CASE(InZeroArea)
            {
                nsfx::Buffer b0(1000, 700, 300);
                // [700 s zs 300 ze e 300]
                b0.AddAtStart(100);
                b0.AddAtEnd(100);
                // [600 s 100 zs 300 ze 100 e 200]
                nsfx::BufferIterator it = b0.begin();
                it += 400;
                size_t cursor = it.GetCursor();

                it -= 1;
                it.Read<uint8_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 2;
                it.Read<uint16_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 4;
                it.Read<uint32_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 8;
                it.Read<uint64_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 4;
                it.Read<float>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 8;
                it.Read<double>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                uint64_t t64;
                it -= 8;
                it.Read(reinterpret_cast<uint8_t*>(&t64), sizeof (uint64_t));
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());
            }

            NSFX_TEST_CASE(ZeroToTrailingArea)
            {
                nsfx::Buffer b0(1000, 700, 300);
                // [700 s zs 300 ze e 300]
                b0.AddAtStart(100);
                b0.AddAtEnd(100);
                // [600 s 100 zs 300 ze 100 e 200]
                nsfx::BufferIterator it = b0.begin();
                it += 401;
                size_t cursor = it.GetCursor();

                it -= 1;
                it.Read<uint8_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 2;
                it.Read<uint16_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 4;
                it.Read<uint32_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 8;
                it.Read<uint64_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 4;
                it.Read<float>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 8;
                it.Read<double>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                uint64_t t64;
                it -= 8;
                it.Read(reinterpret_cast<uint8_t*>(&t64), sizeof (uint64_t));
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());
            }

            NSFX_TEST_CASE(InTrailingArea)
            {
                nsfx::Buffer b0(1000, 700, 300);
                // [700 s zs 300 ze e 300]
                b0.AddAtStart(100);
                b0.AddAtEnd(100);
                // [600 s 100 zs 300 ze 100 e 200]
                nsfx::BufferIterator it = b0.begin();
                it += 500;
                size_t cursor = it.GetCursor();

                it -= 1;
                it.Write<uint8_t>(1);
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 2;
                it.Write<uint16_t>(1);
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 4;
                it.Write<uint32_t>(1);
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 8;
                it.Write<uint64_t>(1);
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 1;
                it.Read<uint8_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 2;
                it.Read<uint16_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 4;
                it.Read<uint32_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 8;
                it.Read<uint64_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 4;
                it.Read<float>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 8;
                it.Read<double>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                uint64_t t64;
                it -= 8;
                it.Read(reinterpret_cast<uint8_t*>(&t64), sizeof (uint64_t));
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());
            }

        }

        NSFX_TEST_CASE(Endian)
        {
            nsfx::Buffer b0(1000, 700, 300);
            // [700 s zs 300 ze e 300]
            b0.AddAtStart(100);
            // [600 s 100 zs 300 ze e 300]
            nsfx::BufferIterator it = b0.begin();
            const nsfx::Buffer::BufferStorage* storage = b0.GetStorage();
            const uint8_t*  p8  = reinterpret_cast<const uint8_t* >(storage->bytes_ + b0.GetStart());
            const uint16_t* p16 = reinterpret_cast<const uint16_t*>(storage->bytes_ + b0.GetStart());
            const uint32_t* p32 = reinterpret_cast<const uint32_t*>(storage->bytes_ + b0.GetStart());
            const uint64_t* p64 = reinterpret_cast<const uint64_t*>(storage->bytes_ + b0.GetStart());
            const float*    pf  = reinterpret_cast<const float*   >(storage->bytes_ + b0.GetStart());
            const double*   pd  = reinterpret_cast<const double*  >(storage->bytes_ + b0.GetStart());
            // n: native, r: reverse
            uint8_t  n8  = 0xfe;
            uint8_t  r8  = 0xfe;
            uint16_t n16 = 0xfedc;
            uint16_t r16 = 0xdcfe;
            uint32_t n32 = 0xfedcba98UL;
            uint32_t r32 = 0x98badcfeUL;
            uint64_t n64 = 0xfedcba9876543210ULL;
            uint64_t r64 = 0x1032547698badcfeULL;
            float    nf  = *(reinterpret_cast<float*>(&n32));
            float    rf  = *(reinterpret_cast<float*>(&r32));
            double   nd  = *(reinterpret_cast<double*>(&n64));
            double   rd  = *(reinterpret_cast<double*>(&r64));
            // l: little, b: big
#if defined(BOOST_LITTLE_ENDIAN)
            uint8_t  l8  = n8;
            uint8_t  b8  = r8;
            uint16_t l16 = n16;
            uint16_t b16 = r16;
            uint32_t l32 = n32;
            uint32_t b32 = r32;
            uint64_t l64 = n64;
            uint64_t b64 = r64;
            float    lf  = nf;
            float    bf  = rf;
            double   ld  = nd;
            double   bd  = rd;
#else // if defined(BOOST_BIG_ENDIAN)
            uint8_t  l8  = r8;
            uint8_t  b8  = n8;
            uint16_t l16 = r16;
            uint16_t b16 = n16;
            uint32_t l32 = r32;
            uint32_t b32 = n32;
            uint64_t l64 = r64;
            uint64_t b64 = n64;
            float    lf  = rf;
            float    bf  = nf;
            double   ld  = rd;
            double   bd  = nd;
#endif // defined(BOOST_LITTLE_ENDIAN)

            // native
            it.Write<int8_t>(n8);
            it -= sizeof (int8_t);
            NSFX_TEST_EXPECT_EQ(it.Read<int8_t>(), (int8_t)(n8));
            it -= sizeof (int8_t);
            NSFX_TEST_EXPECT_EQ((int8_t)(*p8), (int8_t)(n8));

            it.Write<uint8_t>(n8);
            it -= sizeof (uint8_t);
            NSFX_TEST_EXPECT_EQ(it.Read<uint8_t>(), n8);
            it -= sizeof (uint8_t);
            NSFX_TEST_EXPECT_EQ(*p8, n8);

            it.Write<int16_t>(n16);
            it -= sizeof (int16_t);
            NSFX_TEST_EXPECT_EQ(it.Read<int16_t>(), (int16_t)(n16));
            it -= sizeof (int16_t);
            NSFX_TEST_EXPECT_EQ((int16_t)(*p16), (int16_t)(n16));

            it.Write<uint16_t>(n16);
            it -= sizeof (uint16_t);
            NSFX_TEST_EXPECT_EQ(it.Read<uint16_t>(), n16);
            it -= sizeof (uint16_t);
            NSFX_TEST_EXPECT_EQ(*p16, n16);

            it.Write<int32_t>(n32);
            it -= sizeof (int32_t);
            NSFX_TEST_EXPECT_EQ(it.Read<int32_t>(), (int32_t)(n32));
            it -= sizeof (int32_t);
            NSFX_TEST_EXPECT_EQ((int32_t)(*p32), (int32_t)(n32));

            it.Write<uint32_t>(n32);
            it -= sizeof (uint32_t);
            NSFX_TEST_EXPECT_EQ(it.Read<uint32_t>(), n32);
            it -= sizeof (uint32_t);
            NSFX_TEST_EXPECT_EQ(*p32, n32);

            it.Write<int64_t>(n64);
            it -= sizeof (int64_t);
            NSFX_TEST_EXPECT_EQ(it.Read<int64_t>(), (int64_t)(n64));
            it -= sizeof (int64_t);
            NSFX_TEST_EXPECT_EQ((int64_t)(*p64), (int64_t)(n64));

            it.Write<uint64_t>(n64);
            it -= sizeof (uint64_t);
            NSFX_TEST_EXPECT_EQ(it.Read<uint64_t>(), n64);
            it -= sizeof (uint64_t);
            NSFX_TEST_EXPECT_EQ(*p64, n64);

            it.Write<float>(nf);
            it -= sizeof (float);
            NSFX_TEST_EXPECT_EQ(it.Read<float>(), nf);
            it -= sizeof (float);
            NSFX_TEST_EXPECT_EQ(*pf, nf);

            it.Write<double>(nd);
            it -= sizeof (double);
            NSFX_TEST_EXPECT_EQ(it.Read<double>(), nd);
            it -= sizeof (double);
            NSFX_TEST_EXPECT_EQ(*pd, nd);

            uint64_t t64;
            it.Write(reinterpret_cast<const uint8_t*>(&n64), sizeof (uint64_t));
            it -= sizeof (uint64_t);
            it.Read(reinterpret_cast<uint8_t*>(&t64), sizeof (uint64_t));
            it -= sizeof (uint64_t);
            NSFX_TEST_EXPECT_EQ(t64, n64);
            NSFX_TEST_EXPECT_EQ(*p64, n64);

            // little
            it.WriteL<int8_t>(n8);
            it -= sizeof (int8_t);
            NSFX_TEST_EXPECT_EQ(it.ReadL<int8_t>(), (int8_t)(n8));
            it -= sizeof (int8_t);
            NSFX_TEST_EXPECT_EQ((int8_t)(*p8), (int8_t)(l8));

            it.WriteL<uint8_t>(n8);
            it -= sizeof (uint8_t);
            NSFX_TEST_EXPECT_EQ(it.ReadL<uint8_t>(), n8);
            it -= sizeof (uint8_t);
            NSFX_TEST_EXPECT_EQ(*p8, l8);

            it.WriteL<int16_t>(n16);
            it -= sizeof (int16_t);
            NSFX_TEST_EXPECT_EQ(it.ReadL<int16_t>(), (int16_t)(n16));
            it -= sizeof (int16_t);
            NSFX_TEST_EXPECT_EQ((int16_t)(*p16), (int16_t)(l16));

            it.WriteL<uint16_t>(n16);
            it -= sizeof (uint16_t);
            NSFX_TEST_EXPECT_EQ(it.ReadL<uint16_t>(), n16);
            it -= sizeof (uint16_t);
            NSFX_TEST_EXPECT_EQ(*p16, l16);

            it.WriteL<int32_t>(n32);
            it -= sizeof (int32_t);
            NSFX_TEST_EXPECT_EQ(it.ReadL<int32_t>(), (int32_t)(n32));
            it -= sizeof (int32_t);
            NSFX_TEST_EXPECT_EQ((int32_t)(*p32), (int32_t)(l32));

            it.WriteL<uint32_t>(n32);
            it -= sizeof (uint32_t);
            NSFX_TEST_EXPECT_EQ(it.ReadL<uint32_t>(), n32);
            it -= sizeof (uint32_t);
            NSFX_TEST_EXPECT_EQ(*p32, l32);

            it.WriteL<int64_t>(n64);
            it -= sizeof (int64_t);
            NSFX_TEST_EXPECT_EQ(it.ReadL<int64_t>(), (int64_t)(n64));
            it -= sizeof (int64_t);
            NSFX_TEST_EXPECT_EQ((int64_t)(*p64), (int64_t)(l64));

            it.WriteL<uint64_t>(n64);
            it -= sizeof (uint64_t);
            NSFX_TEST_EXPECT_EQ(it.ReadL<uint64_t>(), n64);
            it -= sizeof (uint64_t);
            NSFX_TEST_EXPECT_EQ(*p64, l64);

            it.WriteL<float>(nf);
            it -= sizeof (float);
            NSFX_TEST_EXPECT_EQ(it.ReadL<float>(), nf);
            it -= sizeof (float);
            NSFX_TEST_EXPECT_EQ(*pf, lf);

            it.WriteL<double>(nd);
            it -= sizeof (double);
            NSFX_TEST_EXPECT_EQ(it.ReadL<double>(), nd);
            it -= sizeof (double);
            NSFX_TEST_EXPECT_EQ(*pd, ld);

            it.WriteL(reinterpret_cast<const uint8_t*>(&n64), sizeof (uint64_t));
            it -= sizeof (uint64_t);
            NSFX_TEST_EXPECT_EQ(it.ReadL<uint64_t>(), n64);
            it -= sizeof (uint64_t);
            NSFX_TEST_EXPECT_EQ(*p64, l64);

            it.WriteL(reinterpret_cast<const uint8_t*>(&n64), sizeof (uint64_t));
            it -= sizeof (uint64_t);
            it.ReadL(reinterpret_cast<uint8_t*>(&t64), sizeof (uint64_t));
            it -= sizeof (uint64_t);
            NSFX_TEST_EXPECT_EQ(t64, n64);
            NSFX_TEST_EXPECT_EQ(*p64, l64);

            // big
            it.WriteB<int8_t>(n8);
            it -= sizeof (int8_t);
            NSFX_TEST_EXPECT_EQ(it.ReadB<int8_t>(), (int8_t)(n8));
            it -= sizeof (int8_t);
            NSFX_TEST_EXPECT_EQ((int8_t)(*p8), (int8_t)(b8));

            it.WriteB<uint8_t>(n8);
            it -= sizeof (uint8_t);
            NSFX_TEST_EXPECT_EQ(it.ReadB<uint8_t>(), n8);
            it -= sizeof (uint8_t);
            NSFX_TEST_EXPECT_EQ(*p8, b8);

            it.WriteB<int16_t>(n16);
            it -= sizeof (int16_t);
            NSFX_TEST_EXPECT_EQ(it.ReadB<int16_t>(), (int16_t)(n16));
            it -= sizeof (int16_t);
            NSFX_TEST_EXPECT_EQ((int16_t)(*p16), (int16_t)(b16));

            it.WriteB<uint16_t>(n16);
            it -= sizeof (uint16_t);
            NSFX_TEST_EXPECT_EQ(it.ReadB<uint16_t>(), n16);
            it -= sizeof (uint16_t);
            NSFX_TEST_EXPECT_EQ(*p16, b16);

            it.WriteB<int32_t>(n32);
            it -= sizeof (int32_t);
            NSFX_TEST_EXPECT_EQ(it.ReadB<int32_t>(), (int32_t)(n32));
            it -= sizeof (int32_t);
            NSFX_TEST_EXPECT_EQ((int32_t)(*p32), (int32_t)(b32));

            it.WriteB<uint32_t>(n32);
            it -= sizeof (uint32_t);
            NSFX_TEST_EXPECT_EQ(it.ReadB<uint32_t>(), n32);
            it -= sizeof (uint32_t);
            NSFX_TEST_EXPECT_EQ(*p32, b32);

            it.WriteB<int64_t>(n64);
            it -= sizeof (int64_t);
            NSFX_TEST_EXPECT_EQ(it.ReadB<int64_t>(), (int64_t)(n64));
            it -= sizeof (int64_t);
            NSFX_TEST_EXPECT_EQ((int64_t)(*p64), (int64_t)(b64));

            it.WriteB<uint64_t>(n64);
            it -= sizeof (uint64_t);
            NSFX_TEST_EXPECT_EQ(it.ReadB<uint64_t>(), n64);
            it -= sizeof (uint64_t);
            NSFX_TEST_EXPECT_EQ(*p64, b64);

            it.WriteB<float>(nf);
            it -= sizeof (float);
            NSFX_TEST_EXPECT_EQ(it.ReadB<float>(), nf);
            it -= sizeof (float);
            NSFX_TEST_EXPECT_EQ(*pf, bf);

            it.WriteB<double>(nd);
            it -= sizeof (double);
            NSFX_TEST_EXPECT_EQ(it.ReadB<double>(), nd);
            it -= sizeof (double);
            NSFX_TEST_EXPECT_EQ(*pd, bd);

            it.WriteB(reinterpret_cast<const uint8_t*>(&n64), sizeof (uint64_t));
            it -= sizeof (uint64_t);
            NSFX_TEST_EXPECT_EQ(it.ReadB<uint64_t>(), n64);
            it -= sizeof (uint64_t);
            NSFX_TEST_EXPECT_EQ(*p64, b64);

            it.WriteB(reinterpret_cast<const uint8_t*>(&n64), sizeof (uint64_t));
            it -= sizeof (uint64_t);
            it.ReadB(reinterpret_cast<uint8_t*>(&t64), sizeof (uint64_t));
            it -= sizeof (uint64_t);
            NSFX_TEST_EXPECT_EQ(t64, n64);
            NSFX_TEST_EXPECT_EQ(*p64, b64);

        }

        NSFX_TEST_CASE(AroundBuffer)
        {
            nsfx::Buffer b0(1000, 700, 300);
            // [700 s zs 300 ze e 300]
            b0.AddAtStart(100);
            b0.AddAtEnd(100);
            // [600 s 100 zs 300 ze 100 e 200]
            nsfx::BufferIterator it = b0.begin();
            const nsfx::Buffer::BufferStorage* storage = b0.GetStorage();
            const uint8_t* p8 = reinterpret_cast<const uint8_t* >(storage->bytes_ + b0.GetStart());

            // Write header area.
            NSFX_TEST_EXPECT_EQ(it.GetCursor(), 600);
            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t n8 = 0xfe + i;
                it.Write<uint8_t>(n8);
                NSFX_TEST_EXPECT_EQ(*p8, n8);
                ++p8;
            }

            // Read zero area.
            NSFX_TEST_EXPECT_EQ(it.GetCursor(), 700);
            for (size_t i = 0; i < 300; ++i)
            {
                NSFX_TEST_EXPECT_EQ(it.Read<uint8_t>(), 0);
                ++p8;
            }

            // Write trailer area.
            NSFX_TEST_ASSERT_EQ(it.GetCursor(), 1000);
            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t n8 = 0xef + i;
                it.Write<uint8_t>(n8);
                NSFX_TEST_EXPECT_EQ(*p8, n8);
                ++p8;
            }
        }
    }/*}}}*/
}


NSFX_TEST_SUITE(ConstBufferIterator)
{
    NSFX_TEST_CASE(Ctor)/*{{{*/
    {
        nsfx::Buffer b0(1000, 700, 300);
        // [700 s zs 300 ze e 300]
        b0.AddAtStart(100);
        b0.AddAtEnd(100);
        // [600 s 100 zs 300 ze 100 e 200]
        nsfx::ConstBufferIterator it0 = b0.cbegin();
        nsfx::ConstBufferIterator it1 = b0.cend();

        NSFX_TEST_EXPECT_EQ(it0.GetStart(),  b0.GetStart());
        NSFX_TEST_EXPECT_EQ(it0.GetEnd(),    b0.GetEnd());
        NSFX_TEST_EXPECT_EQ(it0.GetCursor(), b0.GetStart());

        NSFX_TEST_EXPECT_EQ(it1.GetStart(),  b0.GetStart());
        NSFX_TEST_EXPECT_EQ(it1.GetEnd(),    b0.GetEnd());
        NSFX_TEST_EXPECT_EQ(it1.GetCursor(), b0.GetEnd());
    }/*}}}*/

    NSFX_TEST_CASE(CopyCtor)/*{{{*/
    {
        nsfx::Buffer b0(1000, 700, 300);
        // [700 s zs 300 ze e 300]
        b0.AddAtStart(100);
        b0.AddAtEnd(100);
        // [600 s 100 zs 300 ze 100 e 200]
        nsfx::ConstBufferIterator it0 = b0.cbegin();
        nsfx::ConstBufferIterator it1(it0);

        NSFX_TEST_EXPECT_EQ(it1.GetStart(),  b0.GetStart());
        NSFX_TEST_EXPECT_EQ(it1.GetEnd(),    b0.GetEnd());
        NSFX_TEST_EXPECT_EQ(it1.GetCursor(), b0.GetStart());

        nsfx::ConstBufferIterator it2(b0.begin());

        NSFX_TEST_EXPECT_EQ(it2.GetStart(),  b0.GetStart());
        NSFX_TEST_EXPECT_EQ(it2.GetEnd(),    b0.GetEnd());
        NSFX_TEST_EXPECT_EQ(it2.GetCursor(), b0.GetStart());
    }/*}}}*/

    NSFX_TEST_CASE(CopyAssign)/*{{{*/
    {
        nsfx::Buffer b0(1000, 700, 300);
        // [700 s zs 300 ze e 300]
        b0.AddAtStart(100);
        b0.AddAtEnd(100);
        // [600 s 100 zs 300 ze 100 e 200]
        nsfx::ConstBufferIterator it0 = b0.cbegin();
        nsfx::ConstBufferIterator it1 = b0.cend();
        it1 = it0;

        NSFX_TEST_EXPECT_EQ(it1.GetStart(),  b0.GetStart());
        NSFX_TEST_EXPECT_EQ(it1.GetEnd(),    b0.GetEnd());
        NSFX_TEST_EXPECT_EQ(it1.GetCursor(), b0.GetStart());

        it0 = b0.begin();

        NSFX_TEST_EXPECT_EQ(it0.GetStart(),  b0.GetStart());
        NSFX_TEST_EXPECT_EQ(it0.GetEnd(),    b0.GetEnd());
        NSFX_TEST_EXPECT_EQ(it0.GetCursor(), b0.GetStart());
    }/*}}}*/

    NSFX_TEST_CASE(Compare)/*{{{*/
    {
        nsfx::Buffer b0(1000, 700, 300);
        // [700 s zs 300 ze e 300]
        nsfx::ConstBufferIterator it0 = b0.cbegin();
        nsfx::ConstBufferIterator it1 = b0.cend();

        NSFX_TEST_EXPECT(it0 == it0);
        NSFX_TEST_EXPECT(it0 <= it0);
        NSFX_TEST_EXPECT(it0 >= it0);

        NSFX_TEST_EXPECT(it1 == it1);
        NSFX_TEST_EXPECT(it1 <= it1);
        NSFX_TEST_EXPECT(it1 >= it1);

        NSFX_TEST_EXPECT(it0 != it1);
        NSFX_TEST_EXPECT(it1 != it0);

        NSFX_TEST_EXPECT(it0 < it1);
        NSFX_TEST_EXPECT(it0 <= it1);

        NSFX_TEST_EXPECT(it1 > it0);
        NSFX_TEST_EXPECT(it1 >= it0);
    }/*}}}*/

    NSFX_TEST_CASE(MoveAround)/*{{{*/
    {
        nsfx::Buffer b0(1000, 700, 300);
        // [700 s zs 300 ze e 300]
        nsfx::ConstBufferIterator it = b0.cbegin();

        ++it;
        NSFX_TEST_EXPECT_EQ(it.GetCursor(), 701);
        --it;
        NSFX_TEST_EXPECT_EQ(it.GetCursor(), 700);

        auto it1 = it++;
        NSFX_TEST_EXPECT_EQ(it.GetCursor(), 701);
        NSFX_TEST_EXPECT_EQ(it1.GetCursor(), 700);

        it1 = it--;
        NSFX_TEST_EXPECT_EQ(it.GetCursor(), 700);
        NSFX_TEST_EXPECT_EQ(it1.GetCursor(), 701);

        it += 300;
        NSFX_TEST_EXPECT_EQ(it.GetCursor(), 1000);
        it -= 300;
        NSFX_TEST_EXPECT_EQ(it.GetCursor(), 700);

        it = it + 300;
        NSFX_TEST_EXPECT_EQ(it.GetCursor(), 1000);
        it = it - 300;
        NSFX_TEST_EXPECT_EQ(it.GetCursor(), 700);
    }/*}}}*/

    NSFX_TEST_SUITE(IO)/*{{{*/
    {
        NSFX_TEST_SUITE(Cursor)
        {
            NSFX_TEST_CASE(InHeaderArea)
            {
                nsfx::Buffer b0(1000, 700, 300);
                // [700 s zs 300 ze e 300]
                b0.AddAtStart(100);
                b0.AddAtEnd(100);
                // [600 s 100 zs 300 ze 100 e 200]
                nsfx::ConstBufferIterator it = b0.cbegin();
                it += 100;
                size_t cursor = it.GetCursor();

                it -= 1;
                it.Read<uint8_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 2;
                it.Read<uint16_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 4;
                it.Read<uint32_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 8;
                it.Read<uint64_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());
            }

            NSFX_TEST_CASE(HeaderToZeroArea)
            {
                nsfx::Buffer b0(1000, 700, 300);
                // [700 s zs 300 ze e 300]
                b0.AddAtStart(100);
                b0.AddAtEnd(100);
                // [600 s 100 zs 300 ze 100 e 200]
                nsfx::ConstBufferIterator it = b0.cbegin();
                it += 101;
                size_t cursor = it.GetCursor();

                it -= 1;
                it.Read<uint8_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 2;
                it.Read<uint16_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 4;
                it.Read<uint32_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 8;
                it.Read<uint64_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());
            }

            NSFX_TEST_CASE(InZeroArea)
            {
                nsfx::Buffer b0(1000, 700, 300);
                // [700 s zs 300 ze e 300]
                b0.AddAtStart(100);
                b0.AddAtEnd(100);
                // [600 s 100 zs 300 ze 100 e 200]
                nsfx::ConstBufferIterator it = b0.cbegin();
                it += 400;
                size_t cursor = it.GetCursor();

                it -= 1;
                it.Read<uint8_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 2;
                it.Read<uint16_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 4;
                it.Read<uint32_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 8;
                it.Read<uint64_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());
            }

            NSFX_TEST_CASE(ZeroToTrailingArea)
            {
                nsfx::Buffer b0(1000, 700, 300);
                // [700 s zs 300 ze e 300]
                b0.AddAtStart(100);
                b0.AddAtEnd(100);
                // [600 s 100 zs 300 ze 100 e 200]
                nsfx::ConstBufferIterator it = b0.cbegin();
                it += 401;
                size_t cursor = it.GetCursor();

                it -= 1;
                it.Read<uint8_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 2;
                it.Read<uint16_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 4;
                it.Read<uint32_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 8;
                it.Read<uint64_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());
            }

            NSFX_TEST_CASE(InTrailingArea)
            {
                nsfx::Buffer b0(1000, 700, 300);
                // [700 s zs 300 ze e 300]
                b0.AddAtStart(100);
                b0.AddAtEnd(100);
                // [600 s 100 zs 300 ze 100 e 200]
                nsfx::ConstBufferIterator it = b0.cbegin();
                it += 500;
                size_t cursor = it.GetCursor();

                it -= 1;
                it.Read<uint8_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 2;
                it.Read<uint16_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 4;
                it.Read<uint32_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());

                it -= 8;
                it.Read<uint64_t>();
                NSFX_TEST_EXPECT_EQ(cursor, it.GetCursor());
            }

        }

        NSFX_TEST_CASE(Endian)
        {
            nsfx::Buffer b0(1000, 700, 300);
            // [700 s zs 300 ze e 300]
            b0.AddAtStart(100);
            // [600 s 100 zs 300 ze e 300]
            nsfx::BufferIterator it = b0.begin();
            nsfx::ConstBufferIterator it1 = b0.cbegin();
            const nsfx::Buffer::BufferStorage* storage = b0.GetStorage();
            const uint8_t*  p8  = reinterpret_cast<const uint8_t* >(storage->bytes_ + b0.GetStart());
            const uint16_t* p16 = reinterpret_cast<const uint16_t*>(storage->bytes_ + b0.GetStart());
            const uint32_t* p32 = reinterpret_cast<const uint32_t*>(storage->bytes_ + b0.GetStart());
            const uint64_t* p64 = reinterpret_cast<const uint64_t*>(storage->bytes_ + b0.GetStart());
            const float*    pf  = reinterpret_cast<const float*   >(storage->bytes_ + b0.GetStart());
            const double*   pd  = reinterpret_cast<const double*  >(storage->bytes_ + b0.GetStart());
            // n: native, r: reverse
            uint8_t  n8  = 0xfe;
            uint8_t  r8  = 0xfe;
            uint16_t n16 = 0xfedc;
            uint16_t r16 = 0xdcfe;
            uint32_t n32 = 0xfedcba98UL;
            uint32_t r32 = 0x98badcfeUL;
            uint64_t n64 = 0xfedcba9876543210ULL;
            uint64_t r64 = 0x1032547698badcfeULL;
            float    nf  = *(reinterpret_cast<float*>(&n32));
            float    rf  = *(reinterpret_cast<float*>(&r32));
            double   nd  = *(reinterpret_cast<double*>(&n64));
            double   rd  = *(reinterpret_cast<double*>(&r64));
            // l: little, b: big
#if defined(BOOST_LITTLE_ENDIAN)
            uint8_t  l8  = n8;
            uint8_t  b8  = r8;
            uint16_t l16 = n16;
            uint16_t b16 = r16;
            uint32_t l32 = n32;
            uint32_t b32 = r32;
            uint64_t l64 = n64;
            uint64_t b64 = r64;
            float    lf  = nf;
            float    bf  = rf;
            double   ld  = nd;
            double   bd  = rd;
#else // if defined(BOOST_BIG_ENDIAN)
            uint8_t  l8  = r8;
            uint8_t  b8  = n8;
            uint16_t l16 = r16;
            uint16_t b16 = n16;
            uint32_t l32 = r32;
            uint32_t b32 = n32;
            uint64_t l64 = r64;
            uint64_t b64 = n64;
            float    lf  = rf;
            float    bf  = nf;
            double   ld  = rd;
            double   bd  = nd;
#endif // defined(BOOST_LITTLE_ENDIAN)

            // native
            it.Write<int8_t>(n8);
            NSFX_TEST_EXPECT_EQ(it1.Read<int8_t>(), (int8_t)(n8));
            it -= sizeof (int8_t);
            it1 -= sizeof (int8_t);
            NSFX_TEST_EXPECT_EQ((int8_t)(*p8), (int8_t)(n8));

            it.Write<uint8_t>(n8);
            NSFX_TEST_EXPECT_EQ(it1.Read<uint8_t>(), n8);
            it -= sizeof (uint8_t);
            it1 -= sizeof (uint8_t);
            NSFX_TEST_EXPECT_EQ(*p8, n8);

            it.Write<int16_t>(n16);
            NSFX_TEST_EXPECT_EQ(it1.Read<int16_t>(), (int16_t)(n16));
            it -= sizeof (int16_t);
            it1 -= sizeof (int16_t);
            NSFX_TEST_EXPECT_EQ((int16_t)(*p16), (int16_t)(n16));

            it.Write<uint16_t>(n16);
            NSFX_TEST_EXPECT_EQ(it1.Read<uint16_t>(), n16);
            it -= sizeof (uint16_t);
            it1 -= sizeof (uint16_t);
            NSFX_TEST_EXPECT_EQ(*p16, n16);

            it.Write<int32_t>(n32);
            NSFX_TEST_EXPECT_EQ(it1.Read<int32_t>(), (int32_t)(n32));
            it -= sizeof (int32_t);
            it1 -= sizeof (int32_t);
            NSFX_TEST_EXPECT_EQ((int32_t)(*p32), (int32_t)(n32));

            it.Write<uint32_t>(n32);
            NSFX_TEST_EXPECT_EQ(it1.Read<uint32_t>(), n32);
            it -= sizeof (uint32_t);
            it1 -= sizeof (uint32_t);
            NSFX_TEST_EXPECT_EQ(*p32, n32);

            it.Write<int64_t>(n64);
            NSFX_TEST_EXPECT_EQ(it1.Read<int64_t>(), (int64_t)(n64));
            it -= sizeof (int64_t);
            it1 -= sizeof (int64_t);
            NSFX_TEST_EXPECT_EQ((int64_t)(*p64), (int64_t)(n64));

            it.Write<uint64_t>(n64);
            NSFX_TEST_EXPECT_EQ(it1.Read<uint64_t>(), n64);
            it -= sizeof (uint64_t);
            it1 -= sizeof (int64_t);
            NSFX_TEST_EXPECT_EQ(*p64, n64);

            it.Write<float>(nf);
            NSFX_TEST_EXPECT_EQ(it1.Read<float>(), nf);
            it -= sizeof (float);
            it1 -= sizeof (float);
            NSFX_TEST_EXPECT_EQ(*pf, nf);

            it.Write<double>(nd);
            NSFX_TEST_EXPECT_EQ(it1.Read<double>(), nd);
            it -= sizeof (double);
            it1 -= sizeof (double);
            NSFX_TEST_EXPECT_EQ(*pd, nd);

            uint64_t t64;
            it.Write(reinterpret_cast<const uint8_t*>(&n64), sizeof (uint64_t));
            it1.Read(reinterpret_cast<uint8_t*>(&t64), sizeof (uint64_t));
            it -= sizeof (uint64_t);
            it1 -= sizeof (uint64_t);
            NSFX_TEST_EXPECT_EQ(t64, n64);
            NSFX_TEST_EXPECT_EQ(*p64, n64);

            // little
            it.WriteL<int8_t>(n8);
            NSFX_TEST_EXPECT_EQ(it1.ReadL<int8_t>(), (int8_t)(n8));
            it -= sizeof (int8_t);
            it1 -= sizeof (int8_t);
            NSFX_TEST_EXPECT_EQ((int8_t)(*p8), (int8_t)(l8));

            it.WriteL<uint8_t>(n8);
            NSFX_TEST_EXPECT_EQ(it1.ReadL<uint8_t>(), n8);
            it -= sizeof (uint8_t);
            it1 -= sizeof (uint8_t);
            NSFX_TEST_EXPECT_EQ(*p8, l8);

            it.WriteL<int16_t>(n16);
            NSFX_TEST_EXPECT_EQ(it1.ReadL<int16_t>(), (int16_t)(n16));
            it -= sizeof (int16_t);
            it1 -= sizeof (int16_t);
            NSFX_TEST_EXPECT_EQ((int16_t)(*p16), (int16_t)(l16));

            it.WriteL<uint16_t>(n16);
            NSFX_TEST_EXPECT_EQ(it1.ReadL<uint16_t>(), n16);
            it -= sizeof (uint16_t);
            it1 -= sizeof (uint16_t);
            NSFX_TEST_EXPECT_EQ(*p16, l16);

            it.WriteL<int32_t>(n32);
            NSFX_TEST_EXPECT_EQ(it1.ReadL<int32_t>(), (int32_t)(n32));
            it -= sizeof (int32_t);
            it1 -= sizeof (int32_t);
            NSFX_TEST_EXPECT_EQ((int32_t)(*p32), (int32_t)(l32));

            it.WriteL<uint32_t>(n32);
            NSFX_TEST_EXPECT_EQ(it1.ReadL<uint32_t>(), n32);
            it -= sizeof (uint32_t);
            it1 -= sizeof (uint32_t);
            NSFX_TEST_EXPECT_EQ(*p32, l32);

            it.WriteL<int64_t>(n64);
            NSFX_TEST_EXPECT_EQ(it1.ReadL<int64_t>(), (int64_t)(n64));
            it -= sizeof (int64_t);
            it1 -= sizeof (int64_t);
            NSFX_TEST_EXPECT_EQ((int64_t)(*p64), (int64_t)(l64));

            it.WriteL<uint64_t>(n64);
            NSFX_TEST_EXPECT_EQ(it1.ReadL<uint64_t>(), n64);
            it -= sizeof (uint64_t);
            it1 -= sizeof (uint64_t);
            NSFX_TEST_EXPECT_EQ(*p64, l64);

            it.WriteL<float>(nf);
            NSFX_TEST_EXPECT_EQ(it1.ReadL<float>(), nf);
            it -= sizeof (float);
            it1 -= sizeof (float);
            NSFX_TEST_EXPECT_EQ(*pf, lf);

            it.WriteL<double>(nd);
            NSFX_TEST_EXPECT_EQ(it1.ReadL<double>(), nd);
            it -= sizeof (double);
            it1 -= sizeof (double);
            NSFX_TEST_EXPECT_EQ(*pd, ld);

            it.WriteL(reinterpret_cast<const uint8_t*>(&n64), sizeof (uint64_t));
            it1.ReadL(reinterpret_cast<uint8_t*>(&t64), sizeof (uint64_t));
            it -= sizeof (uint64_t);
            it1 -= sizeof (uint64_t);
            NSFX_TEST_EXPECT_EQ(t64, n64);
            NSFX_TEST_EXPECT_EQ(*p64, l64);

            // big
            it.WriteB<int8_t>(n8);
            NSFX_TEST_EXPECT_EQ(it1.ReadB<int8_t>(), (int8_t)(n8));
            it -= sizeof (int8_t);
            it1 -= sizeof (int8_t);
            NSFX_TEST_EXPECT_EQ((int8_t)(*p8), (int8_t)(b8));

            it.WriteB<uint8_t>(n8);
            NSFX_TEST_EXPECT_EQ(it1.ReadB<uint8_t>(), n8);
            it -= sizeof (uint8_t);
            it1 -= sizeof (uint8_t);
            NSFX_TEST_EXPECT_EQ(*p8, b8);

            it.WriteB<int16_t>(n16);
            NSFX_TEST_EXPECT_EQ(it1.ReadB<int16_t>(), (int16_t)(n16));
            it -= sizeof (int16_t);
            it1 -= sizeof (int16_t);
            NSFX_TEST_EXPECT_EQ((int16_t)(*p16), (int16_t)(b16));

            it.WriteB<uint16_t>(n16);
            NSFX_TEST_EXPECT_EQ(it1.ReadB<uint16_t>(), n16);
            it -= sizeof (uint16_t);
            it1 -= sizeof (uint16_t);
            NSFX_TEST_EXPECT_EQ(*p16, b16);

            it.WriteB<int32_t>(n32);
            NSFX_TEST_EXPECT_EQ(it1.ReadB<int32_t>(), (int32_t)(n32));
            it -= sizeof (int32_t);
            it1 -= sizeof (int32_t);
            NSFX_TEST_EXPECT_EQ((int32_t)(*p32), (int32_t)(b32));

            it.WriteB<uint32_t>(n32);
            NSFX_TEST_EXPECT_EQ(it1.ReadB<uint32_t>(), n32);
            it -= sizeof (uint32_t);
            it1 -= sizeof (uint32_t);
            NSFX_TEST_EXPECT_EQ(*p32, b32);

            it.WriteB<int64_t>(n64);
            NSFX_TEST_EXPECT_EQ(it1.ReadB<int64_t>(), (int64_t)(n64));
            it -= sizeof (int64_t);
            it1 -= sizeof (int64_t);
            NSFX_TEST_EXPECT_EQ((int64_t)(*p64), (int64_t)(b64));

            it.WriteB<uint64_t>(n64);
            NSFX_TEST_EXPECT_EQ(it1.ReadB<uint64_t>(), n64);
            it -= sizeof (uint64_t);
            it1 -= sizeof (uint64_t);
            NSFX_TEST_EXPECT_EQ(*p64, b64);

            it.WriteB<float>(nf);
            NSFX_TEST_EXPECT_EQ(it1.ReadB<float>(), nf);
            it -= sizeof (float);
            it1 -= sizeof (float);
            NSFX_TEST_EXPECT_EQ(*pf, bf);

            it.WriteB<double>(nd);
            NSFX_TEST_EXPECT_EQ(it1.ReadB<double>(), nd);
            it -= sizeof (double);
            it1 -= sizeof (double);
            NSFX_TEST_EXPECT_EQ(*pd, bd);

            it.WriteB(reinterpret_cast<const uint8_t*>(&n64), sizeof (uint64_t));
            it1.ReadB(reinterpret_cast<uint8_t*>(&t64), sizeof (uint64_t));
            it -= sizeof (uint64_t);
            it1 -= sizeof (uint64_t);
            NSFX_TEST_EXPECT_EQ(t64, n64);
            NSFX_TEST_EXPECT_EQ(*p64, b64);

        }

        NSFX_TEST_CASE(AroundBuffer)
        {
            nsfx::Buffer b0(1000, 700, 300);
            // [700 s zs 300 ze e 300]
            b0.AddAtStart(100);
            b0.AddAtEnd(100);
            // [600 s 100 zs 300 ze 100 e 200]
            nsfx::BufferIterator it = b0.begin();
            nsfx::ConstBufferIterator it1 = b0.cbegin();
            const nsfx::Buffer::BufferStorage* storage = b0.GetStorage();
            const uint8_t* p8 = reinterpret_cast<const uint8_t* >(storage->bytes_ + b0.GetStart());

            // Write header area.
            NSFX_TEST_EXPECT_EQ(it1.GetCursor(), 600);
            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t n8 = 0xfe + i;
                it.Write<uint8_t>(n8);
                NSFX_TEST_EXPECT_EQ(*p8, n8);
                ++it1;
                ++p8;
            }

            // Read zero area.
            NSFX_TEST_EXPECT_EQ(it1.GetCursor(), 700);
            for (size_t i = 0; i < 300; ++i)
            {
                NSFX_TEST_EXPECT_EQ(it1.Read<uint8_t>(), 0);
                ++it;
                ++p8;
            }

            // Write trailer area.
            NSFX_TEST_ASSERT_EQ(it1.GetCursor(), 1000);
            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t n8 = 0xef + i;
                it.Write<uint8_t>(n8);
                NSFX_TEST_EXPECT_EQ(*p8, n8);
                ++it1;
                ++p8;
            }
        }
    }/*}}}*/
}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

