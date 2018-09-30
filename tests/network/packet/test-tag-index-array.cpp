/**
 * @file
 *
 * @brief Test TagIndexArray.
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
#include <nsfx/network/packet/tag/basic-tag-index-array.h>
#include <iostream>


NSFX_TEST_SUITE(TagIndexArray)
{
    typedef nsfx::FixedBuffer      TagBuffer;
    typedef nsfx::ConstFixedBuffer ConstTagBuffer;

    typedef nsfx::BasicTag<ConstTagBuffer>           Tag;
    typedef nsfx::BasicTagIndex<ConstTagBuffer>      TagIndex;
    typedef nsfx::BasicTagIndexArray<ConstTagBuffer> TagIndexArray;

    NSFX_TEST_CASE(Allocate)
    {
        TagIndexArray* tia = TagIndexArray::Allocate(4);
        NSFX_TEST_EXPECT_EQ(tia->refCount_, 1);
        NSFX_TEST_EXPECT_EQ(tia->capacity_, 4);
        NSFX_TEST_EXPECT_EQ(tia->dirty_, 0);
        TagIndexArray::Release(tia);
    }

    NSFX_TEST_CASE(RefCount)
    {
        TagIndexArray* tia = TagIndexArray::Allocate(4);
        NSFX_TEST_EXPECT_EQ(tia->refCount_, 1);
        NSFX_TEST_EXPECT_EQ(tia->capacity_, 4);
        NSFX_TEST_EXPECT_EQ(tia->dirty_, 0);

        // Addref.
        TagIndexArray::AddRef(tia);
        NSFX_TEST_EXPECT_EQ(tia->refCount_, 2);

        // Release.
        TagIndexArray::Release(tia);
        NSFX_TEST_EXPECT_EQ(tia->refCount_, 1);

        // Add elements.
        TagBuffer b(16);
        TagIndex* idx = tia->indices_;
        for (size_t i = 0; i < tia->capacity_; ++i)
        {
            size_t tagId = 4;
            size_t tagStart = 5;
            size_t tagEnd = 6;
            Tag tag(tagId, b);
            new (idx) TagIndex(tag, tagStart, tagEnd);
            ++tia->dirty_;
            ++idx;
        }
        NSFX_TEST_EXPECT_EQ(b.GetStorage()->refCount_, tia->capacity_ + 1);
        // Release.
        TagIndexArray::Release(tia);
        // All array indices are released.
        NSFX_TEST_EXPECT_EQ(b.GetStorage()->refCount_, 1);
    }
}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

