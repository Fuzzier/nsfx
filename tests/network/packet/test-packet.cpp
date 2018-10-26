/**
 * @file
 *
 * @brief Test Packet.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-06-05
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/network/packet/packet.h>
#include <iostream>


NSFX_TEST_SUITE(Packet)
{
    NSFX_TEST_CASE(Ctor)/*{{{*/
    {
        nsfx::PacketBuffer b0(1000, 700, 300);
        // [700 s zs 300 ze e 300]
        b0.AddAtStart(100);
        // [600 s 100 zs 300 ze e 300]
        b0.AddAtEnd(100);
        // [600 s 100 zs 300 ze 100 e 200]
        nsfx::Packet p0(b0);
        NSFX_TEST_EXPECT_EQ(p0.GetSize(), 500);
        NSFX_TEST_EXPECT(b0.cbegin() == p0.GetBuffer().cbegin());
        NSFX_TEST_EXPECT(b0.cend() == p0.GetBuffer().cend());
    }/*}}}*/

    NSFX_TEST_CASE(Add)/*{{{*/
    {
        nsfx::PacketBuffer b0(1000, 700, 300);
        // [700 s zs 300 ze e 300]
        nsfx::Packet p0(b0);
        // Add header.
        nsfx::PacketBuffer h = p0.AddHeader(100);
        auto it = h.begin();
        for (size_t i = 0; i < 100; ++i)
        {
            uint8_t v = (uint8_t)(0xfe + i);
            it.Write<uint8_t>(v);
        }
        // Add trailer.
        nsfx::PacketBuffer t = p0.AddTrailer(100);
        it = t.begin();
        for (size_t i = 0; i < 100; ++i)
        {
            uint8_t v = (uint8_t)(0xef + i);
            it.Write<uint8_t>(v);
        }
        // Test
        auto it1 = p0.GetBuffer().cbegin();
        for (size_t i = 0; i < 100; ++i)
        {
            uint8_t v = (uint8_t)(0xfe + i);
            NSFX_TEST_EXPECT_EQ(it1.Read<uint8_t>(), v);
        }
        it1 += 300;
        for (size_t i = 0; i < 100; ++i)
        {
            uint8_t v = (uint8_t)(0xef + i);
            NSFX_TEST_EXPECT_EQ(it1.Read<uint8_t>(), v);
        }
    }/*}}}*/

    NSFX_TEST_CASE(Remove)/*{{{*/
    {
        nsfx::PacketBuffer b0(1000, 700, 300);
        // [700 s zs 300 ze e 300]
        nsfx::Packet p0(b0);
        // Add header.
        nsfx::PacketBuffer h = p0.AddHeader(100);
        auto it = h.begin();
        for (size_t i = 0; i < 100; ++i)
        {
            uint8_t v = (uint8_t)(0xfe + i);
            it.Write<uint8_t>(v);
        }
        // Add trailer.
        nsfx::PacketBuffer t = p0.AddTrailer(100);
        it = t.begin();
        for (size_t i = 0; i < 100; ++i)
        {
            uint8_t v = (uint8_t)(0xef + i);
            it.Write<uint8_t>(v);
        }
        // Remove header.
        p0.RemoveHeader(50);
        // Remove trailer.
        p0.RemoveTrailer(50);
        // Test
        auto it1 = p0.GetBuffer().cbegin();
        for (size_t i = 0; i < 50; ++i)
        {
            uint8_t v = (uint8_t)(0xfe + 50 + i);
            NSFX_TEST_EXPECT_EQ(it1.Read<uint8_t>(), v);
        }
        for (size_t i = 0; i < 300; ++i)
        {
            NSFX_TEST_EXPECT_EQ(it1.Read<uint8_t>(), 0);
        }
        for (size_t i = 0; i < 50; ++i)
        {
            uint8_t v = (uint8_t)(0xef + i);
            NSFX_TEST_EXPECT_EQ(it1.Read<uint8_t>(), v);
        }
    }/*}}}*/

    NSFX_TEST_CASE(ReassembleFragments)/*{{{*/
    {
        nsfx::PacketBuffer b0(1000, 700, 300);
        // [700 s zs 300 ze e 300]
        nsfx::Packet p0(b0);
        // Add header.
        nsfx::PacketBuffer h = p0.AddHeader(100);
        auto it = h.begin();
        for (size_t i = 0; i < 100; ++i)
        {
            uint8_t v = (uint8_t)(0xfe + i);
            it.Write<uint8_t>(v);
        }
        // Add trailer.
        nsfx::PacketBuffer t = p0.AddTrailer(100);
        it = t.begin();
        for (size_t i = 0; i < 100; ++i)
        {
            uint8_t v = (uint8_t)(0xef + i);
            it.Write<uint8_t>(v);
        }
        // Fragmentation.
        nsfx::Packet f0 = p0.MakeFragment(50, 200);
        nsfx::Packet f1 = p0.MakeFragment(250, 200);
        // Test
        auto it1 = f0.GetBuffer().cbegin();
        for (size_t i = 0; i < 50; ++i)
        {
            uint8_t v = (uint8_t)(0xfe + 50 + i);
            NSFX_TEST_EXPECT_EQ(it1.Read<uint8_t>(), v);
        }
        for (size_t i = 0; i < 150; ++i)
        {
            NSFX_TEST_EXPECT_EQ(it1.Read<uint8_t>(), 0);
        }
        it1 = f1.GetBuffer().cbegin();
        for (size_t i = 0; i < 150; ++i)
        {
            NSFX_TEST_EXPECT_EQ(it1.Read<uint8_t>(), 0);
        }
        for (size_t i = 0; i < 50; ++i)
        {
            uint8_t v = (uint8_t)(0xef + i);
            NSFX_TEST_EXPECT_EQ(it1.Read<uint8_t>(), v);
        }
        // Reassembly.
        nsfx::Packet p1(f1);
        p1.AddHeader(f0);
        // Test
        it1 = p1.GetBuffer().cbegin();
        for (size_t i = 0; i < 50; ++i)
        {
            uint8_t v = (uint8_t)(0xfe + 50 + i);
            NSFX_TEST_EXPECT_EQ(it1.Read<uint8_t>(), v);
        }
        for (size_t i = 0; i < 300; ++i)
        {
            NSFX_TEST_EXPECT_EQ(it1.Read<uint8_t>(), 0);
        }
        for (size_t i = 0; i < 50; ++i)
        {
            uint8_t v = (uint8_t)(0xef + i);
            NSFX_TEST_EXPECT_EQ(it1.Read<uint8_t>(), v);
        }
        // Reassembly.
        nsfx::Packet p2(f0);
        p2.AddTrailer(f1);
        // Test
        it1 = p2.GetBuffer().cbegin();
        for (size_t i = 0; i < 50; ++i)
        {
            uint8_t v = (uint8_t)(0xfe + 50 + i);
            NSFX_TEST_EXPECT_EQ(it1.Read<uint8_t>(), v);
        }
        for (size_t i = 0; i < 300; ++i)
        {
            NSFX_TEST_EXPECT_EQ(it1.Read<uint8_t>(), 0);
        }
        for (size_t i = 0; i < 50; ++i)
        {
            uint8_t v = (uint8_t)(0xef + i);
            NSFX_TEST_EXPECT_EQ(it1.Read<uint8_t>(), v);
        }
    }/*}}}*/

    NSFX_TEST_CASE(ByteTag)/*{{{*/
    {
        nsfx::TagBuffer tb(16);
        {
            nsfx::PacketBuffer b0(1000, 700, 400);
            nsfx::Packet p0(b0);
            // [700 s zs 400 ze e 300]
            uint32_t tagId = 1;
            // Add 4 tags.
            // |<--------------buffer------------->|
            // 0        100      200      300      400
            // |--------|--------|--------|--------|
            // |<-tag1->|                 |<-tag4->|
            // |<------tag2----->|<------tag3----->|
            p0.AddByteTag(tagId++, tb,   0, 100);
            p0.AddByteTag(tagId++, tb,   0, 200);
            p0.AddByteTag(tagId++, tb, 200, 200);
            p0.AddByteTag(tagId++, tb, 300, 100);
            // Create fragments.
            // | f1 |
            // 0    50
            // |----|---|--------|--------|--------|
            // |<-tag1->|                 |<-tag4->|
            // |<------tag2----->|<------tag3----->|
            nsfx::Packet f1 = p0.MakeFragment(0, 50);
            NSFX_TEST_EXPECT(f1.HasByteTag(1, 0));
            NSFX_TEST_EXPECT(f1.HasByteTag(2, 0));
            //      |f2 |
            //      0   50
            // |----|---|--------|--------|--------|
            // |<-tag1->|                 |<-tag4->|
            // |<------tag2----->|<------tag3----->|
            nsfx::Packet f2 = p0.MakeFragment(50, 50);
            NSFX_TEST_EXPECT(f2.HasByteTag(1, 0));
            NSFX_TEST_EXPECT(f2.HasByteTag(2, 0));
            //          |     f3      |
            //          0             150
            // |----|---|-------------|---|--------|
            // |<-tag1->|                 |<-tag4->|
            // |<------tag2----->|<------tag3----->|
            nsfx::Packet f3 = p0.MakeFragment(100, 150);
            NSFX_TEST_EXPECT(f3.HasByteTag(2, 0));
            NSFX_TEST_EXPECT(f3.HasByteTag(3, 100));
            //                        |       f4   |
            //                        0   50       150
            // |----|---|--------|----|---|--------|
            // |<-tag1->|                 |<-tag4->|
            // |<------tag2----->|<------tag3----->|
            nsfx::Packet f4 = p0.MakeFragment(250, 150);
            NSFX_TEST_EXPECT(f4.HasByteTag(3, 0));
            NSFX_TEST_EXPECT(f4.HasByteTag(4, 50));
            // Reassemble the fragments.
            {
                nsfx::Packet r(f4);
                r.AddHeader(f3);
                r.AddHeader(f2);
                r.AddHeader(f1);
                // tag1 survives.
                NSFX_TEST_EXPECT(r.HasByteTag(1, 0));
                NSFX_TEST_EXPECT(r.HasByteTag(1, 100-1));
                // tag2 survives.
                NSFX_TEST_EXPECT(r.HasByteTag(2, 0));
                NSFX_TEST_EXPECT(r.HasByteTag(2, 200-1));
                // tag3 survives.
                NSFX_TEST_EXPECT(r.HasByteTag(3, 200));
                NSFX_TEST_EXPECT(r.HasByteTag(3, 400-1));
                // tag4 survives.
                NSFX_TEST_EXPECT(r.HasByteTag(4, 300));
                NSFX_TEST_EXPECT(r.HasByteTag(4, 400-1));
                nsfx::ConstTagBuffer b4 = r.GetByteTag(4, 300);
                NSFX_TEST_EXPECT_EQ(b4.GetSize(), 16);
            }
            // Reassemble the fragments.
            {
                nsfx::Packet r(f1);
                r.AddTrailer(f2);
                r.AddTrailer(f3);
                r.AddTrailer(f4);
                // tag1 survives.
                NSFX_TEST_EXPECT(r.HasByteTag(1, 0));
                NSFX_TEST_EXPECT(r.HasByteTag(1, 100-1));
                // tag2 survives.
                NSFX_TEST_EXPECT(r.HasByteTag(2, 0));
                NSFX_TEST_EXPECT(r.HasByteTag(2, 200-1));
                // tag3 survives.
                NSFX_TEST_EXPECT(r.HasByteTag(3, 200));
                NSFX_TEST_EXPECT(r.HasByteTag(3, 400-1));
                // tag4 survives.
                NSFX_TEST_EXPECT(r.HasByteTag(4, 300));
                NSFX_TEST_EXPECT(r.HasByteTag(4, 400-1));
                nsfx::ConstTagBuffer b4 = r.GetByteTag(4, 400-1);
                NSFX_TEST_EXPECT_EQ(b4.GetSize(), 16);
            }
        }
        NSFX_TEST_EXPECT_EQ(tb.GetStorage()->refCount_, 1);
    }/*}}}*/

    NSFX_TEST_CASE(PacketTag)/*{{{*/
    {
        nsfx::PacketBuffer b0(1000, 700, 400);
        nsfx::Packet p0(b0);
        // [700 s zs 400 ze e 300]
        uint32_t tagId = 1;
        nsfx::TagBuffer tb(16);
        // Add 4 tags.
        // |<--------------buffer------------->|
        // 0        100      200      300      400
        // |--------|--------|--------|--------|
        // |<-tag1->|                 |<-tag4->|
        // |<------tag2----->|<------tag3----->|
        p0.AddByteTag(tagId++, tb,   0, 100);
        p0.AddByteTag(tagId++, tb,   0, 200);
        p0.AddByteTag(tagId++, tb, 200, 200);
        p0.AddByteTag(tagId++, tb, 300, 100);
        {
            nsfx::PacketBuffer b1(1000, 700, 400);
            nsfx::Packet p1(b1);
            // [700 s zs 400 ze e 300]
            tagId = 1;
            // Add 4 tags.
            // |<--------------buffer------------->|
            // 0        100      200      300      400
            // |--------|--------|--------|--------|
            // |<-tag1->|                 |<-tag4->|
            // |<------tag2----->|<------tag3----->|
            p1.AddPacketTag(tagId++, p0,   0, 100);
            p1.AddPacketTag(tagId++, p0,   0, 200);
            p1.AddPacketTag(tagId++, p0, 200, 200);
            p1.AddPacketTag(tagId++, p0, 300, 100);
            // Create fragments.
            // | f1 |
            // 0    50
            // |----|---|--------|--------|--------|
            // |<-tag1->|                 |<-tag4->|
            // |<------tag2----->|<------tag3----->|
            nsfx::Packet f1 = p1.MakeFragment(0, 50);
            NSFX_TEST_EXPECT(f1.HasPacketTag(1, 0));
            NSFX_TEST_EXPECT(f1.HasPacketTag(2, 0));
            //      |f2 |
            //      0   50
            // |----|---|--------|--------|--------|
            // |<-tag1->|                 |<-tag4->|
            // |<------tag2----->|<------tag3----->|
            nsfx::Packet f2 = p1.MakeFragment(50, 50);
            NSFX_TEST_EXPECT(f2.HasPacketTag(1, 0));
            NSFX_TEST_EXPECT(f2.HasPacketTag(2, 0));
            //          |     f3      |
            //          0             150
            // |----|---|-------------|---|--------|
            // |<-tag1->|                 |<-tag4->|
            // |<------tag2----->|<------tag3----->|
            nsfx::Packet f3 = p1.MakeFragment(100, 150);
            NSFX_TEST_EXPECT(f3.HasPacketTag(2, 0));
            NSFX_TEST_EXPECT(f3.HasPacketTag(3, 100));
            //                        |       f4   |
            //                        0   50       150
            // |----|---|--------|----|---|--------|
            // |<-tag1->|                 |<-tag4->|
            // |<------tag2----->|<------tag3----->|
            nsfx::Packet f4 = p1.MakeFragment(250, 150);
            NSFX_TEST_EXPECT(f4.HasPacketTag(3, 0));
            NSFX_TEST_EXPECT(f4.HasPacketTag(4, 50));
            // Reassemble the fragments.
            {
                nsfx::Packet r(f4);
                r.AddHeader(f3);
                r.AddHeader(f2);
                r.AddHeader(f1);
                // tag1 survives.
                NSFX_TEST_EXPECT(r.HasPacketTag(1, 0));
                NSFX_TEST_EXPECT(r.HasPacketTag(1, 100-1));
                // tag2 survives.
                NSFX_TEST_EXPECT(r.HasPacketTag(2, 0));
                NSFX_TEST_EXPECT(r.HasPacketTag(2, 200-1));
                // tag3 survives.
                NSFX_TEST_EXPECT(r.HasPacketTag(3, 200));
                NSFX_TEST_EXPECT(r.HasPacketTag(3, 400-1));
                // tag4 survives.
                NSFX_TEST_EXPECT(r.HasPacketTag(4, 300));
                NSFX_TEST_EXPECT(r.HasPacketTag(4, 400-1));
                nsfx::Packet p4 = r.GetPacketTag(4, 300);
                NSFX_TEST_EXPECT_EQ(p4.GetSize(), 400);
                p4.HasByteTag(1,   0);
                p4.HasByteTag(2, 100);
                p4.HasByteTag(3, 200);
                p4.HasByteTag(4, 300);
            }
            // Reassemble the fragments.
            {
                nsfx::Packet r(f1);
                r.AddTrailer(f2);
                r.AddTrailer(f3);
                r.AddTrailer(f4);
                // tag1 survives.
                NSFX_TEST_EXPECT(r.HasPacketTag(1, 0));
                NSFX_TEST_EXPECT(r.HasPacketTag(1, 100-1));
                // tag2 survives.
                NSFX_TEST_EXPECT(r.HasPacketTag(2, 0));
                NSFX_TEST_EXPECT(r.HasPacketTag(2, 200-1));
                // tag3 survives.
                NSFX_TEST_EXPECT(r.HasPacketTag(3, 200));
                NSFX_TEST_EXPECT(r.HasPacketTag(3, 400-1));
                // tag4 survives.
                NSFX_TEST_EXPECT(r.HasPacketTag(4, 300));
                NSFX_TEST_EXPECT(r.HasPacketTag(4, 400-1));
                nsfx::Packet p4 = r.GetPacketTag(4, 400-1);
                NSFX_TEST_EXPECT_EQ(p4.GetSize(), 400);
                p4.HasByteTag(1,   0);
                p4.HasByteTag(2, 100);
                p4.HasByteTag(3, 200);
                p4.HasByteTag(4, 300);
            }
        }
        NSFX_TEST_EXPECT_EQ(tb.GetStorage()->refCount_, 5);
    }/*}}}*/

    NSFX_TEST_CASE(CopyTags)/*{{{*/
    {
        nsfx::TagBuffer tb(16);
        {
            nsfx::PacketBuffer b0(1000, 700, 400);
            nsfx::Packet p0(b0);
            // [700 s zs 400 ze e 300]
            // Add 4 tags.
            // |<--------------buffer------------->|
            // 0        100      200      300      400
            // |--------|--------|--------|--------|
            // |<-tag1->|                 |<-tag4->|
            // |<------tag2----->|<------tag3----->|
            uint32_t tagId = 1;
            p0.AddByteTag(tagId++, tb,   0, 100);
            p0.AddByteTag(tagId++, tb,   0, 200);
            p0.AddPacketTag(tagId++, p0, 200, 200);
            p0.AddByteTag(tagId++, tb, 300, 100);

            nsfx::PacketBuffer b1(1000, 700, 400);
            nsfx::Packet p1(b1);
            // Add 2 tags.
            // |<--------------buffer------------->|
            // 0        100      200      300      400
            // |--------|--------|--------|--------|
            // |<------tag1----->|<------tag2----->|
            tagId = 1;
            p1.AddPacketTag(tagId++, p1, 0, 200);
            p1.AddByteTag(tagId++, tb, 200, 200);

            p1.CopyTagsFrom(p0);
            NSFX_TEST_EXPECT(p1.HasByteTag(1, 0));
            NSFX_TEST_EXPECT(p1.HasByteTag(2, 0));
            NSFX_TEST_EXPECT(p1.HasPacketTag(3, 200));
            NSFX_TEST_EXPECT(p1.HasByteTag(4, 300));

            nsfx::Packet p2 = p1.GetPacketTag(3, 200);
            NSFX_TEST_EXPECT(p2.HasByteTag(1, 0));
            NSFX_TEST_EXPECT(p2.HasByteTag(2, 0));
            NSFX_TEST_EXPECT(!p2.HasPacketTag(3, 200));
            NSFX_TEST_EXPECT(!p2.HasByteTag(4, 300));
        }
        NSFX_TEST_EXPECT_EQ(tb.GetStorage()->refCount_, 1);
    }/*}}}*/
}


int main(void)
{
    // nsfx::test::runner::GetLogger()->AddStreamSink(std::cerr);
    // for (size_t i = 0; i < 100000; ++i)
    // {
    //     nsfx::test::runner::Run();
    // }
    // clock_t t0 = clock();
    // for (size_t i = 0; i < 200000; ++i)
    // {
        nsfx::test::runner::Run();
    // }
    // clock_t t1 = clock();
    // std::cout << t1 - t0 << std::endl;

    return 0;
}

