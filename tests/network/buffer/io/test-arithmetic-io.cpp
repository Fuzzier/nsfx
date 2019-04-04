/**
 * @file
 *
 * @brief Test Arithmetic IO.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2019-04-04
 *
 * @copyright Copyright (c) 2019.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/network/buffer.h>
#include <nsfx/network/buffer/io/arithmetic-io.h>
#include <iostream>


NSFX_TEST_SUITE(ArithmeticIo)
{
    using namespace nsfx;

    NSFX_TEST_CASE(Native)
    {
        {
            Buffer buffer;
            int8_t v0 = (int8_t)0xfe;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            Write(itw, v0);

            int8_t v1;
            auto itr = buffer.cbegin();
            Read(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
        {
            Buffer buffer;
            uint8_t v0 = (uint8_t)0xfe;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            Write(itw, v0);

            uint8_t v1;
            auto itr = buffer.cbegin();
            Read(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
        {
            Buffer buffer;
            int16_t v0 = (int16_t)0xfedc;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            Write(itw, v0);

            int16_t v1;
            auto itr = buffer.cbegin();
            Read(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
        {
            Buffer buffer;
            uint16_t v0 = (uint16_t)0xfedc;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            Write(itw, v0);

            uint16_t v1;
            auto itr = buffer.cbegin();
            Read(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
        {
            Buffer buffer;
            int32_t v0 = (int32_t)0xfedcba98;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            Write(itw, v0);

            int32_t v1;
            auto itr = buffer.cbegin();
            Read(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
        {
            Buffer buffer;
            uint32_t v0 = (uint32_t)0xfedcba98;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            Write(itw, v0);

            uint32_t v1;
            auto itr = buffer.cbegin();
            Read(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
        {
            Buffer buffer;
            int64_t v0 = (int64_t)0xfedcba987654321f;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            Write(itw, v0);

            int64_t v1;
            auto itr = buffer.cbegin();
            Read(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
        {
            Buffer buffer;
            uint64_t v0 = (uint64_t)0xfedcba987654321f;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            Write(itw, v0);

            uint64_t v1;
            auto itr = buffer.cbegin();
            Read(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
        {
            Buffer buffer;
            float v0 = (float)1.234;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            Write(itw, v0);

            float v1;
            auto itr = buffer.cbegin();
            Read(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
        {
            Buffer buffer;
            float v0 = (float)1.234;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            Write(itw, v0);

            float v1;
            auto itr = buffer.cbegin();
            Read(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
        {
            Buffer buffer;
            double v0 = (double)1.234;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            Write(itw, v0);

            double v1;
            auto itr = buffer.cbegin();
            Read(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
        {
            Buffer buffer;
            double v0 = (double)1.234;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            Write(itw, v0);

            double v1;
            auto itr = buffer.cbegin();
            Read(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
    }

    NSFX_TEST_CASE(Big)
    {
        {
            Buffer buffer;
            int8_t v0 = (int8_t)0xfe;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            WriteB(itw, v0);

            int8_t v1;
            auto itr = buffer.cbegin();
            ReadB(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
        {
            Buffer buffer;
            uint8_t v0 = (uint8_t)0xfe;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            WriteB(itw, v0);

            uint8_t v1;
            auto itr = buffer.cbegin();
            ReadB(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
        {
            Buffer buffer;
            int16_t v0 = (int16_t)0xfedc;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            WriteB(itw, v0);

            int16_t v1;
            auto itr = buffer.cbegin();
            ReadB(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
        {
            Buffer buffer;
            uint16_t v0 = (uint16_t)0xfedc;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            WriteB(itw, v0);

            uint16_t v1;
            auto itr = buffer.cbegin();
            ReadB(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
        {
            Buffer buffer;
            int32_t v0 = (int32_t)0xfedcba98;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            WriteB(itw, v0);

            int32_t v1;
            auto itr = buffer.cbegin();
            ReadB(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
        {
            Buffer buffer;
            uint32_t v0 = (uint32_t)0xfedcba98;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            WriteB(itw, v0);

            uint32_t v1;
            auto itr = buffer.cbegin();
            ReadB(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
        {
            Buffer buffer;
            int64_t v0 = (int64_t)0xfedcba987654321f;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            WriteB(itw, v0);

            int64_t v1;
            auto itr = buffer.cbegin();
            ReadB(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
        {
            Buffer buffer;
            uint64_t v0 = (uint64_t)0xfedcba987654321f;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            WriteB(itw, v0);

            uint64_t v1;
            auto itr = buffer.cbegin();
            ReadB(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
        {
            Buffer buffer;
            float v0 = (float)1.234;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            WriteB(itw, v0);

            float v1;
            auto itr = buffer.cbegin();
            ReadB(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
        {
            Buffer buffer;
            float v0 = (float)1.234;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            WriteB(itw, v0);

            float v1;
            auto itr = buffer.cbegin();
            ReadB(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
        {
            Buffer buffer;
            double v0 = (double)1.234;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            WriteB(itw, v0);

            double v1;
            auto itr = buffer.cbegin();
            ReadB(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
        {
            Buffer buffer;
            double v0 = (double)1.234;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            WriteB(itw, v0);

            double v1;
            auto itr = buffer.cbegin();
            ReadB(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
    }

    NSFX_TEST_CASE(Little)
    {
        {
            Buffer buffer;
            int8_t v0 = (int8_t)0xfe;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            WriteL(itw, v0);

            int8_t v1;
            auto itr = buffer.cbegin();
            ReadL(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
        {
            Buffer buffer;
            uint8_t v0 = (uint8_t)0xfe;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            WriteL(itw, v0);

            uint8_t v1;
            auto itr = buffer.cbegin();
            ReadL(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
        {
            Buffer buffer;
            int16_t v0 = (int16_t)0xfedc;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            WriteL(itw, v0);

            int16_t v1;
            auto itr = buffer.cbegin();
            ReadL(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
        {
            Buffer buffer;
            uint16_t v0 = (uint16_t)0xfedc;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            WriteL(itw, v0);

            uint16_t v1;
            auto itr = buffer.cbegin();
            ReadL(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
        {
            Buffer buffer;
            int32_t v0 = (int32_t)0xfedcba98;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            WriteL(itw, v0);

            int32_t v1;
            auto itr = buffer.cbegin();
            ReadL(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
        {
            Buffer buffer;
            uint32_t v0 = (uint32_t)0xfedcba98;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            WriteL(itw, v0);

            uint32_t v1;
            auto itr = buffer.cbegin();
            ReadL(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
        {
            Buffer buffer;
            int64_t v0 = (int64_t)0xfedcba987654321f;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            WriteL(itw, v0);

            int64_t v1;
            auto itr = buffer.cbegin();
            ReadL(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
        {
            Buffer buffer;
            uint64_t v0 = (uint64_t)0xfedcba987654321f;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            WriteL(itw, v0);

            uint64_t v1;
            auto itr = buffer.cbegin();
            ReadL(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
        {
            Buffer buffer;
            float v0 = (float)1.234;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            WriteL(itw, v0);

            float v1;
            auto itr = buffer.cbegin();
            ReadL(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
        {
            Buffer buffer;
            float v0 = (float)1.234;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            WriteL(itw, v0);

            float v1;
            auto itr = buffer.cbegin();
            ReadL(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
        {
            Buffer buffer;
            double v0 = (double)1.234;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            WriteL(itw, v0);

            double v1;
            auto itr = buffer.cbegin();
            ReadL(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
        {
            Buffer buffer;
            double v0 = (double)1.234;
            buffer.AddAtStart(sizeof (v0));
            auto itw = buffer.begin();
            WriteL(itw, v0);

            double v1;
            auto itr = buffer.cbegin();
            ReadL(itr, &v1);

            NSFX_TEST_EXPECT_EQ(v0, v1);
        }
    }

}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cerr);
    nsfx::test::runner::Run();

    return 0;
}

