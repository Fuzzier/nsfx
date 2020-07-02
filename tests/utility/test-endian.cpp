/**
 * @file
 *
 * @brief Test nop.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2019-12-01
 *
 * @copyright Copyright (c) 2019.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/utility/endian.h>
#include <iostream>


NSFX_TEST_SUITE(endian)
{
    using namespace nsfx;

    NSFX_TEST_CASE(ReorderBytes)
    {
        uint8_t u8 = 0xec;
        uint8_t r8 = 0xec;
        NSFX_TEST_EXPECT_EQ(ReorderBytes(u8, same_byte_order_t()),    r8);
        NSFX_TEST_EXPECT_EQ(ReorderBytes(u8, reverse_byte_order_t()), r8);
#if BOOST_ENDIAN_BIG_BYTE
        NSFX_TEST_EXPECT_EQ(NativeToLittleEndian(u8), r8);
        NSFX_TEST_EXPECT_EQ(LittleToNativeEndian(u8), r8);
        NSFX_TEST_EXPECT_EQ(NativeToBigEndian(u8),    u8);
        NSFX_TEST_EXPECT_EQ(BigToNativeEndian(u8),    u8);
#else
        NSFX_TEST_EXPECT_EQ(NativeToLittleEndian(u8), u8);
        NSFX_TEST_EXPECT_EQ(LittleToNativeEndian(u8), u8);
        NSFX_TEST_EXPECT_EQ(NativeToBigEndian(u8),    r8);
        NSFX_TEST_EXPECT_EQ(BigToNativeEndian(u8),    r8);
#endif
        NSFX_TEST_EXPECT_EQ(LittleToBigEndian(u8), r8);
        NSFX_TEST_EXPECT_EQ(BigToLittleEndian(u8), r8);

        uint16_t u16 = 0xecf0;
        uint16_t r16 = 0xf0ec;
        NSFX_TEST_EXPECT_EQ(ReorderBytes(u16, same_byte_order_t()),    u16);
        NSFX_TEST_EXPECT_EQ(ReorderBytes(u16, reverse_byte_order_t()), r16);
#if BOOST_ENDIAN_BIG_BYTE
        NSFX_TEST_EXPECT_EQ(NativeToLittleEndian(u16), r16);
        NSFX_TEST_EXPECT_EQ(LittleToNativeEndian(u16), r16);
        NSFX_TEST_EXPECT_EQ(NativeToBigEndian(u16),    u16);
        NSFX_TEST_EXPECT_EQ(BigToNativeEndian(u16),    u16);
#else
        NSFX_TEST_EXPECT_EQ(NativeToLittleEndian(u16), u16);
        NSFX_TEST_EXPECT_EQ(LittleToNativeEndian(u16), u16);
        NSFX_TEST_EXPECT_EQ(NativeToBigEndian(u16),    r16);
        NSFX_TEST_EXPECT_EQ(BigToNativeEndian(u16),    r16);
#endif
        NSFX_TEST_EXPECT_EQ(LittleToBigEndian(u16), r16);
        NSFX_TEST_EXPECT_EQ(BigToLittleEndian(u16), r16);

        uint32_t u32 = 0xcda8ecf0;
        uint32_t r32 = 0xf0eca8cd;
        NSFX_TEST_EXPECT_EQ(ReorderBytes(u32, same_byte_order_t()),    u32);
        NSFX_TEST_EXPECT_EQ(ReorderBytes(u32, reverse_byte_order_t()), r32);
#if BOOST_ENDIAN_BIG_BYTE
        NSFX_TEST_EXPECT_EQ(NativeToLittleEndian(u32), r32);
        NSFX_TEST_EXPECT_EQ(LittleToNativeEndian(u32), r32);
        NSFX_TEST_EXPECT_EQ(NativeToBigEndian(u32),    u32);
        NSFX_TEST_EXPECT_EQ(BigToNativeEndian(u32),    u32);
#else
        NSFX_TEST_EXPECT_EQ(NativeToLittleEndian(u32), u32);
        NSFX_TEST_EXPECT_EQ(LittleToNativeEndian(u32), u32);
        NSFX_TEST_EXPECT_EQ(NativeToBigEndian(u32),    r32);
        NSFX_TEST_EXPECT_EQ(BigToNativeEndian(u32),    r32);
#endif
        NSFX_TEST_EXPECT_EQ(LittleToBigEndian(u32), r32);
        NSFX_TEST_EXPECT_EQ(BigToLittleEndian(u32), r32);

        uint64_t u64 = 0xcda8ecf08301361a;
        uint64_t r64 = 0x1a360183f0eca8cd;
        NSFX_TEST_EXPECT_EQ(ReorderBytes(u64, same_byte_order_t()),    u64);
        NSFX_TEST_EXPECT_EQ(ReorderBytes(u64, reverse_byte_order_t()), r64);
#if BOOST_ENDIAN_BIG_BYTE
        NSFX_TEST_EXPECT_EQ(NativeToLittleEndian(u64), r64);
        NSFX_TEST_EXPECT_EQ(LittleToNativeEndian(u64), r64);
        NSFX_TEST_EXPECT_EQ(NativeToBigEndian(u64),    u64);
        NSFX_TEST_EXPECT_EQ(BigToNativeEndian(u64),    u64);
#else
        NSFX_TEST_EXPECT_EQ(NativeToLittleEndian(u64), u64);
        NSFX_TEST_EXPECT_EQ(LittleToNativeEndian(u64), u64);
        NSFX_TEST_EXPECT_EQ(NativeToBigEndian(u64),    r64);
        NSFX_TEST_EXPECT_EQ(BigToNativeEndian(u64),    r64);
#endif
        NSFX_TEST_EXPECT_EQ(LittleToBigEndian(u64), r64);
        NSFX_TEST_EXPECT_EQ(BigToLittleEndian(u64), r64);
    }

}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cerr);
    nsfx::test::runner::Run();

    return 0;
}

