/**
 * @file
 *
 * @brief Test TagIndex.
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


NSFX_TEST_SUITE(TagIndex)
{
    NSFX_TEST_CASE(Ctor)
    {
        size_t tagId1 = 4;
        size_t tagStart1 = 5;
        size_t tagEnd1 = 6;
        nsfx::TagBuffer b1(16);
        b1.begin().Write<uint8_t>(0xfe);
        nsfx::Tag tag1(tagId1, b1);

        nsfx::TagIndex idx1(tag1, tagStart1, tagEnd1);

        NSFX_TEST_EXPECT_EQ(idx1.GetTag().GetId(), tagId1);
        NSFX_TEST_EXPECT_EQ(idx1.GetStart(), tagStart1);
        NSFX_TEST_EXPECT_EQ(idx1.GetEnd(), tagEnd1);
        NSFX_TEST_EXPECT_EQ(idx1.GetTag().GetBuffer().GetSize(), 16);
        NSFX_TEST_EXPECT_EQ(idx1.GetTag().GetBuffer().cbegin().Read<uint8_t>(), 0xfe);
    }

    NSFX_TEST_CASE(CopyCtor)
    {
        size_t tagId1 = 4;
        size_t tagStart1 = 5;
        size_t tagEnd1 = 6;
        nsfx::TagBuffer b1(16);
        b1.begin().Write<uint8_t>(0xfe);
        nsfx::Tag tag1(tagId1, b1);
        nsfx::TagIndex idx1(tag1, tagStart1, tagEnd1);

        nsfx::TagIndex idx2(idx1);

        NSFX_TEST_EXPECT_EQ(idx2.GetTag().GetId(), tagId1);
        NSFX_TEST_EXPECT_EQ(idx2.GetStart(), tagStart1);
        NSFX_TEST_EXPECT_EQ(idx2.GetEnd(), tagEnd1);
        NSFX_TEST_EXPECT_EQ(idx2.GetTag().GetBuffer().GetSize(), 16);
        NSFX_TEST_EXPECT_EQ(idx2.GetTag().GetBuffer().cbegin().Read<uint8_t>(), 0xfe);
    }

    NSFX_TEST_CASE(CopyAssign)
    {
        size_t tagId1 = 4;
        size_t tagStart1 = 5;
        size_t tagEnd1 = 6;
        nsfx::TagBuffer b1(16);
        b1.begin().Write<uint8_t>(0xfe);
        nsfx::Tag tag1(tagId1, b1);
        nsfx::TagIndex idx1(tag1, tagStart1, tagEnd1);

        size_t tagId2 = 7;
        size_t tagStart2 = 8;
        size_t tagEnd2 = 9;
        nsfx::TagBuffer b2(32);
        b2.begin().Write<uint8_t>(0x31);
        nsfx::Tag tag2(tagId2, b2);
        nsfx::TagIndex idx2(tag2, tagStart2, tagEnd2);

        idx2 = idx1;

        NSFX_TEST_EXPECT_EQ(idx2.GetTag().GetId(), tagId1);
        NSFX_TEST_EXPECT_EQ(idx2.GetStart(), tagStart1);
        NSFX_TEST_EXPECT_EQ(idx2.GetEnd(), tagEnd1);
        NSFX_TEST_EXPECT_EQ(idx2.GetTag().GetBuffer().GetSize(), 16);
        NSFX_TEST_EXPECT_EQ(idx2.GetTag().GetBuffer().cbegin().Read<uint8_t>(), 0xfe);
    }

    NSFX_TEST_CASE(MoveCtor)
    {
        size_t tagId1 = 4;
        size_t tagStart1 = 5;
        size_t tagEnd1 = 6;
        nsfx::TagBuffer b1(16);
        b1.begin().Write<uint8_t>(0xfe);
        nsfx::Tag tag1(tagId1, b1);
        nsfx::TagIndex idx1(tag1, tagStart1, tagEnd1);

        nsfx::TagIndex idx2(std::move(idx1));

        NSFX_TEST_EXPECT_EQ(idx1.GetTag().GetId(), tagId1);
        NSFX_TEST_EXPECT_EQ(idx1.GetStart(), tagStart1);
        NSFX_TEST_EXPECT_EQ(idx1.GetEnd(), tagEnd1);
        NSFX_TEST_EXPECT_EQ(idx1.GetTag().GetBuffer().GetSize(), 0);

        NSFX_TEST_EXPECT_EQ(idx2.GetTag().GetId(), tagId1);
        NSFX_TEST_EXPECT_EQ(idx2.GetStart(), tagStart1);
        NSFX_TEST_EXPECT_EQ(idx2.GetEnd(), tagEnd1);
        NSFX_TEST_EXPECT_EQ(idx2.GetTag().GetBuffer().GetSize(), 16);
        NSFX_TEST_EXPECT_EQ(idx2.GetTag().GetBuffer().cbegin().Read<uint8_t>(), 0xfe);
    }

    NSFX_TEST_CASE(MoveAssign)
    {
        size_t tagId1 = 4;
        size_t tagStart1 = 5;
        size_t tagEnd1 = 6;
        nsfx::TagBuffer b1(16);
        b1.begin().Write<uint8_t>(0xfe);
        nsfx::Tag tag1(tagId1, b1);
        nsfx::TagIndex idx1(tag1, tagStart1, tagEnd1);

        size_t tagId2 = 7;
        size_t tagStart2 = 8;
        size_t tagEnd2 = 9;
        nsfx::TagBuffer b2(32);
        b2.begin().Write<uint8_t>(0x31);
        nsfx::Tag tag2(tagId2, b2);
        nsfx::TagIndex idx2(tag2, tagStart2, tagEnd2);

        idx2 = std::move(idx1);

        NSFX_TEST_EXPECT_EQ(idx1.GetTag().GetId(), tagId1);
        NSFX_TEST_EXPECT_EQ(idx1.GetStart(), tagStart1);
        NSFX_TEST_EXPECT_EQ(idx1.GetEnd(), tagEnd1);
        NSFX_TEST_EXPECT_EQ(idx1.GetTag().GetBuffer().GetSize(), 0);

        NSFX_TEST_EXPECT_EQ(idx2.GetTag().GetId(), tagId1);
        NSFX_TEST_EXPECT_EQ(idx2.GetStart(), tagStart1);
        NSFX_TEST_EXPECT_EQ(idx2.GetEnd(), tagEnd1);
        NSFX_TEST_EXPECT_EQ(idx2.GetTag().GetBuffer().GetSize(), 16);
        NSFX_TEST_EXPECT_EQ(idx2.GetTag().GetBuffer().cbegin().Read<uint8_t>(), 0xfe);
    }

    NSFX_TEST_CASE(Swap)
    {
        size_t tagId1 = 4;
        size_t tagStart1 = 5;
        size_t tagEnd1 = 6;
        nsfx::TagBuffer b1(16);
        b1.begin().Write<uint8_t>(0xfe);
        nsfx::Tag tag1(tagId1, b1);
        nsfx::TagIndex idx1(tag1, tagStart1, tagEnd1);

        size_t tagId2 = 7;
        size_t tagStart2 = 8;
        size_t tagEnd2 = 9;
        nsfx::TagBuffer b2(32);
        b2.begin().Write<uint8_t>(0x31);
        nsfx::Tag tag2(tagId2, b2);
        nsfx::TagIndex idx2(tag2, tagStart2, tagEnd2);

        boost::swap(idx1, idx2);

        NSFX_TEST_EXPECT_EQ(idx1.GetTag().GetId(), tagId2);
        NSFX_TEST_EXPECT_EQ(idx1.GetStart(), tagStart2);
        NSFX_TEST_EXPECT_EQ(idx1.GetEnd(), tagEnd2);
        NSFX_TEST_EXPECT_EQ(idx1.GetTag().GetBuffer().GetSize(), 32);
        NSFX_TEST_EXPECT_EQ(idx1.GetTag().GetBuffer().cbegin().Read<uint8_t>(), 0x31);

        NSFX_TEST_EXPECT_EQ(idx2.GetTag().GetId(), tagId1);
        NSFX_TEST_EXPECT_EQ(idx2.GetStart(), tagStart1);
        NSFX_TEST_EXPECT_EQ(idx2.GetEnd(), tagEnd1);
        NSFX_TEST_EXPECT_EQ(idx2.GetTag().GetBuffer().GetSize(), 16);
        NSFX_TEST_EXPECT_EQ(idx2.GetTag().GetBuffer().cbegin().Read<uint8_t>(), 0xfe);
    }

    NSFX_TEST_CASE(HasTaggedByte)
    {
        size_t tagId1 = 4;
        size_t tagStart1 = 5;
        size_t tagEnd1 = 6;
        nsfx::TagBuffer b1(16);
        b1.begin().Write<uint8_t>(0xfe);
        nsfx::Tag tag1(tagId1, b1);
        nsfx::TagIndex idx1(tag1, tagStart1, tagEnd1);

        NSFX_TEST_EXPECT(!idx1.HasTaggedByte(tagStart1, tagStart1));
        NSFX_TEST_EXPECT(idx1.HasTaggedByte(tagStart1, tagStart1 + 1));
        NSFX_TEST_EXPECT(idx1.HasTaggedByte(tagEnd1 - 1, tagEnd1));
        NSFX_TEST_EXPECT(!idx1.HasTaggedByte(tagEnd1, tagEnd1));
    }
}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

