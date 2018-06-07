/**
 * @file
 *
 * @brief Test Tag.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-06-02
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/network/packet/tag.h>
#include <iostream>


NSFX_TEST_SUITE(Tag)
{
    NSFX_TEST_CASE(Ctor)
    {
        nsfx::TagBuffer b(16);
        {
            auto it = b.begin();
            for (size_t i = 0; i < 16; ++i)
            {
                uint8_t v = 0xfe + i;
                it.Write<uint8_t>(v);
            }
        }
        size_t tagId = 1;
        nsfx::Tag tag(tagId, b);
        NSFX_TEST_EXPECT_EQ(tag.GetId(), tagId);
        NSFX_TEST_EXPECT_EQ(tag.GetBuffer().GetSize(), 16);
        auto it = tag.GetBuffer().cbegin();
        for (size_t i = 0; i < 16; ++i)
        {
            uint8_t v = 0xfe + i;
            NSFX_TEST_EXPECT_EQ(it.Read<uint8_t>(), v);
        }
    }
}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

