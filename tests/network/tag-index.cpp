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
#include <nsfx/network/packet/tag-index.h>
#include <iostream>


int k = 0;
struct Test
{
    Test(int i, double j) : i_(i), j_(j) { ++k; }
    ~Test(void) { --k; }
    int i_;
    double j_;
};


NSFX_TEST_SUITE(TagIndex)
{
    NSFX_TEST_CASE(Ctor)
    {
        nsfx::TagStorage* storage = nsfx::TagStorage::Allocate<Test>(1, 2.3);
        nsfx::TagIndex idx;
        size_t tagId = 4;
        size_t tagStart = 5;
        size_t tagEnd = 6;
        nsfx::TagIndex::Ctor(&idx, tagId, tagStart, tagEnd, storage);
        NSFX_TEST_EXPECT_EQ(idx.tagId_, tagId);
        NSFX_TEST_EXPECT_EQ(idx.tagStart_, tagStart);
        NSFX_TEST_EXPECT_EQ(idx.tagEnd_, tagEnd);
        NSFX_TEST_EXPECT_EQ(idx.storage_, storage);

        NSFX_TEST_EXPECT_EQ(storage->refCount_, 1);

        nsfx::TagIndex::Release(&idx);
        // Test::~Test() is called.
        NSFX_TEST_EXPECT_EQ(k, 0);
    }

    NSFX_TEST_CASE(CopyCtor)
    {
        nsfx::TagStorage* storage = nsfx::TagStorage::Allocate<Test>(1, 2.3);
        nsfx::TagIndex idx1;
        size_t tagId = 4;
        size_t tagStart = 5;
        size_t tagEnd = 6;
        nsfx::TagIndex::Ctor(&idx1, tagId, tagStart, tagEnd, storage);

        nsfx::TagIndex idx2;
        nsfx::TagIndex::CopyCtor(&idx2, &idx1);

        NSFX_TEST_EXPECT_EQ(idx1.tagId_, tagId);
        NSFX_TEST_EXPECT_EQ(idx1.tagStart_, tagStart);
        NSFX_TEST_EXPECT_EQ(idx1.tagEnd_, tagEnd);
        NSFX_TEST_EXPECT_EQ(idx1.storage_, storage);

        NSFX_TEST_EXPECT_EQ(idx2.tagId_, tagId);
        NSFX_TEST_EXPECT_EQ(idx2.tagStart_, tagStart);
        NSFX_TEST_EXPECT_EQ(idx2.tagEnd_, tagEnd);
        NSFX_TEST_EXPECT_EQ(idx2.storage_, storage);

        NSFX_TEST_EXPECT_EQ(storage->refCount_, 2);

        nsfx::TagIndex::Release(&idx1);
        // Test::~Test() is not called.
        NSFX_TEST_EXPECT_EQ(k, 1);
        NSFX_TEST_EXPECT_EQ(storage->refCount_, 1);

        nsfx::TagIndex::Release(&idx2);
        // Test::~Test() is called.
        NSFX_TEST_EXPECT_EQ(k, 0);
    }

    NSFX_TEST_CASE(CopyAssign)
    {
        nsfx::TagStorage* storage1 = nsfx::TagStorage::Allocate<Test>(1, 2.3);
        nsfx::TagStorage* storage2 = nsfx::TagStorage::Allocate<Test>(4, 5.6);

        nsfx::TagIndex idx1;
        size_t tagId1 = 7;
        size_t tagStart1 = 8;
        size_t tagEnd1 = 9;
        nsfx::TagIndex::Ctor(&idx1, tagId1, tagStart1, tagEnd1, storage1);

        nsfx::TagIndex idx2;
        size_t tagId2 = 10;
        size_t tagStart2 = 11;
        size_t tagEnd2 = 12;
        nsfx::TagIndex::Ctor(&idx2, tagId2, tagStart2, tagEnd2, storage2);

        nsfx::TagStorage::AddRef(storage2);
        nsfx::TagIndex::CopyAssign(&idx2, &idx1);
        // The storage2 is released by idx2.
        NSFX_TEST_EXPECT_EQ(storage2->refCount_, 1);
        // The storage1 is addref by idx2.
        NSFX_TEST_EXPECT_EQ(storage1->refCount_, 2);
        // Release storage2, since it will not be used.
        nsfx::TagStorage::Release(storage2);

        NSFX_TEST_EXPECT_EQ(idx1.tagId_, tagId1);
        NSFX_TEST_EXPECT_EQ(idx1.tagStart_, tagStart1);
        NSFX_TEST_EXPECT_EQ(idx1.tagEnd_, tagEnd1);
        NSFX_TEST_EXPECT_EQ(idx1.storage_, storage1);

        NSFX_TEST_EXPECT_EQ(idx2.tagId_, tagId1);
        NSFX_TEST_EXPECT_EQ(idx2.tagStart_, tagStart1);
        NSFX_TEST_EXPECT_EQ(idx2.tagEnd_, tagEnd1);
        NSFX_TEST_EXPECT_EQ(idx2.storage_, storage1);

        nsfx::TagIndex::Release(&idx1);
        nsfx::TagIndex::Release(&idx2);
    }

    NSFX_TEST_CASE(Swap)
    {
        nsfx::TagStorage* storage1 = nsfx::TagStorage::Allocate<Test>(1, 2.3);
        nsfx::TagStorage* storage2 = nsfx::TagStorage::Allocate<Test>(4, 5.6);

        nsfx::TagIndex idx1;
        size_t tagId1 = 4;
        size_t tagStart1 = 5;
        size_t tagEnd1 = 6;
        nsfx::TagIndex::Ctor(&idx1, tagId1, tagStart1, tagEnd1, storage1);

        nsfx::TagIndex idx2;
        size_t tagId2 = 7;
        size_t tagStart2 = 8;
        size_t tagEnd2 = 9;
        nsfx::TagIndex::Ctor(&idx2, tagId2, tagStart2, tagEnd2, storage2);

        nsfx::TagIndex::Swap(&idx1, &idx2);

        NSFX_TEST_EXPECT_EQ(idx2.tagId_, tagId1);
        NSFX_TEST_EXPECT_EQ(idx2.tagStart_, tagStart1);
        NSFX_TEST_EXPECT_EQ(idx2.tagEnd_, tagEnd1);
        NSFX_TEST_EXPECT_EQ(idx2.storage_, storage1);

        NSFX_TEST_EXPECT_EQ(idx1.tagId_, tagId2);
        NSFX_TEST_EXPECT_EQ(idx1.tagStart_, tagStart2);
        NSFX_TEST_EXPECT_EQ(idx1.tagEnd_, tagEnd2);
        NSFX_TEST_EXPECT_EQ(idx1.storage_, storage2);

        nsfx::TagIndex::Release(&idx1);
        nsfx::TagIndex::Release(&idx2);
    }

    NSFX_TEST_CASE(HasTaggedByte)
    {
        nsfx::TagStorage* storage = nsfx::TagStorage::Allocate<Test>(1, 2.3);
        nsfx::TagIndex idx;
        size_t tagId = 4;
        size_t tagStart = 5;
        size_t tagEnd = 6;
        nsfx::TagIndex::Ctor(&idx, tagId, tagStart, tagEnd, storage);

        NSFX_TEST_EXPECT(!nsfx::TagIndex::HasTaggedByte(&idx, 0, tagStart));
        NSFX_TEST_EXPECT(nsfx::TagIndex::HasTaggedByte(&idx, tagStart, tagEnd));
        NSFX_TEST_EXPECT(!nsfx::TagIndex::HasTaggedByte(&idx, tagEnd, static_cast<size_t>(-1)));

        nsfx::TagIndex::Release(&idx);
    }
}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

