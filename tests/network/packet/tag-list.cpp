/**
 * @file
 *
 * @brief Test TagList.
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
#include <nsfx/network/packet/tag/basic-tag-list.h>
#include <iostream>


NSFX_TEST_SUITE(TagList)
{
    typedef nsfx::FixedBuffer      TagBuffer;
    typedef nsfx::ConstFixedBuffer ConstTagBuffer;

    typedef nsfx::BasicTag<ConstTagBuffer>           Tag;
    typedef nsfx::BasicTagIndex<ConstTagBuffer>      TagIndex;
    typedef nsfx::BasicTagIndexArray<ConstTagBuffer> TagIndexArray;
    typedef nsfx::BasicTagList<ConstTagBuffer>       TagList;

    NSFX_TEST_CASE(Ctor)/*{{{*/
    {
        TagList tl1(4, 100);
        // Has no tags.
        NSFX_TEST_EXPECT_EQ(tl1.GetSize(), 0);
        NSFX_TEST_EXPECT_EQ(tl1.GetInternalSize(), 0);
        NSFX_TEST_EXPECT_EQ(tl1.GetBufferStart(), TagList::REF_POINT - 50);
        NSFX_TEST_EXPECT_EQ(tl1.GetBufferEnd(),   TagList::REF_POINT + 50);
        const TagIndexArray* tia1 = tl1.GetTagIndexArray();
        NSFX_TEST_ASSERT(tia1);
        NSFX_TEST_EXPECT_EQ(tia1->refCount_, 1);
        NSFX_TEST_EXPECT_EQ(tia1->capacity_, 4);
        NSFX_TEST_EXPECT_EQ(tia1->dirty_, 0);
    }/*}}}*/

    NSFX_TEST_SUITE(Insert)/*{{{*/
    {
        NSFX_TEST_CASE(FromEmpty)
        {
            TagBuffer b(16);
            {
                // Create an empty list.
                TagList tl1(4, 100);
                size_t tagId = 1;
                const TagIndexArray* tia1 = nullptr;
                // Fill the array.
                do
                {
                    tl1.Insert(tagId++, b, 0, 100);
                    tia1 = tl1.GetTagIndexArray();
                    NSFX_TEST_ASSERT(tia1);
                    NSFX_TEST_EXPECT_EQ(tia1->refCount_, 1);
                    NSFX_TEST_EXPECT_EQ(tia1->dirty_, tl1.GetInternalSize());
                    NSFX_TEST_EXPECT_EQ(b.GetStorage()->refCount_, tia1->dirty_ + 1);
                }
                while (tia1->dirty_ < tia1->capacity_);
                // Examine the array.
                NSFX_TEST_EXPECT_EQ(tia1->refCount_, 1);
                NSFX_TEST_EXPECT_EQ(tia1->dirty_, tia1->capacity_);
                NSFX_TEST_EXPECT_EQ(b.GetStorage()->refCount_, tia1->dirty_ + 1);
            }
            // Release the tag list also releases all tags.
            NSFX_TEST_EXPECT_EQ(b.GetStorage()->refCount_, 1);
        }

        NSFX_TEST_CASE(FromFreeTag)
        {
            TagBuffer b(16);
            {
                // Create an empty list.
                TagList tl1(4, 100);
                size_t tagId = 1;
                const TagIndexArray* tia1 = nullptr;
                // Fill the array.
                do
                {
                    // Create a free tag.
                    tl1.Insert(tagId++, b, 0, 100);
                    tia1 = tl1.GetTagIndexArray();
                    NSFX_TEST_ASSERT(tia1);
                    NSFX_TEST_EXPECT_EQ(tia1->refCount_, 1);
                    NSFX_TEST_EXPECT_EQ(tia1->dirty_, tl1.GetInternalSize());
                    NSFX_TEST_EXPECT_EQ(b.GetStorage()->refCount_, tia1->dirty_ + 1);
                    // Test.
                    NSFX_TEST_EXPECT(tl1.Exists(tagId-1, 0));
                    NSFX_TEST_EXPECT(tl1.Exists(tagId-1, 100-1));
                    Tag tag = tl1.Get(tagId-1, 0);
                }
                while (tia1->dirty_ < tia1->capacity_);
                // Examine the array.
                NSFX_TEST_EXPECT_EQ(tia1->refCount_, 1);
                NSFX_TEST_EXPECT_EQ(tia1->dirty_, tia1->capacity_);
                NSFX_TEST_EXPECT_EQ(b.GetStorage()->refCount_, tia1->dirty_ + 1);
            }
            // Release the tag list also releases all tags.
            NSFX_TEST_EXPECT_EQ(b.GetStorage()->refCount_, 1);
        }

        NSFX_TEST_SUITE(NonShared)
        {
            NSFX_TEST_CASE(Compact)
            {
                TagBuffer b(16);
                // Create an empty list.
                TagList tl1(6, 400);
                size_t tagId = 1;
                // Add 4 tags.
                // |<--------------buffer------------->|
                // 0        100      200      300      400
                // |--------|--------|--------|--------|
                // |<-tag1->|                 |<-tag4->|
                // |<------tag2----->|<------tag3----->|
                tl1.Insert(tagId++, b,   0, 100);
                tl1.Insert(tagId++, b,   0, 200);
                tl1.Insert(tagId++, b, 200, 200);
                tl1.Insert(tagId++, b, 300, 100);
                // Shrink the buffer.
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
                const TagIndexArray* tia1 = tl1.GetTagIndexArray();
                // Let 'tl2' share and fill the array.
                // The newly added tags are not in 'tl1'.
                // Then release 'tl2'.
                {
                    TagList tl2(tl1);
                    tl2.Insert(tagId++, b, 0, 100);
                    tl2.Insert(tagId++, b, 0, 100);
                }
                size_t num1 = tia1->dirty_;
                // Add tag7 to trigger compaction.
                tl1.Insert(tagId++, b, 0, 200);
                // The array is not reallocated.
                const TagIndexArray* tia2 = tl1.GetTagIndexArray();
                NSFX_TEST_EXPECT_EQ(tia1, tia2);
                // The tags that are not in 'tl1' are removed.
                // The tags whose bytes are outside of the buffer are also removed.
                NSFX_TEST_EXPECT_EQ(tl1.GetSize(), 3);
                NSFX_TEST_EXPECT_EQ(tl1.GetInternalSize(), 3);
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
                TagBuffer b(16);
                // Create an empty list.
                TagList tl1(4, 400);
                size_t tagId = 1;
                // Add 4 tags to fill the array.
                // |<--------------buffer------------->|
                // 0        100      200      300      400
                // |--------|--------|--------|--------|
                // |<-tag1->|                 |<-tag4->|
                // |<------tag2----->|<------tag3----->|
                tl1.Insert(tagId++, b,   0, 100);
                tl1.Insert(tagId++, b,   0, 200);
                tl1.Insert(tagId++, b, 200, 200);
                tl1.Insert(tagId++, b, 300, 100);
                const TagIndexArray* tia1 = tl1.GetTagIndexArray();
                // Add tag5 to trigger reallocation.
                tl1.Insert(tagId++, b, 0, 400);
                // The array is reallocated.
                const TagIndexArray* tia2 = tl1.GetTagIndexArray();
                NSFX_TEST_EXPECT_NE(tia1, tia2);
                // The tags whose bytes are outside of the buffer are removed.
                NSFX_TEST_EXPECT_EQ(tl1.GetSize(), 5);
                NSFX_TEST_EXPECT_EQ(tl1.GetInternalSize(), 5);
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
                TagBuffer b(16);
                // Create an empty list.
                TagList tl1(4, 400);
                size_t tagId = 1;
                // Add 4 tags to fill the array.
                // |<--------------buffer------------->|
                // 0        100      200      300      400
                // |--------|--------|--------|--------|
                // |<-tag1->|                 |<-tag4->|
                // |<------tag2----->|<------tag3----->|
                tl1.Insert(tagId++, b,   0, 100);
                tl1.Insert(tagId++, b,   0, 200);
                tl1.Insert(tagId++, b, 200, 200);
                tl1.Insert(tagId++, b, 300, 100);
                // Shrink the buffer.
                // The bytes of tag1 and tag4 are outside of the buffer.
                // The bytes of tag2 and tag3 are partially inside of the buffer.
                //          |<-----buffer---->|
                //          0        100      200
                // |--------|--------|--------|--------|
                // |<-tag1->|                 |<-tag4->|
                // |<------tag2----->|<------tag3----->|
                tl1.RemoveAtStart(100);
                tl1.RemoveAtEnd(100);
                const TagIndexArray* tia1 = tl1.GetTagIndexArray();
                // Share the array with 'tl2'.
                TagList tl2(tl1);
                // Add tag5 to trigger reallocation.
                tl1.Insert(tagId++, b, 0, 200);
                // The array is reallocated.
                const TagIndexArray* tia2 = tl1.GetTagIndexArray();
                NSFX_TEST_EXPECT_NE(tia1, tia2);
                // The tags whose bytes are outside of the buffer are removed.
                NSFX_TEST_EXPECT_EQ(tl1.GetSize(), 3);
                NSFX_TEST_EXPECT_EQ(tl1.GetInternalSize(), 3);
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
                    TagBuffer b(16);
                    // Create an empty list.
                    TagList tl1(5, 400);
                    size_t tagId = 1;
                    // Add 4 tags.
                    // |<--------------buffer------------->|
                    // 0        100      200      300      400
                    // |--------|--------|--------|--------|
                    // |<-tag1->|                 |<-tag4->|
                    // |<------tag2----->|<------tag3----->|
                    tl1.Insert(tagId++, b,   0, 100);
                    tl1.Insert(tagId++, b,   0, 200);
                    tl1.Insert(tagId++, b, 200, 200);
                    tl1.Insert(tagId++, b, 300, 100);
                    // Share the array with 'tl2'.
                    TagList tl2(tl1);
                    // Shrink the buffer.
                    // The bytes of tag1 and tag4 are outside of the buffer.
                    // The bytes of tag2 and tag3 are partially inside of the buffer.
                    //          |<-----buffer---->|
                    //          0        100      200
                    // |--------|--------|--------|--------|
                    // |<-tag1->|                 |<-tag4->|
                    // |<------tag2----->|<------tag3----->|
                    tl1.RemoveAtStart(100);
                    tl1.RemoveAtEnd(100);
                    const TagIndexArray* tia1 = tl1.GetTagIndexArray();
                    // Add tag5 to 'tl1'.
                    tl1.Insert(tagId++, b, 0, 200);
                    // The array is not reallocated.
                    const TagIndexArray* tia2 = tl1.GetTagIndexArray();
                    NSFX_TEST_EXPECT_EQ(tia1, tia2);
                    // The tags whose bytes are outside of the buffer are not
                    // removed, since the array is shared and cannot be
                    // compacted.
                    NSFX_TEST_EXPECT_EQ(tl1.GetSize(), 3);
                    NSFX_TEST_EXPECT_EQ(tl1.GetInternalSize(), 5);
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
                    TagBuffer b(16);
                    // Create an empty list.
                    TagList tl1(6, 400);
                    size_t tagId = 1;
                    // Add 4 tags.
                    // |<--------------buffer------------->|
                    // 0        100      200      300      400
                    // |--------|--------|--------|--------|
                    // |<-tag1->|                 |<-tag4->|
                    // |<------tag2----->|<------tag3----->|
                    tl1.Insert(tagId++, b,   0, 100);
                    tl1.Insert(tagId++, b,   0, 200);
                    tl1.Insert(tagId++, b, 200, 200);
                    tl1.Insert(tagId++, b, 300, 100);
                    // Shrink the buffer.
                    // The bytes of tag1 and tag4 are outside of the buffer.
                    // The bytes of tag2 and tag3 are partially inside of the buffer.
                    //          |<-----buffer---->|
                    //          0        100      200
                    // |--------|--------|--------|--------|
                    // |<-tag1->|                 |<-tag4->|
                    // |<------tag2----->|<------tag3----->|
                    tl1.RemoveAtStart(100);
                    tl1.RemoveAtEnd(100);
                    const TagIndexArray* tia1 = tl1.GetTagIndexArray();
                    // Share the array with 'tl2'.
                    TagList tl2(tl1);
                    // Add tag5 to 'tl2', which is not in 'tl1'.
                    tl2.Insert(tagId++, b, 0, 200);
                    // Add tag6 to 'tl1' to trigger reallocation.
                    tl1.Insert(tagId++, b, 0, 200);
                    // The array is reallocated, even if the array has free elements.
                    const TagIndexArray* tia2 = tl1.GetTagIndexArray();
                    NSFX_TEST_EXPECT_NE(tia1, tia2);
                    // The tags whose bytes are outside of the buffer are removed.
                    NSFX_TEST_EXPECT_EQ(tl1.GetSize(), 3);
                    NSFX_TEST_EXPECT_EQ(tl1.GetInternalSize(), 3);
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
    }/*}}}*/

    NSFX_TEST_SUITE(AddAtStart)/*{{{*/
    {
        NSFX_TEST_CASE(FromEmpty)
        {
            // Create an empty list.
            TagList tl1;
            // Expand the buffer at start.
            tl1.AddAtStart(100);
            NSFX_TEST_EXPECT_EQ(tl1.GetInternalSize(), 0);
        }

        NSFX_TEST_CASE(NotShared)
        {
            TagBuffer b(16);
            // Create an empty list.
            TagList tl1(4, 400);
            size_t tagId = 1;
            // Add 4 tags.
            // |<--------------buffer------------->|
            // 0        100      200      300      400
            // |--------|--------|--------|--------|
            // |<-tag1->|                 |<-tag4->|
            // |<------tag2----->|<------tag3----->|
            tl1.Insert(tagId++, b,   0, 100);
            tl1.Insert(tagId++, b,   0, 200);
            tl1.Insert(tagId++, b, 200, 200);
            tl1.Insert(tagId++, b, 300, 100);
            // Shrink the buffer.
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
            // Expand the buffer at start.
            tl1.AddAtStart(100);
            // Expand the buffer.
            // |<---------buffer--------->|
            // 0        100      200      300
            // |--------|--------|--------|--------|
            // |<------tag2----->|<------tag3----->|
            // The tags whose bytes are outside of the buffer are also removed.
            NSFX_TEST_EXPECT_EQ(tl1.GetSize(), 2);
            NSFX_TEST_EXPECT_EQ(tl1.GetInternalSize(), 2);
            // tag2 survives.
            NSFX_TEST_EXPECT(tl1.Exists(2, 0));
            NSFX_TEST_EXPECT(tl1.Exists(2, 200-1));
            // tag3 survives.
            NSFX_TEST_EXPECT(tl1.Exists(3, 200));
            NSFX_TEST_EXPECT(tl1.Exists(3, 300-1));
        }

        NSFX_TEST_CASE(Shared)
        {
            TagBuffer b(16);
            // Create an empty list.
            TagList tl1(4, 400);
            size_t tagId = 1;
            // Add 4 tags.
            // |<--------------buffer------------->|
            // 0        100      200      300      400
            // |--------|--------|--------|--------|
            // |<-tag1->|                 |<-tag4->|
            // |<------tag2----->|<------tag3----->|
            tl1.Insert(tagId++, b,   0, 100);
            tl1.Insert(tagId++, b,   0, 200);
            tl1.Insert(tagId++, b, 200, 200);
            tl1.Insert(tagId++, b, 300, 100);
            // Shrink the buffer.
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
            // Share the array with 'tl2'.
            TagList tl2(tl1);
            // Expand the buffer at start.
            tl1.AddAtStart(100);
            // Expand the buffer.
            // |<---------buffer--------->|
            // 0        100      200      300
            // |--------|--------|--------|--------|
            // |<------tag2----->|<------tag3----->|
            // The tags whose bytes are outside of the buffer are also removed.
            NSFX_TEST_EXPECT_EQ(tl1.GetSize(), 2);
            NSFX_TEST_EXPECT_EQ(tl1.GetInternalSize(), 2);
            // tag2 survives.
            NSFX_TEST_EXPECT(tl1.Exists(2, 0));
            NSFX_TEST_EXPECT(tl1.Exists(2, 200-1));
            // tag3 survives.
            NSFX_TEST_EXPECT(tl1.Exists(3, 200));
            NSFX_TEST_EXPECT(tl1.Exists(3, 300-1));
        }
    }/*}}}*/

    NSFX_TEST_SUITE(AddAtEnd)/*{{{*/
    {
        NSFX_TEST_CASE(FromEmpty)
        {
            // Create an empty list.
            TagList tl1;
            // Expand the buffer at end.
            tl1.AddAtEnd(100);
            NSFX_TEST_EXPECT_EQ(tl1.GetInternalSize(), 0);
        }

        NSFX_TEST_CASE(NotShared)
        {
            TagBuffer b(16);
            // Create an empty list.
            TagList tl1(4, 400);
            size_t tagId = 1;
            // Add 4 tags.
            // |<--------------buffer------------->|
            // 0        100      200      300      400
            // |--------|--------|--------|--------|
            // |<-tag1->|                 |<-tag4->|
            // |<------tag2----->|<------tag3----->|
            tl1.Insert(tagId++, b,   0, 100);
            tl1.Insert(tagId++, b,   0, 200);
            tl1.Insert(tagId++, b, 200, 200);
            tl1.Insert(tagId++, b, 300, 100);
            // Shrink the buffer.
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
            // Expand the buffer at end.
            tl1.AddAtEnd(100);
            // Expand the buffer.
            //          |<---------buffer--------->|
            //          0        100      200      300
            // |--------|--------|--------|--------|
            // |<------tag2----->|<------tag3----->|
            // The tags whose bytes are outside of the buffer are also removed.
            NSFX_TEST_EXPECT_EQ(tl1.GetSize(), 2);
            NSFX_TEST_EXPECT_EQ(tl1.GetInternalSize(), 2);
            // tag2 survives.
            NSFX_TEST_EXPECT(tl1.Exists(2, 0));
            NSFX_TEST_EXPECT(tl1.Exists(2, 100-1));
            // tag3 survives.
            NSFX_TEST_EXPECT(tl1.Exists(3, 100));
            NSFX_TEST_EXPECT(tl1.Exists(3, 300-1));
        }

        NSFX_TEST_CASE(Shared)
        {
            TagBuffer b(16);
            // Create an empty list.
            TagList tl1(4, 400);
            size_t tagId = 1;
            // Add 4 tags.
            // |<--------------buffer------------->|
            // 0        100      200      300      400
            // |--------|--------|--------|--------|
            // |<-tag1->|                 |<-tag4->|
            // |<------tag2----->|<------tag3----->|
            tl1.Insert(tagId++, b,   0, 100);
            tl1.Insert(tagId++, b,   0, 200);
            tl1.Insert(tagId++, b, 200, 200);
            tl1.Insert(tagId++, b, 300, 100);
            // Shrink the buffer.
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
            // Share the array with 'tl2'.
            TagList tl2(tl1);
            // Expand the buffer at end.
            tl1.AddAtEnd(100);
            // Expand the buffer.
            //          |<---------buffer--------->|
            //          0        100      200      300
            // |--------|--------|--------|--------|
            // |<------tag2----->|<------tag3----->|
            // The tags whose bytes are outside of the buffer are also removed.
            NSFX_TEST_EXPECT_EQ(tl1.GetSize(), 2);
            NSFX_TEST_EXPECT_EQ(tl1.GetInternalSize(), 3);
            // tag2 survives.
            NSFX_TEST_EXPECT(tl1.Exists(2, 0));
            NSFX_TEST_EXPECT(tl1.Exists(2, 100-1));
            // tag3 survives.
            NSFX_TEST_EXPECT(tl1.Exists(3, 100));
            NSFX_TEST_EXPECT(tl1.Exists(3, 300-1));
        }
    }/*}}}*/

    NSFX_TEST_SUITE(ReassembleFragments)/*{{{*/
    {
        NSFX_TEST_CASE(Test)
        {
            TagBuffer b(16);
            {
                // Create an empty list.
                TagList tl1(4, 400);
                size_t tagId = 1;
                // Add 4 tags.
                // |<--------------buffer------------->|
                // 0        100      200      300      400
                // |--------|--------|--------|--------|
                // |<-tag1->|                 |<-tag4->|
                // |<------tag2----->|<------tag3----->|
                tl1.Insert(tagId++, b,   0, 100);
                tl1.Insert(tagId++, b,   0, 200);
                tl1.Insert(tagId++, b, 200, 200);
                tl1.Insert(tagId++, b, 300, 100);
                // Create fragments.
                // | f1 |
                // 0    50
                // |----|---|--------|--------|--------|
                // |<-tag1->|                 |<-tag4->|
                // |<------tag2----->|<------tag3----->|
                TagList f1(tl1);
                f1.RemoveAtEnd(350);
                NSFX_TEST_EXPECT_EQ(f1.GetSize(), 2);
                NSFX_TEST_EXPECT(f1.Exists(1, 0));
                NSFX_TEST_EXPECT(f1.Exists(2, 0));
                //      |f2 |
                //      0   50
                // |----|---|--------|--------|--------|
                // |<-tag1->|                 |<-tag4->|
                // |<------tag2----->|<------tag3----->|
                TagList f2(tl1);
                f2.RemoveAtStart(50);
                f2.RemoveAtEnd(300);
                NSFX_TEST_EXPECT_EQ(f2.GetSize(), 2);
                NSFX_TEST_EXPECT(f2.Exists(1, 0));
                NSFX_TEST_EXPECT(f2.Exists(2, 0));
                //          |     f3      |
                //          0             150
                // |----|---|-------------|---|--------|
                // |<-tag1->|                 |<-tag4->|
                // |<------tag2----->|<------tag3----->|
                TagList f3(tl1);
                f3.RemoveAtStart(100);
                f3.RemoveAtEnd(150);
                NSFX_TEST_EXPECT_EQ(f3.GetSize(), 2);
                NSFX_TEST_EXPECT(f3.Exists(2, 0));
                NSFX_TEST_EXPECT(f3.Exists(3, 100));
                //                        |       f4   |
                //                        0   50       150
                // |----|---|--------|----|---|--------|
                // |<-tag1->|                 |<-tag4->|
                // |<------tag2----->|<------tag3----->|
                TagList f4(tl1);
                f4.RemoveAtStart(250);
                NSFX_TEST_EXPECT_EQ(f4.GetSize(), 2);
                NSFX_TEST_EXPECT(f4.Exists(3, 0));
                NSFX_TEST_EXPECT(f4.Exists(4, 50));
                // Reassemble the fragments.
                {
                    TagList r;
                    r.AddAtStart(f4);
                    r.AddAtStart(f3);
                    r.AddAtStart(f2);
                    r.AddAtStart(f1);
                    NSFX_TEST_EXPECT_EQ(r.GetSize(), 4);
                    NSFX_TEST_EXPECT_EQ(r.GetInternalSize(), 4);
                    // tag1 survives.
                    NSFX_TEST_EXPECT(r.Exists(1, 0));
                    NSFX_TEST_EXPECT(r.Exists(1, 100-1));
                    // tag2 survives.
                    NSFX_TEST_EXPECT(r.Exists(2, 0));
                    NSFX_TEST_EXPECT(r.Exists(2, 200-1));
                    // tag3 survives.
                    NSFX_TEST_EXPECT(r.Exists(3, 200));
                    NSFX_TEST_EXPECT(r.Exists(3, 400-1));
                    // tag4 survives.
                    NSFX_TEST_EXPECT(r.Exists(4, 300));
                    NSFX_TEST_EXPECT(r.Exists(4, 400-1));
                }
                // Reassemble the fragments.
                {
                    TagList r;
                    r.AddAtEnd(f1);
                    r.AddAtEnd(f2);
                    r.AddAtEnd(f3);
                    r.AddAtEnd(f4);
                    NSFX_TEST_EXPECT_EQ(r.GetSize(), 4);
                    NSFX_TEST_EXPECT_EQ(r.GetInternalSize(), 4);
                    // tag1 survives.
                    NSFX_TEST_EXPECT(r.Exists(1, 0));
                    NSFX_TEST_EXPECT(r.Exists(1, 100-1));
                    // tag2 survives.
                    NSFX_TEST_EXPECT(r.Exists(2, 0));
                    NSFX_TEST_EXPECT(r.Exists(2, 200-1));
                    // tag3 survives.
                    NSFX_TEST_EXPECT(r.Exists(3, 200));
                    NSFX_TEST_EXPECT(r.Exists(3, 400-1));
                    // tag4 survives.
                    NSFX_TEST_EXPECT(r.Exists(4, 300));
                    NSFX_TEST_EXPECT(r.Exists(4, 400-1));
                }
            }
            NSFX_TEST_EXPECT_EQ(b.GetStorage()->refCount_, 1);
        }
    }/*}}}*/
}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

