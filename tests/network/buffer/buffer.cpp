/**
 * @file
 *
 * @brief Test Buffer.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-05-26
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/network/buffer.h>
#include <iostream>


NSFX_TEST_SUITE(Buffer)
{
    NSFX_TEST_SUITE(Ctor)/*{{{*/
    {
        NSFX_TEST_CASE(Ctor0)
        {
            nsfx::Buffer b0;
            NSFX_TEST_EXPECT_EQ(b0.GetSize(), 0);
            NSFX_TEST_EXPECT_EQ(b0.GetInternalSize(), b0.GetSize());
            NSFX_TEST_EXPECT_EQ(b0.GetCapacity(), 0);
            NSFX_TEST_EXPECT_EQ(b0.GetStart(), 0);
            NSFX_TEST_EXPECT_EQ(b0.GetEnd(), 0);
            NSFX_TEST_EXPECT(!b0.GetStorage());
        }

        NSFX_TEST_CASE(Ctor1)
        {
            nsfx::Buffer b0(1000);
            // [1000 s zs ze e]
            NSFX_TEST_EXPECT_EQ(b0.GetSize(), 0);
            NSFX_TEST_EXPECT_EQ(b0.GetInternalSize(), b0.GetSize());
            NSFX_TEST_EXPECT_EQ(b0.GetCapacity(), 1000);
            NSFX_TEST_EXPECT_EQ(b0.GetStart(), 1000);
            NSFX_TEST_EXPECT_EQ(b0.GetEnd(), 1000);
            NSFX_TEST_ASSERT(b0.GetStorage());
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->capacity_, 1000);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyStart_, 1000);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyEnd_, 1000);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
        }

        NSFX_TEST_CASE(Ctor2)
        {
            nsfx::Buffer b0(1000, 300);
            // [1000 s zs 300 ze e]
            NSFX_TEST_EXPECT_EQ(b0.GetSize(), 300);
            NSFX_TEST_EXPECT_EQ(b0.GetInternalSize(), b0.GetSize());
            NSFX_TEST_EXPECT_EQ(b0.GetCapacity(), 1300);
            NSFX_TEST_EXPECT_EQ(b0.GetStart(), 1000);
            NSFX_TEST_EXPECT_EQ(b0.GetEnd(), 1300);
            NSFX_TEST_ASSERT(b0.GetStorage());
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->capacity_, 1300);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyStart_, 1000);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyEnd_, 1300);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
        }

        NSFX_TEST_CASE(Ctor3)
        {
            nsfx::Buffer b0(1000, 700, 300);
            // [700 s zs 300 ze e 300]
            NSFX_TEST_EXPECT_EQ(b0.GetSize(), 300);
            NSFX_TEST_EXPECT_EQ(b0.GetInternalSize(), b0.GetSize());
            NSFX_TEST_EXPECT_EQ(b0.GetCapacity(), 1300);
            NSFX_TEST_EXPECT_EQ(b0.GetStart(), 700);
            NSFX_TEST_EXPECT_EQ(b0.GetEnd(), 1000);
            NSFX_TEST_ASSERT(b0.GetStorage());
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->capacity_, 1300);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyStart_, 700);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyEnd_, 1000);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
        }

        NSFX_TEST_CASE(FromConstBuffer)
        {
            nsfx::Buffer b0(300);
            b0.AddAtStart(300);
            auto it0 = b0.begin();
            for (size_t i = 0; i < 300; ++i)
            {
                uint8_t v = 0xfe + i;
                it0.Write<uint8_t>(v);
            }
            it0 = b0.begin();

            nsfx::ConstBuffer cb0(b0);
            nsfx::Buffer b1(cb0);
            auto it1 = b1.cbegin();
            for (size_t i = 0; i < 300; ++i)
            {
                uint8_t v = 0xfe + i;
                NSFX_TEST_EXPECT_EQ(it1.Read<uint8_t>(), v);
            }
        }

        NSFX_TEST_CASE(FromConstZcBuffer)
        {
            nsfx::ZcBuffer b0(300);
            b0.AddAtStart(300);
            auto it0 = b0.begin();
            for (size_t i = 0; i < 300; ++i)
            {
                uint8_t v = 0xfe + i;
                it0.Write<uint8_t>(v);
            }
            it0 = b0.begin();

            nsfx::Buffer b1(b0);
            auto it1 = b1.cbegin();
            for (size_t i = 0; i < 300; ++i)
            {
                uint8_t v = 0xfe + i;
                NSFX_TEST_EXPECT_EQ(it1.Read<uint8_t>(), v);
            }
        }

        NSFX_TEST_CASE(FromConstTagBuffer)
        {
            nsfx::TagBuffer b0(300);
            auto it0 = b0.begin();
            for (size_t i = 0; i < 300; ++i)
            {
                uint8_t v = 0xfe + i;
                it0.Write<uint8_t>(v);
            }
            it0 = b0.begin();

            nsfx::Buffer b1(b0);
            auto it1 = b1.cbegin();
            for (size_t i = 0; i < 300; ++i)
            {
                uint8_t v = 0xfe + i;
                NSFX_TEST_EXPECT_EQ(it1.Read<uint8_t>(), v);
            }
        }

    }/*}}}*/

    NSFX_TEST_SUITE(Copy)/*{{{*/
    {
        NSFX_TEST_CASE(CopyCtor)
        {
            nsfx::Buffer b0(1000, 700, 300);
            // [700 s zs 300 ze e 300]
            b0.AddAtStart(100);
            // [600 s 100 zs 300 ze e 300]
            b0.AddAtEnd(100);
            // [600 s 100 zs 300 ze 100 e 200]
            nsfx::Buffer b1(b0);
            //
            NSFX_TEST_EXPECT_EQ(b0.GetSize(), 500);
            NSFX_TEST_EXPECT_EQ(b0.GetInternalSize(), b0.GetSize());
            NSFX_TEST_EXPECT_EQ(b0.GetCapacity(), 1300);
            NSFX_TEST_EXPECT_EQ(b0.GetStart(), 600);
            NSFX_TEST_EXPECT_EQ(b0.GetEnd(), 1100);
            NSFX_TEST_ASSERT(b0.GetStorage());
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->capacity_, 1300);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyStart_, 600);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyEnd_, 1100);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 2);
            //
            NSFX_TEST_EXPECT_EQ(b1.GetSize(), 500);
            NSFX_TEST_EXPECT_EQ(b1.GetInternalSize(), b1.GetSize());
            NSFX_TEST_EXPECT_EQ(b1.GetCapacity(), 1300);
            NSFX_TEST_EXPECT_EQ(b1.GetStart(), 600);
            NSFX_TEST_EXPECT_EQ(b1.GetEnd(), 1100);
            NSFX_TEST_ASSERT(b1.GetStorage());
            NSFX_TEST_EXPECT_EQ(b0.GetStorage(), b1.GetStorage());
            NSFX_TEST_EXPECT_EQ(b1.GetStorage()->capacity_, 1300);
            NSFX_TEST_EXPECT_EQ(b1.GetStorage()->dirtyStart_, 600);
            NSFX_TEST_EXPECT_EQ(b1.GetStorage()->dirtyEnd_, 1100);
            NSFX_TEST_EXPECT_EQ(b1.GetStorage()->refCount_, 2);
        }

        NSFX_TEST_CASE(CopyAssign)
        {
            nsfx::Buffer b0(1000, 700, 300);
            // [700 s zs 300 ze e 300]
            b0.AddAtStart(100);
            // [600 s 100 zs 300 ze e 300]
            b0.AddAtEnd(100);
            // [600 s 100 zs 300 ze 100 e 200]
            nsfx::Buffer b1(2000, 600);
            nsfx::Buffer b2(b1); // Before change b1, use make a backup in b2.
            b1 = b0;
            //
            NSFX_TEST_EXPECT_EQ(b0.GetSize(), 500);
            NSFX_TEST_EXPECT_EQ(b0.GetInternalSize(), b0.GetSize());
            NSFX_TEST_EXPECT_EQ(b0.GetCapacity(), 1300);
            NSFX_TEST_EXPECT_EQ(b0.GetStart(), 600);
            NSFX_TEST_EXPECT_EQ(b0.GetEnd(), 1100);
            NSFX_TEST_ASSERT(b0.GetStorage());
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->capacity_, 1300);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyStart_, 600);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyEnd_, 1100);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 2);
            //
            NSFX_TEST_EXPECT_EQ(b1.GetSize(), 500);
            NSFX_TEST_EXPECT_EQ(b1.GetInternalSize(), b1.GetSize());
            NSFX_TEST_EXPECT_EQ(b1.GetCapacity(), 1300);
            NSFX_TEST_EXPECT_EQ(b1.GetStart(), 600);
            NSFX_TEST_EXPECT_EQ(b1.GetEnd(), 1100);
            NSFX_TEST_ASSERT(b1.GetStorage());
            NSFX_TEST_EXPECT_EQ(b0.GetStorage(), b1.GetStorage());
            NSFX_TEST_EXPECT_EQ(b1.GetStorage()->capacity_, 1300);
            NSFX_TEST_EXPECT_EQ(b1.GetStorage()->dirtyStart_, 600);
            NSFX_TEST_EXPECT_EQ(b1.GetStorage()->dirtyEnd_, 1100);
            NSFX_TEST_EXPECT_EQ(b1.GetStorage()->refCount_, 2);
            //
            NSFX_TEST_EXPECT_EQ(b2.GetSize(), 600);
            NSFX_TEST_EXPECT_EQ(b2.GetInternalSize(), b2.GetSize());
            NSFX_TEST_EXPECT_EQ(b2.GetCapacity(), 2600);
            NSFX_TEST_EXPECT_EQ(b2.GetStart(), 2000);
            NSFX_TEST_EXPECT_EQ(b2.GetEnd(), 2600);
            NSFX_TEST_ASSERT(b2.GetStorage());
            NSFX_TEST_EXPECT_EQ(b2.GetStorage()->capacity_, 2600);
            NSFX_TEST_EXPECT_EQ(b2.GetStorage()->dirtyStart_, 2000);
            NSFX_TEST_EXPECT_EQ(b2.GetStorage()->dirtyEnd_, 2600);
            NSFX_TEST_EXPECT_EQ(b2.GetStorage()->refCount_, 1);
        }
    }/*}}}*/

    NSFX_TEST_SUITE(Move)/*{{{*/
    {
        NSFX_TEST_CASE(MoveCtor)
        {
            nsfx::Buffer b0(1000, 700, 300);
            // [700 s zs 300 ze e 300]
            b0.AddAtStart(100);
            // [600 s 100 zs 300 ze e 300]
            b0.AddAtEnd(100);
            // [600 s 100 zs 300 ze 100 e 200]
            nsfx::Buffer b1(std::move(b0));
            //
            NSFX_TEST_EXPECT_EQ(b0.GetSize(), 0);
            NSFX_TEST_EXPECT_EQ(b0.GetInternalSize(), b0.GetSize());
            NSFX_TEST_EXPECT_EQ(b0.GetCapacity(), 0);
            NSFX_TEST_EXPECT_EQ(b0.GetStart(), 0);
            NSFX_TEST_EXPECT_EQ(b0.GetEnd(), 0);
            NSFX_TEST_ASSERT(!b0.GetStorage());
            //
            NSFX_TEST_EXPECT_EQ(b1.GetSize(), 500);
            NSFX_TEST_EXPECT_EQ(b1.GetInternalSize(), b1.GetSize());
            NSFX_TEST_EXPECT_EQ(b1.GetCapacity(), 1300);
            NSFX_TEST_EXPECT_EQ(b1.GetStart(), 600);
            NSFX_TEST_EXPECT_EQ(b1.GetEnd(), 1100);
            NSFX_TEST_ASSERT(b1.GetStorage());
            NSFX_TEST_EXPECT_EQ(b1.GetStorage()->capacity_, 1300);
            NSFX_TEST_EXPECT_EQ(b1.GetStorage()->dirtyStart_, 600);
            NSFX_TEST_EXPECT_EQ(b1.GetStorage()->dirtyEnd_, 1100);
            NSFX_TEST_EXPECT_EQ(b1.GetStorage()->refCount_, 1);
        }

        NSFX_TEST_CASE(MoveAssign)
        {
            nsfx::Buffer b0(1000, 700, 300);
            // [700 s zs 300 ze e 300]
            b0.AddAtStart(100);
            // [600 s 100 zs 300 ze e 300]
            b0.AddAtEnd(100);
            // [600 s 100 zs 300 ze 100 e 200]
            nsfx::Buffer b1(2000, 600);
            nsfx::Buffer b2(b1); // Before change b1, use make a backup in b2.
            b1 = std::move(b0);
            //
            NSFX_TEST_EXPECT_EQ(b0.GetSize(), 0);
            NSFX_TEST_EXPECT_EQ(b0.GetInternalSize(), b0.GetSize());
            NSFX_TEST_EXPECT_EQ(b0.GetCapacity(), 0);
            NSFX_TEST_EXPECT_EQ(b0.GetStart(), 0);
            NSFX_TEST_EXPECT_EQ(b0.GetEnd(), 0);
            NSFX_TEST_ASSERT(!b0.GetStorage());
            //
            NSFX_TEST_EXPECT_EQ(b1.GetSize(), 500);
            NSFX_TEST_EXPECT_EQ(b1.GetInternalSize(), b1.GetSize());
            NSFX_TEST_EXPECT_EQ(b1.GetCapacity(), 1300);
            NSFX_TEST_EXPECT_EQ(b1.GetStart(), 600);
            NSFX_TEST_EXPECT_EQ(b1.GetEnd(), 1100);
            NSFX_TEST_ASSERT(b1.GetStorage());
            NSFX_TEST_EXPECT_EQ(b1.GetStorage()->capacity_, 1300);
            NSFX_TEST_EXPECT_EQ(b1.GetStorage()->dirtyStart_, 600);
            NSFX_TEST_EXPECT_EQ(b1.GetStorage()->dirtyEnd_, 1100);
            NSFX_TEST_EXPECT_EQ(b1.GetStorage()->refCount_, 1);
            //
            NSFX_TEST_EXPECT_EQ(b2.GetSize(), 600);
            NSFX_TEST_EXPECT_EQ(b2.GetInternalSize(), b2.GetSize());
            NSFX_TEST_EXPECT_EQ(b2.GetCapacity(), 2600);
            NSFX_TEST_EXPECT_EQ(b2.GetStart(), 2000);
            NSFX_TEST_EXPECT_EQ(b2.GetEnd(), 2600);
            NSFX_TEST_ASSERT(b2.GetStorage());
            NSFX_TEST_EXPECT_EQ(b2.GetStorage()->capacity_, 2600);
            NSFX_TEST_EXPECT_EQ(b2.GetStorage()->dirtyStart_, 2000);
            NSFX_TEST_EXPECT_EQ(b2.GetStorage()->dirtyEnd_, 2600);
            NSFX_TEST_EXPECT_EQ(b2.GetStorage()->refCount_, 1);
        }
    }/*}}}*/

    NSFX_TEST_SUITE(CopyTo)/*{{{*/
    {
        NSFX_TEST_CASE(Segmented)
        {
            nsfx::Buffer b0(1000, 700, 300);
            // [700 s zs 300 ze e 300]
            b0.AddAtStart(100);
            // [600 s 100 zs 300 ze e 300]
            b0.AddAtEnd(100);
            // [600 s 100 zs 300 ze 100 e 200]

            nsfx::BufferIterator it = b0.begin();
            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t v = 0xfe + i;
                it.Write<uint8_t>(v);
            }
            it += 300;
            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t v = 0xef + i;
                it.Write<uint8_t>(v);
            }

            size_t size = b0.GetSize();
            uint8_t* byte = new uint8_t[size];
            b0.CopyTo(byte, size);

            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t v = 0xfe + i;
                NSFX_TEST_EXPECT_EQ(byte[i], v);
            }
            for (size_t i = 100; i < 400; ++i)
            {
                NSFX_TEST_EXPECT_EQ(byte[i], 0);
            }
            for (size_t i = 400; i < 500; ++i)
            {
                uint8_t v = 0xef + i - 400;
                NSFX_TEST_EXPECT_EQ(byte[i], v);
            }
        }

        NSFX_TEST_CASE(Continuous)
        {
            nsfx::Buffer b0(1000);
            // [1000 s zs ze e]
            b0.AddAtEnd(100);
            // [900 s zs ze 100 e]
            b0.AddAtStart(100);
            // [800 s 100 zs ze 100 e]

            nsfx::BufferIterator it = b0.begin();
            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t v = 0xfe + i;
                it.Write<uint8_t>(v);
            }
            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t v = 0xef + i;
                it.Write<uint8_t>(v);
            }

            size_t size = b0.GetSize();
            uint8_t* byte = new uint8_t[size];
            b0.CopyTo(byte, size);

            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t v = 0xfe + i;
                NSFX_TEST_EXPECT_EQ(byte[i], v);
            }
            for (size_t i = 100; i < 200; ++i)
            {
                uint8_t v = 0xef + i - 100;
                NSFX_TEST_EXPECT_EQ(byte[i], v);
            }
        }
    }/*}}}*/

    NSFX_TEST_SUITE(AddAtStart)/*{{{*/
    {
        NSFX_TEST_SUITE(Add0)
        {
            NSFX_TEST_CASE(FromEmpty)
            {
                nsfx::Buffer b0;
                b0.AddAtStart(0);
                // Nothing was done, the storage is still nullptr.
                NSFX_TEST_EXPECT_EQ(b0.GetSize(), 0);
                NSFX_TEST_EXPECT_EQ(b0.GetInternalSize(), b0.GetSize());
                NSFX_TEST_EXPECT_EQ(b0.GetCapacity(), 0);
                NSFX_TEST_EXPECT_EQ(b0.GetStart(), 0);
                NSFX_TEST_EXPECT_EQ(b0.GetEnd(), 0);
                NSFX_TEST_ASSERT(!b0.GetStorage());
            }

            NSFX_TEST_CASE(FromNonEmpty)
            {
                nsfx::Buffer b0(1000, 300);
                const nsfx::BufferStorage* s0 = b0.GetStorage();
                b0.AddAtStart(0);
                // Nothing was done.
                NSFX_TEST_EXPECT_EQ(b0.GetSize(), 300);
                NSFX_TEST_EXPECT_EQ(b0.GetInternalSize(), b0.GetSize());
                NSFX_TEST_EXPECT_EQ(b0.GetCapacity(), 1300);
                NSFX_TEST_EXPECT_EQ(b0.GetStart(), 1000);
                NSFX_TEST_EXPECT_EQ(b0.GetEnd(), 1300);
                NSFX_TEST_ASSERT(b0.GetStorage());
                NSFX_TEST_EXPECT_EQ(b0.GetStorage(), s0);
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->capacity_, 1300);
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyStart_, 1000);
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyEnd_, 1300);
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
            }
        }

        NSFX_TEST_SUITE(AddNonZero)
        {
            NSFX_TEST_CASE(FromEmpty)
            {
                nsfx::Buffer b0;
                b0.AddAtStart(300);
                // [s 300 zs ze e]
                // Reallocate just enough capacity.
                NSFX_TEST_EXPECT_EQ(b0.GetSize(), 300);
                NSFX_TEST_EXPECT_EQ(b0.GetInternalSize(), b0.GetSize());
                NSFX_TEST_EXPECT_EQ(b0.GetCapacity(), 300);
                NSFX_TEST_EXPECT_EQ(b0.GetStart(), 0);
                NSFX_TEST_EXPECT_EQ(b0.GetEnd(), 300);
                NSFX_TEST_ASSERT(b0.GetStorage());
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->capacity_, 300);
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyStart_, 0);
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyEnd_, 300);
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
            }

            NSFX_TEST_SUITE(NotShared)
            {
                NSFX_TEST_CASE(PreHeaderBigEnough)
                {
                    nsfx::Buffer b0(1000, 700, 300);
                    // [700 s zs 300 ze e 300]
                    const nsfx::BufferStorage* s0 = b0.GetStorage();
                    b0.AddAtEnd(100);
                    // [700 s zs 300 ze 100 e 200]
                    b0.AddAtStart(700);
                    // [s 700 zs 300 ze 100 e 200]
                    // Adjust offset.
                    NSFX_TEST_EXPECT_EQ(b0.GetSize(), 1100);
                    NSFX_TEST_EXPECT_EQ(b0.GetInternalSize(), b0.GetSize());
                    NSFX_TEST_EXPECT_EQ(b0.GetCapacity(), 1300);
                    NSFX_TEST_EXPECT_EQ(b0.GetStart(), 0);
                    NSFX_TEST_EXPECT_EQ(b0.GetEnd(), 1100);
                    NSFX_TEST_ASSERT(b0.GetStorage());
                    NSFX_TEST_EXPECT_EQ(b0.GetStorage(), s0);
                    NSFX_TEST_EXPECT_EQ(b0.GetStorage()->capacity_, 1300);
                    NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyStart_, 0);
                    NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyEnd_, 1100);
                    NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
                }

                NSFX_TEST_CASE(CapacityBigEnough)
                {
                    nsfx::Buffer b0(1000, 700, 300);
                    // [700 s zs 300 ze e 300]
                    const nsfx::BufferStorage* s0 = b0.GetStorage();
                    b0.AddAtEnd(100);
                    // [700 s zs 300 ze 100 e 200]
                    b0.AddAtStart(900);
                    // [s 900 zs 300 ze 100 e]
                    // Move memory.
                    NSFX_TEST_EXPECT_EQ(b0.GetSize(), 1300);
                    NSFX_TEST_EXPECT_EQ(b0.GetInternalSize(), b0.GetSize());
                    NSFX_TEST_EXPECT_EQ(b0.GetCapacity(), 1300);
                    NSFX_TEST_EXPECT_EQ(b0.GetStart(), 0);
                    NSFX_TEST_EXPECT_EQ(b0.GetEnd(), 1300);
                    NSFX_TEST_ASSERT(b0.GetStorage());
                    NSFX_TEST_EXPECT_EQ(b0.GetStorage(), s0);
                    NSFX_TEST_EXPECT_EQ(b0.GetStorage()->capacity_, 1300);
                    NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyStart_, 0);
                    NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyEnd_, 1300);
                    NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
                }

                NSFX_TEST_CASE(CapacityNotEnough)
                {
                    nsfx::Buffer b0(1000, 700, 300);
                    // [700 s zs 300 ze e 300 ]
                    const nsfx::BufferStorage* s0 = b0.GetStorage();
                    b0.AddAtEnd(100);
                    // [700 s zs 300 ze 100 e 200 ]
                    b0.AddAtStart(1000);
                    // [s 1000 zs 300 ze 100 e]
                    // Reallocate just enough capacity.
                    NSFX_TEST_EXPECT_EQ(b0.GetSize(), 1400);
                    NSFX_TEST_EXPECT_EQ(b0.GetInternalSize(), b0.GetSize());
                    NSFX_TEST_EXPECT_EQ(b0.GetCapacity(), 1400);
                    NSFX_TEST_EXPECT_EQ(b0.GetStart(), 0);
                    NSFX_TEST_EXPECT_EQ(b0.GetEnd(), 1400);
                    NSFX_TEST_ASSERT(b0.GetStorage());
                    NSFX_TEST_EXPECT_NE(b0.GetStorage(), s0);
                    NSFX_TEST_EXPECT_EQ(b0.GetStorage()->capacity_, 1400);
                    NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyStart_, 0);
                    NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyEnd_, 1400);
                    NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
                }
            }

            NSFX_TEST_SUITE(Shared)
            {
                NSFX_TEST_SUITE(NotDirty)
                {
                    NSFX_TEST_CASE(PreHeaderBigEnough)
                    {
                        nsfx::Buffer b0(1000, 700, 300);
                        // b0 [700 s zs 300 ze e 300]
                        b0.AddAtStart(100);
                        b0.AddAtEnd(100);
                        // b0 [600 s 100 zs 300 ze 100 e 200]
                        nsfx::Buffer b1(b0);
                        b1.AddAtEnd(100); // dirty at end, but not dirty at start
                        // b1 [600 s 100 zs 300 ze 200 e 100]
                        const nsfx::BufferStorage* s0 = b0.GetStorage();
                        b0.AddAtStart(600);
                        // b0 [s 700 zs 300 ze 100 e 200]
                        // Adjust offset.
                        NSFX_TEST_EXPECT_EQ(b0.GetSize(), 1100);
                        NSFX_TEST_EXPECT_EQ(b0.GetInternalSize(), b0.GetSize());
                        NSFX_TEST_EXPECT_EQ(b0.GetCapacity(), 1300);
                        NSFX_TEST_EXPECT_EQ(b0.GetStart(), 0);
                        NSFX_TEST_EXPECT_EQ(b0.GetEnd(), 1100);
                        NSFX_TEST_ASSERT(b0.GetStorage());
                        NSFX_TEST_EXPECT_EQ(b0.GetStorage(), s0);
                        NSFX_TEST_EXPECT_EQ(b0.GetStorage()->capacity_, 1300);
                        NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyStart_, 0);
                        NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyEnd_, 1200);
                        NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 2);
                    }

                    NSFX_TEST_CASE(PreHeaderNotEnough)
                    {
                        nsfx::Buffer b0(1000, 700, 300);
                        // b0 [700 s zs 300 ze e 300]
                        b0.AddAtStart(100);
                        b0.AddAtEnd(100);
                        // b0 [600 s 100 zs 300 ze 100 e 200]
                        nsfx::Buffer b1(b0);
                        b1.AddAtEnd(100); // dirty at end, but not dirty at start
                        // b1 [600 s 100 zs 300 ze 200 e 100]
                        const nsfx::BufferStorage* s0 = b0.GetStorage();
                        b0.AddAtStart(601);
                        // b0 [s 701 zs 300 ze 100 e]
                        // Reallocate just enough capacity.
                        NSFX_TEST_EXPECT_EQ(b0.GetSize(), 1101);
                        NSFX_TEST_EXPECT_EQ(b0.GetInternalSize(), b0.GetSize());
                        NSFX_TEST_EXPECT_EQ(b0.GetCapacity(), 1101);
                        NSFX_TEST_EXPECT_EQ(b0.GetStart(), 0);
                        NSFX_TEST_EXPECT_EQ(b0.GetEnd(), 1101);
                        NSFX_TEST_ASSERT(b0.GetStorage());
                        NSFX_TEST_EXPECT_NE(b0.GetStorage(), s0);
                        NSFX_TEST_EXPECT_EQ(b0.GetStorage()->capacity_, 1101);
                        NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyStart_, 0);
                        NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyEnd_, 1101);
                        NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
                    }
                }

                NSFX_TEST_SUITE(Dirty)
                {
                    NSFX_TEST_CASE(PreHeaderBigEnough)
                    {
                        nsfx::Buffer b0(1000, 700, 300);
                        // b0 [700 s zs 300 ze e 300]
                        b0.AddAtStart(100);
                        b0.AddAtEnd(100);
                        // b0 [600 s 100 zs 300 ze 100 e 200]
                        nsfx::Buffer b1(b0);
                        b1.AddAtStart(1); // dirty at start
                        // b1 [599 s 101 zs 300 ze 200 e 100]
                        const nsfx::BufferStorage* s0 = b0.GetStorage();
                        b0.AddAtStart(600);
                        // b0 [s 700 zs 300 ze 100 e]
                        // Reallocate (preserve capacity).
                        NSFX_TEST_EXPECT_EQ(b0.GetSize(), 1100);
                        NSFX_TEST_EXPECT_EQ(b0.GetInternalSize(), b0.GetSize());
                        NSFX_TEST_EXPECT_EQ(b0.GetCapacity(), 1300);
                        NSFX_TEST_EXPECT_EQ(b0.GetStart(), 0);
                        NSFX_TEST_EXPECT_EQ(b0.GetEnd(), 1100);
                        NSFX_TEST_ASSERT(b0.GetStorage());
                        NSFX_TEST_EXPECT_NE(b0.GetStorage(), s0);
                        NSFX_TEST_EXPECT_EQ(b0.GetStorage()->capacity_, 1300);
                        NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyStart_, 0);
                        NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyEnd_, 1100);
                        NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
                    }

                    NSFX_TEST_CASE(PreHeaderNotEnough)
                    {
                        nsfx::Buffer b0(1000, 700, 300);
                        // b0 [700 s zs 300 ze e 300]
                        b0.AddAtStart(100);
                        b0.AddAtEnd(100);
                        // b0 [600 s 100 zs 300 ze 100 e 200]
                        nsfx::Buffer b1(b0);
                        b1.AddAtStart(1); // dirty at start
                        // b1 [599 s 101 zs 300 ze 200 e 100]
                        const nsfx::BufferStorage* s0 = b0.GetStorage();
                        b0.AddAtStart(601);
                        // b0 [s 701 zs 300 ze 100 e]
                        // Reallocate.
                        NSFX_TEST_EXPECT_EQ(b0.GetSize(), 1101);
                        NSFX_TEST_EXPECT_EQ(b0.GetInternalSize(), b0.GetSize());
                        NSFX_TEST_EXPECT_EQ(b0.GetCapacity(), 1101);
                        NSFX_TEST_EXPECT_EQ(b0.GetStart(), 0);
                        NSFX_TEST_EXPECT_EQ(b0.GetEnd(), 1101);
                        NSFX_TEST_ASSERT(b0.GetStorage());
                        NSFX_TEST_EXPECT_NE(b0.GetStorage(), s0);
                        NSFX_TEST_EXPECT_EQ(b0.GetStorage()->capacity_, 1101);
                        NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyStart_, 0);
                        NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyEnd_, 1101);
                        NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
                    }
                }
            }
        }

        NSFX_TEST_CASE(AddMemory)
        {
            uint8_t s[100];
            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t v = 0xfe + i;
                s[i] = v;
            }

            nsfx::Buffer b0(1000, 700, 300);
            // b0 [700 s zs 300 ze e 300]
            b0.AddAtStart(s, 100);
            b0.AddAtEnd(100);
            // b0 [600 s 100 zs 300 ze 100 e 200]
            auto it = b0.begin();
            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t v = 0xfe + i;
                if (i == 99)
                {
                    i = 99;
                }
                NSFX_TEST_EXPECT_EQ(it.Read<uint8_t>(), v) << i;
            }
            for (size_t i = 0; i < 300; ++i)
            {
                NSFX_TEST_EXPECT_EQ(it.Read<uint8_t>(), 0);
            }
        }

        NSFX_TEST_CASE(AddBuffer)
        {
            nsfx::Buffer s(1000, 700, 0);
            s.AddAtStart(100);
            s.AddAtEnd(100);
            // s [600 s 100 zs ze 100 e 200]
            auto it = s.begin();
            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t v = 0xfe + i;
                it.Write<uint8_t>(v);
            }
            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t v = 0xef + i;
                it.Write<uint8_t>(v);
            }

            nsfx::Buffer b0(1000, 700, 300);
            b0.AddAtEnd(100);
            b0.AddAtStart(s);
            // s [100 s 200 zs 300 ze 100 e 200]
            it = b0.begin();
            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t v = 0xfe + i;
                NSFX_TEST_EXPECT_EQ(it.Read<uint8_t>(), v);
            }
            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t v = 0xef + i;
                NSFX_TEST_EXPECT_EQ(it.Read<uint8_t>(), v);
            }
            for (size_t i = 0; i < 300; ++i)
            {
                NSFX_TEST_EXPECT_EQ(it.Read<uint8_t>(), 0);
            }
        }
    }/*}}}*/

    NSFX_TEST_SUITE(AddAtEnd)/*{{{*/
    {
        NSFX_TEST_SUITE(Add0)
        {
            NSFX_TEST_CASE(FromEmpty)
            {
                nsfx::Buffer b0;
                b0.AddAtEnd(0);
                // Nothing was done, the storage is still nullptr.
                NSFX_TEST_EXPECT_EQ(b0.GetSize(), 0);
                NSFX_TEST_EXPECT_EQ(b0.GetInternalSize(), b0.GetSize());
                NSFX_TEST_EXPECT_EQ(b0.GetCapacity(), 0);
                NSFX_TEST_EXPECT_EQ(b0.GetStart(), 0);
                NSFX_TEST_EXPECT_EQ(b0.GetEnd(), 0);
                NSFX_TEST_ASSERT(!b0.GetStorage());
            }

            NSFX_TEST_CASE(FromNonEmpty)
            {
                nsfx::Buffer b0(1000, 300);
                const nsfx::BufferStorage* s0 = b0.GetStorage();
                b0.AddAtEnd(0);
                // Nothing was done.
                NSFX_TEST_EXPECT_EQ(b0.GetSize(), 300);
                NSFX_TEST_EXPECT_EQ(b0.GetInternalSize(), b0.GetSize());
                NSFX_TEST_EXPECT_EQ(b0.GetCapacity(), 1300);
                NSFX_TEST_EXPECT_EQ(b0.GetStart(), 1000);
                NSFX_TEST_EXPECT_EQ(b0.GetEnd(), 1300);
                NSFX_TEST_ASSERT(b0.GetStorage());
                NSFX_TEST_EXPECT_EQ(b0.GetStorage(), s0);
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->capacity_, 1300);
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyStart_, 1000);
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyEnd_, 1300);
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
            }
        }

        NSFX_TEST_SUITE(AddNonZero)
        {
            NSFX_TEST_CASE(FromEmpty)
            {
                nsfx::Buffer b0;
                b0.AddAtEnd(300);
                // [s zs ze 300 e]
                // Reallocate just enough capacity.
                NSFX_TEST_EXPECT_EQ(b0.GetSize(), 300);
                NSFX_TEST_EXPECT_EQ(b0.GetInternalSize(), b0.GetSize());
                NSFX_TEST_EXPECT_EQ(b0.GetCapacity(), 300);
                NSFX_TEST_EXPECT_EQ(b0.GetStart(), 0);
                NSFX_TEST_EXPECT_EQ(b0.GetEnd(), 300);
                NSFX_TEST_ASSERT(b0.GetStorage());
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->capacity_, 300);
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyStart_, 0);
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyEnd_, 300);
                NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
            }

            NSFX_TEST_SUITE(NotShared)
            {
                NSFX_TEST_CASE(PostTrailerBigEnough)
                {
                    nsfx::Buffer b0(1000, 700, 300);
                    // [700 s zs 300 ze e 300]
                    const nsfx::BufferStorage* s0 = b0.GetStorage();
                    b0.AddAtStart(100);
                    // [600 s 100 zs 300 ze e 300]
                    b0.AddAtEnd(300);
                    // [600 s 100 zs 300 ze 300 e]
                    // Adjust offset.
                    NSFX_TEST_EXPECT_EQ(b0.GetSize(), 700);
                    NSFX_TEST_EXPECT_EQ(b0.GetInternalSize(), b0.GetSize());
                    NSFX_TEST_EXPECT_EQ(b0.GetCapacity(), 1300);
                    NSFX_TEST_EXPECT_EQ(b0.GetStart(), 600);
                    NSFX_TEST_EXPECT_EQ(b0.GetEnd(), 1300);
                    NSFX_TEST_ASSERT(b0.GetStorage());
                    NSFX_TEST_EXPECT_EQ(b0.GetStorage(), s0);
                    NSFX_TEST_EXPECT_EQ(b0.GetStorage()->capacity_, 1300);
                    NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyStart_, 600);
                    NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyEnd_, 1300);
                    NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
                }

                NSFX_TEST_CASE(CapacityBigEnough)
                {
                    nsfx::Buffer b0(1000, 700, 300);
                    // [700 s zs 300 ze e 300]
                    const nsfx::BufferStorage* s0 = b0.GetStorage();
                    b0.AddAtStart(100);
                    // [600 s 100 zs 300 ze e 300]
                    b0.AddAtEnd(900);
                    // [s 100 zs 300 ze 900 e]
                    // Move memory.
                    NSFX_TEST_EXPECT_EQ(b0.GetSize(), 1300);
                    NSFX_TEST_EXPECT_EQ(b0.GetInternalSize(), b0.GetSize());
                    NSFX_TEST_EXPECT_EQ(b0.GetCapacity(), 1300);
                    NSFX_TEST_EXPECT_EQ(b0.GetStart(), 0);
                    NSFX_TEST_EXPECT_EQ(b0.GetEnd(), 1300);
                    NSFX_TEST_ASSERT(b0.GetStorage());
                    NSFX_TEST_EXPECT_EQ(b0.GetStorage(), s0);
                    NSFX_TEST_EXPECT_EQ(b0.GetStorage()->capacity_, 1300);
                    NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyStart_, 0);
                    NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyEnd_, 1300);
                    NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
                }

                NSFX_TEST_CASE(CapacityNotEnough)
                {
                    nsfx::Buffer b0(1000, 700, 300);
                    // [ 700 s zs 300 ze e 300 ]
                    const nsfx::BufferStorage* s0 = b0.GetStorage();
                    b0.AddAtStart(100);
                    // [ 600 s 100 zs 300 ze e 300 ]
                    b0.AddAtEnd(1000);
                    // [ s 100 zs 300 ze 1000 e]
                    // Reallocate just enough capacity.
                    NSFX_TEST_EXPECT_EQ(b0.GetSize(), 1400);
                    NSFX_TEST_EXPECT_EQ(b0.GetInternalSize(), b0.GetSize());
                    NSFX_TEST_EXPECT_EQ(b0.GetCapacity(), 1400);
                    NSFX_TEST_EXPECT_EQ(b0.GetStart(), 0);
                    NSFX_TEST_EXPECT_EQ(b0.GetEnd(), 1400);
                    NSFX_TEST_ASSERT(b0.GetStorage());
                    NSFX_TEST_EXPECT_NE(b0.GetStorage(), s0);
                    NSFX_TEST_EXPECT_EQ(b0.GetStorage()->capacity_, 1400);
                    NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyStart_, 0);
                    NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyEnd_, 1400);
                    NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
                }
            }

            NSFX_TEST_SUITE(Shared)
            {
                NSFX_TEST_SUITE(NotDirty)
                {
                    NSFX_TEST_CASE(PostTrailerBigEnough)
                    {
                        nsfx::Buffer b0(1000, 700, 300);
                        // b0 [700 s zs 300 ze e 300]
                        b0.AddAtStart(100);
                        b0.AddAtEnd(100);
                        // b0 [600 s 100 zs 300 ze 100 e 200]
                        nsfx::Buffer b1(b0);
                        b1.AddAtStart(100); // dirty at start, but not dirty at end
                        // b1 [500 s 200 zs 300 ze 100 e 200]
                        const nsfx::BufferStorage* s0 = b0.GetStorage();
                        b0.AddAtEnd(200);
                        // b0 [600 s 100 zs 300 ze 300 e]
                        // Adjust offset.
                        NSFX_TEST_EXPECT_EQ(b0.GetSize(), 700);
                        NSFX_TEST_EXPECT_EQ(b0.GetInternalSize(), b0.GetSize());
                        NSFX_TEST_EXPECT_EQ(b0.GetCapacity(), 1300);
                        NSFX_TEST_EXPECT_EQ(b0.GetStart(), 600);
                        NSFX_TEST_EXPECT_EQ(b0.GetEnd(), 1300);
                        NSFX_TEST_ASSERT(b0.GetStorage());
                        NSFX_TEST_EXPECT_EQ(b0.GetStorage(), s0);
                        NSFX_TEST_EXPECT_EQ(b0.GetStorage()->capacity_, 1300);
                        NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyStart_, 500);
                        NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyEnd_, 1300);
                        NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 2);
                    }

                    NSFX_TEST_CASE(PostTrailerNotEnough)
                    {
                        nsfx::Buffer b0(1000, 700, 300);
                        // b0 [700 s zs 300 ze e 300]
                        b0.AddAtStart(100);
                        b0.AddAtEnd(100);
                        // b0 [600 s 100 zs 300 ze 100 e 200]
                        nsfx::Buffer b1(b0);
                        b1.AddAtStart(100); // dirty at start, but not dirty at end
                        // b1 [500 s 200 zs 300 ze 100 e 200]
                        const nsfx::BufferStorage* s0 = b0.GetStorage();
                        b0.AddAtEnd(201);
                        // b0 [s 100 zs 300 ze 301 e]
                        // Reallocate just enough capacity.
                        NSFX_TEST_EXPECT_EQ(b0.GetSize(), 701);
                        NSFX_TEST_EXPECT_EQ(b0.GetInternalSize(), b0.GetSize());
                        NSFX_TEST_EXPECT_EQ(b0.GetCapacity(), 701);
                        NSFX_TEST_EXPECT_EQ(b0.GetStart(), 0);
                        NSFX_TEST_EXPECT_EQ(b0.GetEnd(), 701);
                        NSFX_TEST_ASSERT(b0.GetStorage());
                        NSFX_TEST_EXPECT_NE(b0.GetStorage(), s0);
                        NSFX_TEST_EXPECT_EQ(b0.GetStorage()->capacity_, 701);
                        NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyStart_, 0);
                        NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyEnd_, 701);
                        NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
                    }
                }

                NSFX_TEST_SUITE(Dirty)
                {
                    NSFX_TEST_CASE(PostTrailerBigEnough)
                    {
                        nsfx::Buffer b0(1000, 700, 300);
                        // b0 [700 s zs 300 ze e 300]
                        b0.AddAtStart(100);
                        b0.AddAtEnd(100);
                        // b0 [600 s 100 zs 300 ze 100 e 200]
                        nsfx::Buffer b1(b0);
                        b1.AddAtEnd(1); // dirty at end
                        // b1 [600 s 100 zs 300 ze 101 e 199]
                        const nsfx::BufferStorage* s0 = b0.GetStorage();
                        b0.AddAtEnd(200);
                        // b0 [600 s 100 zs 300 ze 300 e]
                        // Reallocate (preserve capacity).
                        NSFX_TEST_EXPECT_EQ(b0.GetSize(), 700);
                        NSFX_TEST_EXPECT_EQ(b0.GetInternalSize(), b0.GetSize());
                        NSFX_TEST_EXPECT_EQ(b0.GetCapacity(), 1300);
                        NSFX_TEST_EXPECT_EQ(b0.GetStart(), 600);
                        NSFX_TEST_EXPECT_EQ(b0.GetEnd(), 1300);
                        NSFX_TEST_ASSERT(b0.GetStorage());
                        NSFX_TEST_EXPECT_NE(b0.GetStorage(), s0);
                        NSFX_TEST_EXPECT_EQ(b0.GetStorage()->capacity_, 1300);
                        NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyStart_, 600);
                        NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyEnd_, 1300);
                        NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
                    }

                    NSFX_TEST_CASE(PostTrailerNotEnough)
                    {
                        nsfx::Buffer b0(1000, 700, 300);
                        // b0 [700 s zs 300 ze e 300]
                        b0.AddAtStart(100);
                        b0.AddAtEnd(100);
                        // b0 [600 s 100 zs 300 ze 100 e 200]
                        nsfx::Buffer b1(b0);
                        b1.AddAtEnd(1); // dirty at end
                        // b1 [600 s 100 zs 300 ze 101 e 199]
                        const nsfx::BufferStorage* s0 = b0.GetStorage();
                        b0.AddAtEnd(201);
                        // b0 [s 100 zs 300 ze 301 e]
                        // Reallocate just enough capacity.
                        NSFX_TEST_EXPECT_EQ(b0.GetSize(), 701);
                        NSFX_TEST_EXPECT_EQ(b0.GetInternalSize(), b0.GetSize());
                        NSFX_TEST_EXPECT_EQ(b0.GetCapacity(), 701);
                        NSFX_TEST_EXPECT_EQ(b0.GetStart(), 0);
                        NSFX_TEST_EXPECT_EQ(b0.GetEnd(), 701);
                        NSFX_TEST_ASSERT(b0.GetStorage());
                        NSFX_TEST_EXPECT_NE(b0.GetStorage(), s0);
                        NSFX_TEST_EXPECT_EQ(b0.GetStorage()->capacity_, 701);
                        NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyStart_, 0);
                        NSFX_TEST_EXPECT_EQ(b0.GetStorage()->dirtyEnd_, 701);
                        NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
                    }
                }
            }
        }

        NSFX_TEST_CASE(AddMemory)
        {
            uint8_t s[100];
            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t v = 0xef + i;
                s[i] = v;
            }

            nsfx::Buffer b0(1000, 700, 300);
            // b0 [700 s zs 300 ze e 300]
            b0.AddAtStart(100);
            b0.AddAtEnd(s, 100);
            // b0 [600 s 100 zs 300 ze 100 e 200]
            auto it = b0.end() - 100;
            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t v = 0xef + i;
                NSFX_TEST_EXPECT_EQ(it.Read<uint8_t>(), v);
            }
            it -= 400;
            for (size_t i = 0; i < 300; ++i)
            {
                NSFX_TEST_EXPECT_EQ(it.Read<uint8_t>(), 0);
            }
        }

        NSFX_TEST_CASE(AddBuffer)
        {
            nsfx::Buffer s(1000, 700, 0);
            s.AddAtStart(100);
            s.AddAtEnd(100);
            // s [600 s 100 zs ze 100 e 200]
            auto it = s.begin();
            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t v = 0xfe + i;
                it.Write<uint8_t>(v);
            }
            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t v = 0xef + i;
                it.Write<uint8_t>(v);
            }

            nsfx::Buffer b0(1000, 700, 300);
            b0.AddAtEnd(s);
            b0.AddAtStart(100);
            // s [200 s 100 zs 300 ze 200 e 100]
            it = b0.end() - 200;
            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t v = 0xfe + i;
                NSFX_TEST_EXPECT_EQ(it.Read<uint8_t>(), v);
            }
            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t v = 0xef + i;
                NSFX_TEST_EXPECT_EQ(it.Read<uint8_t>(), v);
            }
            it -= 500;
            for (size_t i = 0; i < 300; ++i)
            {
                NSFX_TEST_EXPECT_EQ(it.Read<uint8_t>(), 0);
            }
        }
    }/*}}}*/

    NSFX_TEST_SUITE(Fragment)/*{{{*/
    {
        NSFX_TEST_CASE(MakeFragment)
        {
            nsfx::Buffer b0(1000, 700, 300);
            // [700 s zs 300 ze e 300 ]
            const nsfx::BufferStorage* s0 = b0.GetStorage();
            b0.AddAtEnd(100);
            // [700 s zs 300 ze 100 e 200 ]
            b0.AddAtStart(100);
            // [600 s 100 zs 300 ze 100 e 200]
            auto it = b0.begin();
            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t v = 0xfe + i;
                it.Write<uint8_t>(v);
            }
            it += 300;
            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t v = 0xef + i;
                it.Write<uint8_t>(v);
            }
            // Make fragment.
            nsfx::Buffer f0 = b0.MakeFragment(0, 200);
            NSFX_TEST_EXPECT_EQ(f0.GetSize(), 200);
            it = f0.begin();
            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t v = 0xfe + i;
                NSFX_TEST_EXPECT_EQ(it.Read<uint8_t>(), v);
            }
            for (size_t i = 0; i < 100; ++i)
            {
                NSFX_TEST_EXPECT_EQ(it.Read<uint8_t>(), 0);
            }
            // Make fragment.
            nsfx::Buffer f1 = b0.MakeFragment(300, 200);
            NSFX_TEST_EXPECT_EQ(f1.GetSize(), 200);
            it = f1.begin();
            for (size_t i = 0; i < 100; ++i)
            {
                NSFX_TEST_EXPECT_EQ(it.Read<uint8_t>(), 0);
            }
            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t v = 0xef + i;
                NSFX_TEST_EXPECT_EQ(it.Read<uint8_t>(), v);
            }
            // Make fragment.
            nsfx::Buffer f2 = b0.MakeFragment(200, 0);
            NSFX_TEST_EXPECT_EQ(f2.GetSize(), 0);
        }
    }/*}}}*/

    NSFX_TEST_SUITE(RealBuffer)/*{{{*/
    {
        NSFX_TEST_CASE(MakeRealBuffer)
        {
            nsfx::Buffer b0(1000, 700, 300);
            // [700 s zs 300 ze e 300 ]
            const nsfx::BufferStorage* s0 = b0.GetStorage();
            b0.AddAtEnd(100);
            // [700 s zs 300 ze 100 e 200 ]
            b0.AddAtStart(100);
            // [600 s 100 zs 300 ze 100 e 200]
            auto it = b0.begin();
            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t v = 0xfe + i;
                it.Write<uint8_t>(v);
            }
            it += 300;
            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t v = 0xef + i;
                it.Write<uint8_t>(v);
            }
            // Make real buffer.
            nsfx::Buffer b1 = b0.MakeRealBuffer();
            NSFX_TEST_EXPECT_EQ(b1.GetSize(), 500);
            NSFX_TEST_EXPECT_EQ(b1.GetInternalSize(), b1.GetSize());
            it = b1.begin();
            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t v = 0xfe + i;
                NSFX_TEST_EXPECT_EQ(it.Read<uint8_t>(), v);
            }
            for (size_t i = 0; i < 300; ++i)
            {
                uint8_t v = 0xfe + i;
                NSFX_TEST_EXPECT_EQ(it.Read<uint8_t>(), 0);
            }
            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t v = 0xef + i;
                NSFX_TEST_EXPECT_EQ(it.Read<uint8_t>(), v);
            }
        }

        NSFX_TEST_CASE(Realize)
        {
            nsfx::Buffer b0(1000, 700, 300);
            // [700 s zs 300 ze e 300 ]
            const nsfx::BufferStorage* s0 = b0.GetStorage();
            b0.AddAtEnd(100);
            // [700 s zs 300 ze 100 e 200 ]
            b0.AddAtStart(100);
            // [600 s 100 zs 300 ze 100 e 200]
            auto it = b0.begin();
            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t v = 0xfe + i;
                it.Write<uint8_t>(v);
            }
            it += 300;
            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t v = 0xef + i;
                it.Write<uint8_t>(v);
            }
            // Realize.
            b0.Realize();
            NSFX_TEST_EXPECT_EQ(b0.GetSize(), 500);
            NSFX_TEST_EXPECT_EQ(b0.GetInternalSize(), b0.GetSize());
            it = b0.begin();
            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t v = 0xfe + i;
                NSFX_TEST_EXPECT_EQ(it.Read<uint8_t>(), v);
            }
            for (size_t i = 0; i < 300; ++i)
            {
                uint8_t v = 0xfe + i;
                NSFX_TEST_EXPECT_EQ(it.Read<uint8_t>(), 0);
            }
            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t v = 0xef + i;
                NSFX_TEST_EXPECT_EQ(it.Read<uint8_t>(), v);
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

