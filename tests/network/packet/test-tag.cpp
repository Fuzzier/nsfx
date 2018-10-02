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
#include <nsfx/network/buffer.h>
#include <nsfx/network/packet/tag/basic-tag.h>
#include <iostream>


NSFX_TEST_SUITE(Tag)
{
    typedef nsfx::FixedBuffer      TagBuffer;
    typedef nsfx::ConstFixedBuffer ConstTagBuffer;

    typedef nsfx::BasicTag<ConstTagBuffer> Tag;

    NSFX_TEST_CASE(Ctor)
    {
        TagBuffer b(16);
        {
            auto it = b.begin();
            for (size_t i = 0; i < 16; ++i)
            {
                uint8_t v = (uint8_t)(0xfe + i);
                it.Write<uint8_t>(v);
            }
        }
        uint32_t tagId = 1;
        Tag tag(tagId, b);
        NSFX_TEST_EXPECT_EQ(tag.GetId(), tagId);
        NSFX_TEST_EXPECT_EQ(tag.GetValue().GetSize(), 16);
        auto it = tag.GetValue().cbegin();
        for (size_t i = 0; i < 16; ++i)
        {
            uint8_t v = (uint8_t)(0xfe + i);
            NSFX_TEST_EXPECT_EQ(it.Read<uint8_t>(), v);
        }
    }
}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cerr);
    nsfx::test::runner::Run();

    return 0;
}

