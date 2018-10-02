/**
 * @file
 *
 * @brief Test FixedBuffer.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-06-06
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/network/buffer.h>
#include <iostream>


NSFX_TEST_SUITE(FixedBuffer)
{
    NSFX_TEST_SUITE(Ctor)/*{{{*/
    {
        NSFX_TEST_CASE(Ctor0)
        {
            nsfx::FixedBuffer b0;
            NSFX_TEST_EXPECT_EQ(b0.GetSize(), 0);
            NSFX_TEST_EXPECT(!b0.GetStorage());
        }

        NSFX_TEST_CASE(Ctor1)
        {
            nsfx::FixedBuffer b0(300);
            NSFX_TEST_EXPECT_EQ(b0.GetSize(), 300);
            NSFX_TEST_ASSERT(b0.GetStorage());
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->capacity_, 300);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
        }

        NSFX_TEST_CASE(ZeroInit)
        {
            nsfx::FixedBuffer b0(300, true);
            NSFX_TEST_EXPECT_EQ(b0.GetSize(), 300);
            NSFX_TEST_ASSERT(b0.GetStorage());
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->capacity_, 300);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 1);
            for (auto it = b0.cbegin(); it != b0.cend(); ++it)
            {
                NSFX_TEST_EXPECT_EQ(it.Read<uint8_t>(), 0);
            }
        }

        NSFX_TEST_CASE(FromConstBuffer)
        {
            nsfx::Buffer b0(300);
            b0.AddAtStart(300);
            auto it0 = b0.begin();
            for (size_t i = 0; i < 300; ++i)
            {
                uint8_t v = (uint8_t)(0xfe + i);
                it0.Write<uint8_t>(v);
            }
            it0 = b0.begin();

            nsfx::FixedBuffer b1(b0);
            auto it1 = b1.cbegin();
            for (size_t i = 0; i < 300; ++i)
            {
                uint8_t v = (uint8_t)(0xfe + i);
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
                uint8_t v = (uint8_t)(0xfe + i);
                it0.Write<uint8_t>(v);
            }
            it0 = b0.begin();

            nsfx::FixedBuffer b1(b0);
            auto it1 = b1.cbegin();
            for (size_t i = 0; i < 300; ++i)
            {
                uint8_t v = (uint8_t)(0xfe + i);
                NSFX_TEST_EXPECT_EQ(it1.Read<uint8_t>(), v);
            }
        }

        NSFX_TEST_CASE(FromConstFixedBuffer)
        {
            nsfx::FixedBuffer b0(300);
            auto it0 = b0.begin();
            for (size_t i = 0; i < 300; ++i)
            {
                uint8_t v = (uint8_t)(0xfe + i);
                it0.Write<uint8_t>(v);
            }
            it0 = b0.begin();

            nsfx::ConstFixedBuffer cb0(b0);
            nsfx::FixedBuffer b1(cb0);
            auto it1 = b1.cbegin();
            for (size_t i = 0; i < 300; ++i)
            {
                uint8_t v = (uint8_t)(0xfe + i);
                NSFX_TEST_EXPECT_EQ(it1.Read<uint8_t>(), v);
            }
        }

    }/*}}}*/

    NSFX_TEST_SUITE(Copy)/*{{{*/
    {
        NSFX_TEST_CASE(CopyCtor)
        {
            nsfx::FixedBuffer b0(300);
            nsfx::FixedBuffer b1(b0);
            //
            NSFX_TEST_EXPECT_EQ(b0.GetSize(), 300);
            NSFX_TEST_ASSERT(b0.GetStorage());
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->capacity_, 300);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 2);
            //
            NSFX_TEST_EXPECT_EQ(b1.GetSize(), 300);
            NSFX_TEST_ASSERT(b1.GetStorage());
            NSFX_TEST_EXPECT_EQ(b0.GetStorage(), b1.GetStorage());
            NSFX_TEST_EXPECT_EQ(b1.GetStorage()->capacity_, 300);
            NSFX_TEST_EXPECT_EQ(b1.GetStorage()->refCount_, 2);
        }

        NSFX_TEST_CASE(CopyAssign)
        {
            nsfx::FixedBuffer b0(300);
            nsfx::FixedBuffer b1(600);
            nsfx::FixedBuffer b2(b1); // Before change b1, use make a backup in b2.
            b1 = b0;
            //
            NSFX_TEST_EXPECT_EQ(b0.GetSize(), 300);
            NSFX_TEST_ASSERT(b0.GetStorage());
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->capacity_, 300);
            NSFX_TEST_EXPECT_EQ(b0.GetStorage()->refCount_, 2);
            //
            NSFX_TEST_EXPECT_EQ(b1.GetSize(), 300);
            NSFX_TEST_ASSERT(b1.GetStorage());
            NSFX_TEST_EXPECT_EQ(b0.GetStorage(), b1.GetStorage());
            NSFX_TEST_EXPECT_EQ(b1.GetStorage()->capacity_, 300);
            NSFX_TEST_EXPECT_EQ(b1.GetStorage()->refCount_, 2);
            //
            NSFX_TEST_EXPECT_EQ(b2.GetSize(), 600);
            NSFX_TEST_ASSERT(b2.GetStorage());
            NSFX_TEST_EXPECT_EQ(b2.GetStorage()->capacity_, 600);
            NSFX_TEST_EXPECT_EQ(b2.GetStorage()->refCount_, 1);
        }
    }/*}}}*/

    NSFX_TEST_SUITE(Move)/*{{{*/
    {
        NSFX_TEST_CASE(MoveCtor)
        {
            nsfx::FixedBuffer b0(300);
            nsfx::FixedBuffer b1(std::move(b0));
            //
            NSFX_TEST_EXPECT_EQ(b0.GetSize(), 0);
            NSFX_TEST_ASSERT(!b0.GetStorage());
            //
            NSFX_TEST_EXPECT_EQ(b1.GetSize(), 300);
            NSFX_TEST_ASSERT(b1.GetStorage());
            NSFX_TEST_EXPECT_EQ(b1.GetStorage()->capacity_, 300);
            NSFX_TEST_EXPECT_EQ(b1.GetStorage()->refCount_, 1);
        }

        NSFX_TEST_CASE(MoveAssign)
        {
            nsfx::FixedBuffer b0(300);
            nsfx::FixedBuffer b1(600);
            nsfx::FixedBuffer b2(b1); // Before change b1, use make a backup in b2.
            b1 = std::move(b0);
            //
            NSFX_TEST_EXPECT_EQ(b0.GetSize(), 0);
            NSFX_TEST_ASSERT(!b0.GetStorage());
            //
            NSFX_TEST_EXPECT_EQ(b1.GetSize(), 300);
            NSFX_TEST_ASSERT(b1.GetStorage());
            NSFX_TEST_EXPECT_EQ(b1.GetStorage()->capacity_, 300);
            NSFX_TEST_EXPECT_EQ(b1.GetStorage()->refCount_, 1);
            //
            NSFX_TEST_EXPECT_EQ(b2.GetSize(), 600);
            NSFX_TEST_ASSERT(b2.GetStorage());
            NSFX_TEST_EXPECT_EQ(b2.GetStorage()->capacity_, 600);
            NSFX_TEST_EXPECT_EQ(b2.GetStorage()->refCount_, 1);
        }
    }/*}}}*/

    NSFX_TEST_SUITE(CopyTo)/*{{{*/
    {
        NSFX_TEST_CASE(Segmented)
        {
            nsfx::FixedBuffer b0(200);

            nsfx::BufferIterator it = b0.begin();
            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t v = (uint8_t)(0xfe + i);
                it.Write<uint8_t>(v);
            }
            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t v = (uint8_t)(0xef + i);
                it.Write<uint8_t>(v);
            }

            size_t size = b0.GetSize();
            uint8_t* byte = new uint8_t[size];
            b0.CopyTo(byte, size);

            for (size_t i = 0; i < 100; ++i)
            {
                uint8_t v = (uint8_t)(0xfe + i);
                NSFX_TEST_EXPECT_EQ(byte[i], v);
            }
            for (size_t i = 100; i < 200; ++i)
            {
                uint8_t v = (uint8_t)(0xef + i - 100);
                NSFX_TEST_EXPECT_EQ(byte[i], v);
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

