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
#include <nsfx/network/packet/tag-index.h>
#include <iostream>


NSFX_TEST_SUITE(TagIndexArray)
{
    NSFX_TEST_CASE(Allocate)
    {
        nsfx::TagIndexArray* tia = nsfx::TagIndexArray::Allocate(4);
        NSFX_TEST_EXPECT_EQ(tia->refCount_, 1);
        NSFX_TEST_EXPECT_EQ(tia->capacity_, 4);
        NSFX_TEST_EXPECT_EQ(tia->dirty_, 0);
        nsfx::TagIndexArray::Release(tia);
    }

    NSFX_TEST_CASE(RefCount)
    {
        nsfx::TagIndexArray* tia = nsfx::TagIndexArray::Allocate(4);
        NSFX_TEST_EXPECT_EQ(tia->refCount_, 1);
        NSFX_TEST_EXPECT_EQ(tia->capacity_, 4);
        NSFX_TEST_EXPECT_EQ(tia->dirty_, 0);

        // Addref.
        nsfx::TagIndexArray::AddRef(tia);
        NSFX_TEST_EXPECT_EQ(tia->refCount_, 2);

        // Release.
        nsfx::TagIndexArray::Release(tia);
        NSFX_TEST_EXPECT_EQ(tia->refCount_, 1);

        // Add elements.
        nsfx::TagBufferStorage* storage = nsfx::TagBufferStorage::Allocate(16);
        nsfx::TagIndex* idx = tia->indices_;
        for (size_t i = 0; i < tia->capacity_; ++i)
        {
            size_t tagId = 4;
            size_t tagStart = 5;
            size_t tagEnd = 6;
            nsfx::TagBufferStorage::AddRef(storage);
            nsfx::TagIndex::Ctor(idx, tagId, tagStart, tagEnd, storage);
            ++tia->dirty_;
            ++idx;
        }
        NSFX_TEST_EXPECT_EQ(storage->refCount_, tia->capacity_ + 1);
        // Release.
        nsfx::TagIndexArray::Release(tia);
        // All array indices are released.
        NSFX_TEST_EXPECT_EQ(storage->refCount_, 1);
        nsfx::TagBufferStorage::Release(storage);
    }
}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

