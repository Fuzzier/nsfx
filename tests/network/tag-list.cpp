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
#include <nsfx/network/packet/tag-list.h>
#include <iostream>


int k = 0;
struct Test
{
    Test(int i, double j) : i_(i), j_(j) { ++k; }
    ~Test(void) { --k; }
    int i_;
    double j_;
};


NSFX_TEST_SUITE(TagList)
{
    NSFX_TEST_CASE(Ctor)
    {
        nsfx::TagList tl1(4, 100);
        // Has no tags.
        NSFX_TEST_EXPECT_EQ(tl1.GetSize(), 0);
        NSFX_TEST_EXPECT_EQ(tl1.GetBufferStart(), nsfx::TagList::REF_POINT - 50);
        NSFX_TEST_EXPECT_EQ(tl1.GetBufferEnd(),   nsfx::TagList::REF_POINT + 50);
        const nsfx::TagIndexArray* tia1 = tl1.GetTagIndexArray();
        NSFX_TEST_ASSERT(tia1);
        NSFX_TEST_EXPECT_EQ(tia1->refCount_, 1);
        NSFX_TEST_EXPECT_EQ(tia1->capacity_, 4);
        NSFX_TEST_EXPECT_EQ(tia1->dirty_, 0);
    }

    NSFX_TEST_SUITE(Insert)
    {
        NSFX_TEST_CASE(FromEmpty)
        {
            {
                // Create an empty list.
                nsfx::TagList tl1(4, 100);
                size_t tagId = 1;
                const nsfx::TagIndexArray* tia1 = nullptr;
                // Fill the array.
                do
                {
                    tl1.Insert<Test>(tagId++, 0, 100, 1, 2.3);
                    tia1 = tl1.GetTagIndexArray();
                    NSFX_TEST_ASSERT(tia1);
                    NSFX_TEST_EXPECT_EQ(tia1->refCount_, 1);
                    NSFX_TEST_EXPECT_EQ(tia1->dirty_, tl1.GetSize());
                    NSFX_TEST_EXPECT_EQ(k, tia1->dirty_);
                }
                while (tia1->dirty_ < tia1->capacity_);
                // Examine the array.
                NSFX_TEST_EXPECT_EQ(tia1->refCount_, 1);
                NSFX_TEST_EXPECT_EQ(tia1->dirty_, tia1->capacity_);
                NSFX_TEST_EXPECT_EQ(k, tia1->dirty_);
            }
            // Release the tag list also releases all tags.
            NSFX_TEST_EXPECT_EQ(k, 0);
        }

        NSFX_TEST_SUITE(NonShared)
        {
            NSFX_TEST_CASE(Compact)
            {
                // Create an empty list.
                nsfx::TagList tl1(6, 400);
                size_t tagId = 1;
                // Add 4 tags.
                // |<--------------buffer------------->|
                // 0        100      200      300      400
                // |--------|--------|--------|--------|
                // |<-tag1->|                 |<-tag4->|
                // |<------tag2----->|<------tag3----->|
                tl1.Insert<Test>(tagId++,   0, 100, 1, 2.3);
                tl1.Insert<Test>(tagId++,   0, 200, 1, 2.3);
                tl1.Insert<Test>(tagId++, 200, 200, 1, 2.3);
                tl1.Insert<Test>(tagId++, 300, 100, 1, 2.3);
                // Reduce the buffer.
                //          |<-----buffer---->|
                //          0        100      200
                // |--------|--------|--------|--------|
                // |<-tag1->|                 |<-tag4->|
                // |<------tag2----->|<------tag3----->|
                //
                // The bytes of tag1 and tag4 are outside of the buffer.
                // The bytes of tag2 and tag3 are partially inside of the buffer.
                tl1.RemoveAtStart(100);
                tl1.RemoveAtEnd(100);
                const nsfx::TagIndexArray* tia1 = tl1.GetTagIndexArray();
                // Let 'tl2' share and fill the array.
                // The newly added tags are not in 'tl1'.
                // Then release 'tl2'.
                {
                    nsfx::TagList tl2(tl1);
                    tl2.Insert<Test>(tagId++, 0, 100, 1, 2.3);
                    tl2.Insert<Test>(tagId++, 0, 100, 1, 2.3);
                }
                size_t num1 = tia1->dirty_;
                // Add tag7 to trigger compaction.
                tl1.Insert<Test>(tagId++, 0, 200, 1, 2.3);
                // The array is not reallocated.
                const nsfx::TagIndexArray* tia2 = tl1.GetTagIndexArray();
                NSFX_TEST_EXPECT_EQ(tia1, tia2);
                // The tags that are not in 'tl1' are removed.
                // The tags whose bytes are outside of the buffer are also removed.
                NSFX_TEST_EXPECT_EQ(tl1.GetSize(), 3);
                NSFX_TEST_EXPECT_EQ(tia1->refCount_, 1);
                NSFX_TEST_EXPECT_EQ(tia1->dirty_, 3);
                // tag2 survives.
                NSFX_TEST_EXPECT(tl1.Exists(2, 0));
                NSFX_TEST_EXPECT(tl1.Exists(2, 100-1));
                // tag3 survives.
                NSFX_TEST_EXPECT(tl1.Exists(3, 100));
                NSFX_TEST_EXPECT(tl1.Exists(3, 200-1));
                // tag5 survives.
                NSFX_TEST_EXPECT(tl1.Exists(7, 0));
                NSFX_TEST_EXPECT(tl1.Exists(7, 200-1));
            }

            NSFX_TEST_CASE(Reallocate)
            {
                // Create an empty list.
                nsfx::TagList tl1(4, 400);
                size_t tagId = 1;
                // Add 4 tags to fill the array.
                // |<--------------buffer------------->|
                // 0        100      200      300      400
                // |--------|--------|--------|--------|
                // |<-tag1->|                 |<-tag4->|
                // |<------tag2----->|<------tag3----->|
                tl1.Insert<Test>(tagId++,   0, 100, 1, 2.3);
                tl1.Insert<Test>(tagId++,   0, 200, 1, 2.3);
                tl1.Insert<Test>(tagId++, 200, 200, 1, 2.3);
                tl1.Insert<Test>(tagId++, 300, 100, 1, 2.3);
                const nsfx::TagIndexArray* tia1 = tl1.GetTagIndexArray();
                // Add tag5 to trigger reallocation.
                tl1.Insert<Test>(tagId++, 0, 400, 1, 2.3);
                // The array is reallocated.
                const nsfx::TagIndexArray* tia2 = tl1.GetTagIndexArray();
                NSFX_TEST_EXPECT_NE(tia1, tia2);
                // The tags whose bytes are outside of the buffer are removed.
                NSFX_TEST_EXPECT_EQ(tl1.GetSize(), 5);
                NSFX_TEST_EXPECT_EQ(tia2->dirty_, 5);
                // tag1 survives.
                NSFX_TEST_EXPECT(tl1.Exists(1, 0));
                NSFX_TEST_EXPECT(tl1.Exists(1, 100-1));
                // tag2 survives.
                NSFX_TEST_EXPECT(tl1.Exists(2, 0));
                NSFX_TEST_EXPECT(tl1.Exists(2, 200-1));
                // tag3 survives.
                NSFX_TEST_EXPECT(tl1.Exists(3, 200));
                NSFX_TEST_EXPECT(tl1.Exists(3, 400-1));
                // tag4 survives.
                NSFX_TEST_EXPECT(tl1.Exists(4, 300));
                NSFX_TEST_EXPECT(tl1.Exists(4, 400-1));
                // tag5 survives.
                NSFX_TEST_EXPECT(tl1.Exists(5, 0));
                NSFX_TEST_EXPECT(tl1.Exists(5, 400-1));
            }
        }

        NSFX_TEST_SUITE(Shared)
        {
            NSFX_TEST_CASE(ArrayIsFull)
            {
                // Create an empty list.
                nsfx::TagList tl1(4, 400);
                size_t tagId = 1;
                // Add 4 tags to fill the array.
                // |<--------------buffer------------->|
                // 0        100      200      300      400
                // |--------|--------|--------|--------|
                // |<-tag1->|                 |<-tag4->|
                // |<------tag2----->|<------tag3----->|
                tl1.Insert<Test>(tagId++,   0, 100, 1, 2.3);
                tl1.Insert<Test>(tagId++,   0, 200, 1, 2.3);
                tl1.Insert<Test>(tagId++, 200, 200, 1, 2.3);
                tl1.Insert<Test>(tagId++, 300, 100, 1, 2.3);
                // Reduce the buffer.
                // The bytes of tag1 and tag4 are outside of the buffer.
                // The bytes of tag2 and tag3 are partially inside of the buffer.
                //          |<-----buffer---->|
                //          0        100      200
                // |--------|--------|--------|--------|
                // |<-tag1->|                 |<-tag4->|
                // |<------tag2----->|<------tag3----->|
                tl1.RemoveAtStart(100);
                tl1.RemoveAtEnd(100);
                const nsfx::TagIndexArray* tia1 = tl1.GetTagIndexArray();
                // Share the array with 'tl2'.
                nsfx::TagList tl2(tl1);
                // Add tag5 to trigger reallocation.
                tl1.Insert<Test>(tagId++, 0, 200, 1, 2.3);
                // The array is reallocated.
                const nsfx::TagIndexArray* tia2 = tl1.GetTagIndexArray();
                NSFX_TEST_EXPECT_NE(tia1, tia2);
                // The tags whose bytes are outside of the buffer are removed.
                NSFX_TEST_EXPECT_EQ(tl1.GetSize(), 3);
                NSFX_TEST_EXPECT_EQ(tia2->dirty_, 3);
                // tag2 survives.
                NSFX_TEST_EXPECT(tl1.Exists(2, 0));
                NSFX_TEST_EXPECT(tl1.Exists(2, 100-1));
                // tag3 survives.
                NSFX_TEST_EXPECT(tl1.Exists(3, 100));
                NSFX_TEST_EXPECT(tl1.Exists(3, 200-1));
                // tag5 survives.
                NSFX_TEST_EXPECT(tl1.Exists(5, 0));
                NSFX_TEST_EXPECT(tl1.Exists(5, 200-1));
            }

            NSFX_TEST_SUITE(ArrayIsNotFull)
            {
                NSFX_TEST_CASE(NotReallocate)
                {
                    // Create an empty list.
                    nsfx::TagList tl1(5, 400);
                    size_t tagId = 1;
                    // Add 4 tags.
                    // |<--------------buffer------------->|
                    // 0        100      200      300      400
                    // |--------|--------|--------|--------|
                    // |<-tag1->|                 |<-tag4->|
                    // |<------tag2----->|<------tag3----->|
                    tl1.Insert<Test>(tagId++,   0, 100, 1, 2.3);
                    tl1.Insert<Test>(tagId++,   0, 200, 1, 2.3);
                    tl1.Insert<Test>(tagId++, 200, 200, 1, 2.3);
                    tl1.Insert<Test>(tagId++, 300, 100, 1, 2.3);
                    // Share the array with 'tl2'.
                    nsfx::TagList tl2(tl1);
                    // Reduce the buffer.
                    // The bytes of tag1 and tag4 are outside of the buffer.
                    // The bytes of tag2 and tag3 are partially inside of the buffer.
                    //          |<-----buffer---->|
                    //          0        100      200
                    // |--------|--------|--------|--------|
                    // |<-tag1->|                 |<-tag4->|
                    // |<------tag2----->|<------tag3----->|
                    tl1.RemoveAtStart(100);
                    tl1.RemoveAtEnd(100);
                    const nsfx::TagIndexArray* tia1 = tl1.GetTagIndexArray();
                    // Add tag5 to 'tl1'.
                    tl1.Insert<Test>(tagId++, 0, 200, 1, 2.3);
                    // The array is not reallocated.
                    const nsfx::TagIndexArray* tia2 = tl1.GetTagIndexArray();
                    NSFX_TEST_EXPECT_EQ(tia1, tia2);
                    // The tags whose bytes are outside of the buffer are not
                    // removed, since the array is shared and cannot be
                    // compacted.
                    NSFX_TEST_EXPECT_EQ(tl1.GetSize(), 5);
                    NSFX_TEST_EXPECT_EQ(tia2->dirty_, 5);
                    // tag1 survives in 'tl2'.
                    NSFX_TEST_EXPECT(tl2.Exists(1, 0));
                    NSFX_TEST_EXPECT(tl2.Exists(1, 100-1));
                    // tag2 survives.
                    NSFX_TEST_EXPECT(tl1.Exists(2, 0));
                    NSFX_TEST_EXPECT(tl1.Exists(2, 100-1));
                    // tag3 survives.
                    NSFX_TEST_EXPECT(tl1.Exists(3, 100));
                    NSFX_TEST_EXPECT(tl1.Exists(3, 200-1));
                    // tag4 survives in 'tl2'.
                    NSFX_TEST_EXPECT(tl2.Exists(4, 300));
                    NSFX_TEST_EXPECT(tl2.Exists(4, 400-1));
                    // tag5 survives.
                    NSFX_TEST_EXPECT(tl1.Exists(5, 0));
                    NSFX_TEST_EXPECT(tl1.Exists(5, 200-1));
                }

                NSFX_TEST_CASE(Reallocate)
                {
                    // Create an empty list.
                    nsfx::TagList tl1(6, 400);
                    size_t tagId = 1;
                    // Add 4 tags.
                    // |<--------------buffer------------->|
                    // 0        100      200      300      400
                    // |--------|--------|--------|--------|
                    // |<-tag1->|                 |<-tag4->|
                    // |<------tag2----->|<------tag3----->|
                    tl1.Insert<Test>(tagId++,   0, 100, 1, 2.3);
                    tl1.Insert<Test>(tagId++,   0, 200, 1, 2.3);
                    tl1.Insert<Test>(tagId++, 200, 200, 1, 2.3);
                    tl1.Insert<Test>(tagId++, 300, 100, 1, 2.3);
                    // Reduce the buffer.
                    // The bytes of tag1 and tag4 are outside of the buffer.
                    // The bytes of tag2 and tag3 are partially inside of the buffer.
                    //          |<-----buffer---->|
                    //          0        100      200
                    // |--------|--------|--------|--------|
                    // |<-tag1->|                 |<-tag4->|
                    // |<------tag2----->|<------tag3----->|
                    tl1.RemoveAtStart(100);
                    tl1.RemoveAtEnd(100);
                    const nsfx::TagIndexArray* tia1 = tl1.GetTagIndexArray();
                    // Share the array with 'tl2'.
                    nsfx::TagList tl2(tl1);
                    // Add tag5 to 'tl2', which is not in 'tl1'.
                    tl2.Insert<Test>(tagId++, 0, 200, 1, 2.3);
                    // Add tag6 to 'tl1' to trigger reallocation.
                    tl1.Insert<Test>(tagId++, 0, 200, 1, 2.3);
                    // The array is reallocated, even if the array has free elements.
                    const nsfx::TagIndexArray* tia2 = tl1.GetTagIndexArray();
                    NSFX_TEST_EXPECT_NE(tia1, tia2);
                    // The tags whose bytes are outside of the buffer are removed.
                    NSFX_TEST_EXPECT_EQ(tl1.GetSize(), 3);
                    NSFX_TEST_EXPECT_EQ(tia2->dirty_, 3);
                    // tag2 survives.
                    NSFX_TEST_EXPECT(tl1.Exists(2, 0));
                    NSFX_TEST_EXPECT(tl1.Exists(2, 100-1));
                    // tag3 survives.
                    NSFX_TEST_EXPECT(tl1.Exists(3, 100));
                    NSFX_TEST_EXPECT(tl1.Exists(3, 200-1));
                    // tag6 survives.
                    NSFX_TEST_EXPECT(tl1.Exists(6, 0));
                    NSFX_TEST_EXPECT(tl1.Exists(6, 200-1));
                }
            }
        }
    }
}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

