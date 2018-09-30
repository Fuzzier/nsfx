/**
 * @file
 *
 * @brief Test Interoperability.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-06-24
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/interop/variant/variant.h>
#include <nsfx/chrono/virtual-time-point.h>
#include <iostream>


NSFX_TEST_SUITE(Variant)
{
    typedef nsfx::chrono::VirtualTimePoint TimePoint;
    typedef TimePoint::Duration            Duration;

    NSFX_TEST_CASE(empty)
    {
        nsfx::Variant  v0;
        NSFX_TEST_EXPECT_EQ(v0.GetType(), nsfx::VT_EMPTY);

        nsfx::Variant  v1;
        nsfx::Variant  v2(v0);
        NSFX_TEST_EXPECT_EQ(v2.GetType(), nsfx::VT_EMPTY);

        NSFX_TEST_EXPECT(v0 == v0);
        NSFX_TEST_EXPECT(v0 == v1);
        NSFX_TEST_EXPECT(v0 == v2);
        NSFX_TEST_EXPECT(!(v0 != v0));
        NSFX_TEST_EXPECT(!(v0 != v2));
    }

    NSFX_TEST_CASE(bool)
    {
        bool b0 = true;
        nsfx::Variant  v0(b0);
        NSFX_TEST_EXPECT_EQ(v0.GetType(), nsfx::VT_BOOL);
        NSFX_TEST_EXPECT_EQ(v0.GetValue<bool>(), b0);

        bool b1 = false;
        nsfx::Variant  v1(b1);
        nsfx::Variant  v2(v0);
        NSFX_TEST_EXPECT_EQ(v2.GetType(), nsfx::VT_BOOL);
        NSFX_TEST_EXPECT_EQ(v2.GetValue<bool>(), b0);

        NSFX_TEST_EXPECT(v0 == v0);
        NSFX_TEST_EXPECT(v0 != v1);
        NSFX_TEST_EXPECT(v0 == v2);
    }

    NSFX_TEST_CASE(int8_t)
    {
        int8_t i0 = 0xfe;
        nsfx::Variant  v0(i0);
        NSFX_TEST_EXPECT_EQ(v0.GetType(), nsfx::VT_INT8);
        NSFX_TEST_EXPECT_EQ(v0.GetValue<int8_t>(), i0);

        int8_t i1 = 0xef;
        nsfx::Variant  v1(i1);
        nsfx::Variant  v2(v0);
        NSFX_TEST_EXPECT_EQ(v2.GetType(), nsfx::VT_INT8);
        NSFX_TEST_EXPECT_EQ(v2.GetValue<int8_t>(), i0);

        NSFX_TEST_EXPECT(v0 == v0);
        NSFX_TEST_EXPECT(v0 != v1);
        NSFX_TEST_EXPECT(v0 == v2);
    }

    NSFX_TEST_CASE(uint8_t)
    {
        uint8_t u0 = 0xfe;
        nsfx::Variant  v0(u0);
        NSFX_TEST_EXPECT_EQ(v0.GetType(), nsfx::VT_UINT8);
        NSFX_TEST_EXPECT_EQ(v0.GetValue<uint8_t>(), u0);

        uint8_t u1 = 0xef;
        nsfx::Variant  v1(u1);
        nsfx::Variant  v2(v0);
        NSFX_TEST_EXPECT_EQ(v2.GetType(), nsfx::VT_UINT8);
        NSFX_TEST_EXPECT_EQ(v2.GetValue<uint8_t>(), u0);

        NSFX_TEST_EXPECT(v0 == v0);
        NSFX_TEST_EXPECT(v0 != v1);
        NSFX_TEST_EXPECT(v0 == v2);
    }

    NSFX_TEST_CASE(int16_t)
    {
        int16_t i0 = 0xfedc;
        nsfx::Variant  v0(i0);
        NSFX_TEST_EXPECT_EQ(v0.GetType(), nsfx::VT_INT16);
        NSFX_TEST_EXPECT_EQ(v0.GetValue<int16_t>(), i0);

        int16_t i1 = 0xcdef;
        nsfx::Variant  v1(i1);
        nsfx::Variant  v2(v0);
        NSFX_TEST_EXPECT_EQ(v2.GetType(), nsfx::VT_INT16);
        NSFX_TEST_EXPECT_EQ(v2.GetValue<int16_t>(), i0);

        NSFX_TEST_EXPECT(v0 == v0);
        NSFX_TEST_EXPECT(v0 != v1);
        NSFX_TEST_EXPECT(v0 == v2);
    }

    NSFX_TEST_CASE(uint16_t)
    {
        uint16_t u0 = 0xfedc;
        nsfx::Variant  v0(u0);
        NSFX_TEST_EXPECT_EQ(v0.GetType(), nsfx::VT_UINT16);
        NSFX_TEST_EXPECT_EQ(v0.GetValue<uint16_t>(), u0);

        uint16_t u1 = 0xcdef;
        nsfx::Variant  v1(u1);
        nsfx::Variant  v2(v0);
        NSFX_TEST_EXPECT_EQ(v2.GetType(), nsfx::VT_UINT16);
        NSFX_TEST_EXPECT_EQ(v2.GetValue<uint16_t>(), u0);

        NSFX_TEST_EXPECT(v0 == v0);
        NSFX_TEST_EXPECT(v0 != v1);
        NSFX_TEST_EXPECT(v0 == v2);
    }

    NSFX_TEST_CASE(int32_t)
    {
        int32_t i0 = 0xfedcba98;
        nsfx::Variant  v0(i0);
        NSFX_TEST_EXPECT_EQ(v0.GetType(), nsfx::VT_INT32);
        NSFX_TEST_EXPECT_EQ(v0.GetValue<int32_t>(), i0);

        int32_t i1 = 0x89abcdef;
        nsfx::Variant  v1(i1);
        nsfx::Variant  v2(v0);
        NSFX_TEST_EXPECT_EQ(v2.GetValue<int32_t>(), i0);
        NSFX_TEST_EXPECT_EQ(v2.GetValue<int32_t>(), i0);

        NSFX_TEST_EXPECT(v0 == v0);
        NSFX_TEST_EXPECT(v0 != v1);
        NSFX_TEST_EXPECT(v0 == v2);
    }

    NSFX_TEST_CASE(uint32_t)
    {
        uint32_t u0 = 0xfedcba98;
        nsfx::Variant  v0(u0);
        NSFX_TEST_EXPECT_EQ(v0.GetType(), nsfx::VT_UINT32);
        NSFX_TEST_EXPECT_EQ(v0.GetValue<uint32_t>(), u0);

        uint32_t u1 = 0x89abcdef;
        nsfx::Variant  v1(u1);
        nsfx::Variant  v2(v0);
        NSFX_TEST_EXPECT_EQ(v2.GetValue<uint32_t>(), u0);
        NSFX_TEST_EXPECT_EQ(v2.GetValue<uint32_t>(), u0);

        NSFX_TEST_EXPECT(v0 == v0);
        NSFX_TEST_EXPECT(v0 != v1);
        NSFX_TEST_EXPECT(v0 == v2);
    }

    NSFX_TEST_CASE(int64_t)
    {
        int64_t i0 = 0xfedcba9876543210LL;
        nsfx::Variant  v0(i0);
        NSFX_TEST_EXPECT_EQ(v0.GetType(), nsfx::VT_INT64);
        NSFX_TEST_EXPECT_EQ(v0.GetValue<int64_t>(), i0);

        int64_t i1 = 0x0123456789abcdefLL;
        nsfx::Variant  v1(i1);
        nsfx::Variant  v2(v0);
        NSFX_TEST_EXPECT_EQ(v2.GetType(), nsfx::VT_INT64);
        NSFX_TEST_EXPECT_EQ(v2.GetValue<int64_t>(), i0);

        NSFX_TEST_EXPECT(v0 == v0);
        NSFX_TEST_EXPECT(v0 != v1);
        NSFX_TEST_EXPECT(v0 == v2);
    }

    NSFX_TEST_CASE(uint64_t)
    {
        uint64_t u0 = 0xfedcba9876543210ULL;
        nsfx::Variant  v0(u0);
        NSFX_TEST_EXPECT_EQ(v0.GetType(), nsfx::VT_UINT64);
        NSFX_TEST_EXPECT_EQ(v0.GetValue<uint64_t>(), u0);

        uint64_t u1 = 0x0123456789abcdefULL;
        nsfx::Variant  v1(u1);
        nsfx::Variant  v2(v0);
        NSFX_TEST_EXPECT_EQ(v2.GetType(), nsfx::VT_UINT64);
        NSFX_TEST_EXPECT_EQ(v2.GetValue<uint64_t>(), u0);

        NSFX_TEST_EXPECT(v0 == v0);
        NSFX_TEST_EXPECT(v0 != v1);
        NSFX_TEST_EXPECT(v0 == v2);
    }

    NSFX_TEST_CASE(float)
    {
        float f0 = 5.4321F;
        nsfx::Variant  v0(f0);
        NSFX_TEST_EXPECT_EQ(v0.GetType(), nsfx::VT_FLOAT);
        NSFX_TEST_EXPECT_EQ(v0.GetValue<float>(), f0);

        float f1 = 1.2345F;
        nsfx::Variant  v1(f1);
        nsfx::Variant  v2(v0);
        NSFX_TEST_EXPECT_EQ(v2.GetType(), nsfx::VT_FLOAT);
        NSFX_TEST_EXPECT_EQ(v2.GetValue<float>(), f0);

        NSFX_TEST_EXPECT(v0 == v0);
        NSFX_TEST_EXPECT(v0 != v1);
        NSFX_TEST_EXPECT(v0 == v2);
    }

    NSFX_TEST_CASE(double)
    {
        double d0 = 9.87654321;
        nsfx::Variant  v0(d0);
        NSFX_TEST_EXPECT_EQ(v0.GetType(), nsfx::VT_DOUBLE);
        NSFX_TEST_EXPECT_EQ(v0.GetValue<double>(), d0);

        double d1 = 1.23456789;
        nsfx::Variant  v1(d1);
        nsfx::Variant  v2(v0);
        NSFX_TEST_EXPECT_EQ(v2.GetType(), nsfx::VT_DOUBLE);
        NSFX_TEST_EXPECT_EQ(v2.GetValue<double>(), d0);

        NSFX_TEST_EXPECT(v0 == v0);
        NSFX_TEST_EXPECT(v0 != v1);
        NSFX_TEST_EXPECT(v0 == v2);
    }

    NSFX_TEST_CASE(Duration)
    {
        Duration d0(0xfedcba9876543210LL);
        nsfx::Variant  v0(d0);
        NSFX_TEST_EXPECT_EQ(v0.GetType(), nsfx::VT_DURATION);
        NSFX_TEST_EXPECT_EQ(v0.GetValue<Duration>(), d0);

        Duration d1(0x0123456789abcdefLL);
        nsfx::Variant  v1(d1);
        nsfx::Variant  v2(v0);
        NSFX_TEST_EXPECT_EQ(v2.GetType(), nsfx::VT_DURATION);
        NSFX_TEST_EXPECT_EQ(v2.GetValue<Duration>(), d0);

        NSFX_TEST_EXPECT(v0 == v0);
        NSFX_TEST_EXPECT(v0 != v1);
        NSFX_TEST_EXPECT(v0 == v2);
    }

    NSFX_TEST_CASE(TimePoint)
    {
        TimePoint t0(Duration(0xfedcba9876543210LL));
        nsfx::Variant  v0(t0);
        NSFX_TEST_EXPECT_EQ(v0.GetType(), nsfx::VT_TIME_POINT);
        NSFX_TEST_EXPECT_EQ(v0.GetValue<TimePoint>(), t0);

        TimePoint t1(Duration(0x0123456789abcdefLL));
        nsfx::Variant  v1(t1);
        nsfx::Variant  v2(v0);
        NSFX_TEST_EXPECT_EQ(v2.GetType(), nsfx::VT_TIME_POINT);
        NSFX_TEST_EXPECT_EQ(v2.GetValue<TimePoint>(), t0);

        NSFX_TEST_EXPECT(v0 == v0);
        NSFX_TEST_EXPECT(v0 != v1);
        NSFX_TEST_EXPECT(v0 == v2);
    }

    NSFX_TEST_CASE(Cstring)
    {
        const char* s0 = "Test";
        nsfx::Variant  v0(s0);
        NSFX_TEST_EXPECT_EQ(v0.GetType(), nsfx::VT_STRING);
        NSFX_TEST_EXPECT(!strcmp(v0.GetValue<const char*>(), s0));

        const char* s1 = "tseT";
        nsfx::Variant  v1(s1);
        nsfx::Variant  v2(v0);
        NSFX_TEST_EXPECT_EQ(v2.GetType(), nsfx::VT_STRING);
        NSFX_TEST_EXPECT(!strcmp(v2.GetValue<const char*>(), s0));

        NSFX_TEST_EXPECT(v0 == v0);
        NSFX_TEST_EXPECT(v0 != v1);
        NSFX_TEST_EXPECT(v0 == v2);
    }

    NSFX_TEST_CASE(string)
    {
        std::string s("Test");
        nsfx::Variant  v0(s);
        NSFX_TEST_EXPECT_EQ(v0.GetType(), nsfx::VT_STRING);
        NSFX_TEST_EXPECT(v0.GetValue<std::string>() == s);

        std::string s1("tseT");
        nsfx::Variant  v1(s1);
        nsfx::Variant  v2(v0);
        NSFX_TEST_EXPECT_EQ(v2.GetType(), nsfx::VT_STRING);
        NSFX_TEST_EXPECT(v2.GetValue<std::string>() == s);

        NSFX_TEST_EXPECT(v0 == v0);
        NSFX_TEST_EXPECT(v0 != v1);
        NSFX_TEST_EXPECT(v0 == v2);
    }

    // TODO: IObject*

    // NSFX_TEST_CASE(byref_bool)
    // {
    //     bool b0 = true;
    //     nsfx::Variant  r0(&b0);
    //     NSFX_TEST_EXPECT_EQ(r0.GetType(), nsfx::VT_BOOL | nsfx::VT_BYREF);
    //     // NSFX_TEST_EXPECT_EQ(r0.GetValue<bool*>(), &b0);
    //
    //     bool b1 = false;
    //     nsfx::Variant  r1(&b1);
    //     nsfx::Variant  r2(r0);
    //     NSFX_TEST_EXPECT_EQ(r2.GetType(), nsfx::VT_BOOL | nsfx::VT_BYREF);
    //     // NSFX_TEST_EXPECT_EQ(r2.GetValue<bool*>(), &b0);
    //
    //     NSFX_TEST_EXPECT(r0 == r0);
    //     NSFX_TEST_EXPECT(r0 != r1);
    //     NSFX_TEST_EXPECT(r0 == r2);
    //
    //     r0.CarryValue<bool>(b1);
    //     NSFX_TEST_EXPECT_EQ(b0, b1);
    // }
    //
    // NSFX_TEST_CASE(byref_int8_t)
    // {
    //     int8_t i0 = 0xfe;
    //     nsfx::Variant  r0(&i0);
    //     NSFX_TEST_EXPECT_EQ(r0.GetType(), nsfx::VT_INT8 | nsfx::VT_BYREF);
    //     // NSFX_TEST_EXPECT_EQ(r0.GetValue<int8_t*>(), &i0);
    //
    //     int8_t i1 = 0xef;
    //     nsfx::Variant  r1(&i1);
    //     nsfx::Variant  r2(r0);
    //     NSFX_TEST_EXPECT_EQ(r2.GetType(), nsfx::VT_INT8 | nsfx::VT_BYREF);
    //     // NSFX_TEST_EXPECT_EQ(r2.GetValue<int8_t*>(), &i0);
    //
    //     NSFX_TEST_EXPECT(r0 == r0);
    //     NSFX_TEST_EXPECT(r0 != r1);
    //     NSFX_TEST_EXPECT(r0 == r2);
    //
    //     r0.CarryValue<int8_t>(i1);
    //     NSFX_TEST_EXPECT_EQ(i0, i1);
    // }
    //
    // NSFX_TEST_CASE(byref_uint8_t)
    // {
    //     uint8_t u0 = 0xfe;
    //     nsfx::Variant  r0(&u0);
    //     NSFX_TEST_EXPECT_EQ(r0.GetType(), nsfx::VT_UINT8 | nsfx::VT_BYREF);
    //     // NSFX_TEST_EXPECT_EQ(r0.GetValue<uint8_t*>(), &u0);
    //
    //     uint8_t u1 = 0xef;
    //     nsfx::Variant  r1(&u1);
    //     nsfx::Variant  r2(r0);
    //     NSFX_TEST_EXPECT_EQ(r2.GetType(), nsfx::VT_UINT8 | nsfx::VT_BYREF);
    //     // NSFX_TEST_EXPECT_EQ(r2.GetValue<uint8_t*>(), &u0);
    //
    //     NSFX_TEST_EXPECT(r0 == r0);
    //     NSFX_TEST_EXPECT(r0 != r1);
    //     NSFX_TEST_EXPECT(r0 == r2);
    //
    //     r0.CarryValue<uint8_t>(u1);
    //     NSFX_TEST_EXPECT_EQ(u0, u1);
    // }
    //
    // NSFX_TEST_CASE(byref_int16_t)
    // {
    //     int16_t i0 = 0xfedc;
    //     nsfx::Variant  r0(&i0);
    //     NSFX_TEST_EXPECT_EQ(r0.GetType(), nsfx::VT_INT16 | nsfx::VT_BYREF);
    //     // NSFX_TEST_EXPECT_EQ(r0.GetValue<int16_t*>(), &i0);
    //
    //     int16_t i1 = 0xcdef;
    //     nsfx::Variant  r1(&i1);
    //     nsfx::Variant  r2(r0);
    //     NSFX_TEST_EXPECT_EQ(r2.GetType(), nsfx::VT_INT16 | nsfx::VT_BYREF);
    //     // NSFX_TEST_EXPECT_EQ(r2.GetValue<int16_t*>(), &i0);
    //
    //     NSFX_TEST_EXPECT(r0 == r0);
    //     NSFX_TEST_EXPECT(r0 != r1);
    //     NSFX_TEST_EXPECT(r0 == r2);
    //
    //     r0.CarryValue<int16_t>(i1);
    //     NSFX_TEST_EXPECT_EQ(i0, i1);
    // }
    //
    // NSFX_TEST_CASE(byref_uint16_t)
    // {
    //     uint16_t u0 = 0xfedc;
    //     nsfx::Variant  r0(&u0);
    //     NSFX_TEST_EXPECT_EQ(r0.GetType(), nsfx::VT_UINT16 | nsfx::VT_BYREF);
    //     // NSFX_TEST_EXPECT_EQ(r0.GetValue<uint16_t*>(), &u0);
    //
    //     uint16_t u1 = 0xcdef;
    //     nsfx::Variant  r1(&u1);
    //     nsfx::Variant  r2(r0);
    //     NSFX_TEST_EXPECT_EQ(r2.GetType(), nsfx::VT_UINT16 | nsfx::VT_BYREF);
    //     // NSFX_TEST_EXPECT_EQ(r2.GetValue<uint16_t*>(), &u0);
    //
    //     NSFX_TEST_EXPECT(r0 == r0);
    //     NSFX_TEST_EXPECT(r0 != r1);
    //     NSFX_TEST_EXPECT(r0 == r2);
    //
    //     r0.CarryValue<uint16_t>(u1);
    //     NSFX_TEST_EXPECT_EQ(u0, u1);
    // }
    //
    // NSFX_TEST_CASE(byref_int32_t)
    // {
    //     int32_t i0 = 0xfedcba98;
    //     nsfx::Variant  r0(&i0);
    //     NSFX_TEST_EXPECT_EQ(r0.GetType(), nsfx::VT_INT32 | nsfx::VT_BYREF);
    //     // NSFX_TEST_EXPECT_EQ(r0.GetValue<int32_t*>(), &i0);
    //
    //     int32_t i1 = 0x89abcdef;
    //     nsfx::Variant  r1(&i1);
    //     nsfx::Variant  r2(r0);
    //     NSFX_TEST_EXPECT_EQ(r2.GetType(), nsfx::VT_INT32 | nsfx::VT_BYREF);
    //     // NSFX_TEST_EXPECT_EQ(r2.GetValue<int32_t*>(), &i0);
    //
    //     NSFX_TEST_EXPECT(r0 == r0);
    //     NSFX_TEST_EXPECT(r0 != r1);
    //     NSFX_TEST_EXPECT(r0 == r2);
    //
    //     r0.CarryValue<int32_t>(i1);
    //     NSFX_TEST_EXPECT_EQ(i0, i1);
    // }
    //
    // NSFX_TEST_CASE(byref_uint32_t)
    // {
    //     uint32_t u0 = 0xfedcba98;
    //     nsfx::Variant  r0(&u0);
    //     NSFX_TEST_EXPECT_EQ(r0.GetType(), nsfx::VT_UINT32 | nsfx::VT_BYREF);
    //     // NSFX_TEST_EXPECT_EQ(r0.GetValue<uint32_t*>(), &u0);
    //
    //     uint32_t u1 = 0x89abcdef;
    //     nsfx::Variant  r1(&u1);
    //     nsfx::Variant  r2(r0);
    //     NSFX_TEST_EXPECT_EQ(r2.GetType(), nsfx::VT_UINT32 | nsfx::VT_BYREF);
    //     // NSFX_TEST_EXPECT_EQ(r2.GetValue<uint32_t*>(), &u0);
    //
    //     NSFX_TEST_EXPECT(r0 == r0);
    //     NSFX_TEST_EXPECT(r0 != r1);
    //     NSFX_TEST_EXPECT(r0 == r2);
    //
    //     r0.CarryValue<uint32_t>(u1);
    //     NSFX_TEST_EXPECT_EQ(u0, u1);
    // }
    //
    // NSFX_TEST_CASE(byref_int64_t)
    // {
    //     int64_t i0 = 0xfedcba9876543210LL;
    //     nsfx::Variant  r0(&i0);
    //     NSFX_TEST_EXPECT_EQ(r0.GetType(), nsfx::VT_INT64 | nsfx::VT_BYREF);
    //     // NSFX_TEST_EXPECT_EQ(r0.GetValue<int64_t*>(), &i0);
    //
    //     int64_t i1 = 0x0123456789abcdefLL;
    //     nsfx::Variant  r1(&i1);
    //     nsfx::Variant  r2(r0);
    //     NSFX_TEST_EXPECT_EQ(r2.GetType(), nsfx::VT_INT64 | nsfx::VT_BYREF);
    //     // NSFX_TEST_EXPECT_EQ(r2.GetValue<int64_t*>(), &i0);
    //
    //     NSFX_TEST_EXPECT(r0 == r0);
    //     NSFX_TEST_EXPECT(r0 != r1);
    //     NSFX_TEST_EXPECT(r0 == r2);
    //
    //     r0.CarryValue<int64_t>(i1);
    //     NSFX_TEST_EXPECT_EQ(i0, i1);
    // }
    //
    // NSFX_TEST_CASE(byref_uint64_t)
    // {
    //     uint64_t u0 = 0xfedcba9876543210ULL;
    //     nsfx::Variant  r0(&u0);
    //     NSFX_TEST_EXPECT_EQ(r0.GetType(), nsfx::VT_UINT64 | nsfx::VT_BYREF);
    //     // NSFX_TEST_EXPECT_EQ(r0.GetValue<uint64_t*>(), &u0);
    //
    //     uint64_t u1 = 0x0123456789abcdefLL;
    //     nsfx::Variant  r1(&u1);
    //     nsfx::Variant  r2(r0);
    //     NSFX_TEST_EXPECT_EQ(r2.GetType(), nsfx::VT_UINT64 | nsfx::VT_BYREF);
    //     // NSFX_TEST_EXPECT_EQ(r2.GetValue<uint64_t*>(), &u0);
    //
    //     NSFX_TEST_EXPECT(r0 == r0);
    //     NSFX_TEST_EXPECT(r0 != r1);
    //     NSFX_TEST_EXPECT(r0 == r2);
    //
    //     r0.CarryValue<uint64_t>(u1);
    //     NSFX_TEST_EXPECT_EQ(u0, u1);
    // }
    //
    // NSFX_TEST_CASE(byref_float)
    // {
    //     float f0 = 5.4321F;
    //     nsfx::Variant  r0(&f0);
    //     NSFX_TEST_EXPECT_EQ(r0.GetType(), nsfx::VT_FLOAT | nsfx::VT_BYREF);
    //     // NSFX_TEST_EXPECT_EQ(r0.GetValue<float*>(), &f0);
    //
    //     float f1 = 5.4321F;
    //     nsfx::Variant  r1(&f1);
    //     nsfx::Variant  r2(r0);
    //     NSFX_TEST_EXPECT_EQ(r2.GetType(), nsfx::VT_FLOAT | nsfx::VT_BYREF);
    //     // NSFX_TEST_EXPECT_EQ(r2.GetValue<float*>(), &f0);
    //
    //     NSFX_TEST_EXPECT(r0 == r0);
    //     NSFX_TEST_EXPECT(r0 != r1);
    //     NSFX_TEST_EXPECT(r0 == r2);
    //
    //     r0.CarryValue<float>(f1);
    //     NSFX_TEST_EXPECT_EQ(f0, f1);
    // }
    //
    // NSFX_TEST_CASE(byref_double)
    // {
    //     double d0 = 9.87654321;
    //     nsfx::Variant  r0(&d0);
    //     NSFX_TEST_EXPECT_EQ(r0.GetType(), nsfx::VT_DOUBLE | nsfx::VT_BYREF);
    //     // NSFX_TEST_EXPECT_EQ(r0.GetValue<double*>(), &d0);
    //
    //     double d1 = 9.87654321;
    //     nsfx::Variant  r1(&d1);
    //     nsfx::Variant  r2(r0);
    //     NSFX_TEST_EXPECT_EQ(r2.GetType(), nsfx::VT_DOUBLE | nsfx::VT_BYREF);
    //     // NSFX_TEST_EXPECT_EQ(r2.GetValue<double*>(), &d0);
    //
    //     NSFX_TEST_EXPECT(r0 == r0);
    //     NSFX_TEST_EXPECT(r0 != r1);
    //     NSFX_TEST_EXPECT(r0 == r2);
    //
    //     r0.CarryValue<double>(d1);
    //     NSFX_TEST_EXPECT_EQ(d0, d1);
    // }
    //
    // NSFX_TEST_CASE(byref_Cstring)
    // {
    //     const char* s0 = "Test";
    //     nsfx::Variant  r0(&s0);
    //     NSFX_TEST_EXPECT_EQ(r0.GetType(), nsfx::VT_STRING | nsfx::VT_BYREF);
    //     // NSFX_TEST_EXPECT_EQ(r0.GetValue<const char**>(), &s0);
    //
    //     const char* s1 = "tseT";
    //     nsfx::Variant  r1(&s1);
    //     nsfx::Variant  r2(r0);
    //     NSFX_TEST_EXPECT_EQ(r2.GetType(), nsfx::VT_STRING | nsfx::VT_BYREF);
    //     // NSFX_TEST_EXPECT_EQ(r2.GetValue<const char**>(), &s0);
    //
    //     NSFX_TEST_EXPECT(r0 == r0);
    //     NSFX_TEST_EXPECT(r0 != r1);
    //     NSFX_TEST_EXPECT(r0 == r2);
    //
    //     r0.CarryValue<const char*>(s1);
    //     NSFX_TEST_EXPECT(!strcmp(s0, s1));
    // }
    //
    // NSFX_TEST_CASE(byref_string)
    // {
    //     std::string s0("Test");
    //     nsfx::Variant  v0(s0);
    //     nsfx::Variant  r0 = v0.MakeRef();
    //     NSFX_TEST_EXPECT_EQ(r0.GetType(), nsfx::VT_STRING | nsfx::VT_BYREF);
    //     // NSFX_TEST_EXPECT(*r0.GetValue<std::string*>() == s0);
    //
    //     std::string s1("tseT");
    //     nsfx::Variant  v1(s1);
    //     nsfx::Variant  r1 = v1.MakeRef();
    //     nsfx::Variant  r2(r0);
    //     NSFX_TEST_EXPECT_EQ(r2.GetType(), nsfx::VT_STRING | nsfx::VT_BYREF);
    //     // NSFX_TEST_EXPECT(*r2.GetValue<std::string*>() == s0);
    //
    //     NSFX_TEST_EXPECT(r0 == r0);
    //     NSFX_TEST_EXPECT(r0 != r1);
    //     NSFX_TEST_EXPECT(r0 == r2);
    //
    //     r0.CarryValue<std::string>(s1);
    //     NSFX_TEST_EXPECT(v0.GetValue<std::string>() == s1);
    // }
    //
    // NSFX_TEST_CASE(byref_Duration)
    // {
    //     Duration d0(0xfedcba9876543210LL);
    //     nsfx::Variant  v0(d0);
    //     nsfx::Variant  r0 = v0.MakeRef();
    //     NSFX_TEST_EXPECT_EQ(r0.GetType(), nsfx::VT_DURATION | nsfx::VT_BYREF);
    //     // NSFX_TEST_EXPECT_EQ(*r0.GetValue<Duration*>(), d0.GetCount());
    //
    //     Duration d1(0x0123456789abcdefLL);
    //     nsfx::Variant  v1(d1);
    //     nsfx::Variant  r1 = v1.MakeRef();
    //     nsfx::Variant  r2(r0);
    //     NSFX_TEST_EXPECT_EQ(r2.GetType(), nsfx::VT_DURATION | nsfx::VT_BYREF);
    //     // NSFX_TEST_EXPECT_EQ(*r2.GetValue<Duration*>(), d0.GetCount());
    //
    //     NSFX_TEST_EXPECT(r0 == r0);
    //     NSFX_TEST_EXPECT(r0 != r1);
    //     NSFX_TEST_EXPECT(r0 == r2);
    //
    //     r0.CarryValue<Duration>(d1);
    //     NSFX_TEST_EXPECT_EQ(v0.GetValue<Duration>(), d1);
    // }
    //
    // NSFX_TEST_CASE(byref_TimePoint)
    // {
    //     TimePoint t0(Duration(0xfedcba9876543210LL));
    //     nsfx::Variant  v0(t0);
    //     nsfx::Variant  r0 = v0.MakeRef();
    //     NSFX_TEST_EXPECT_EQ(r0.GetType(), nsfx::VT_TIME_POINT | nsfx::VT_BYREF);
    //     // NSFX_TEST_EXPECT_EQ(*r0.GetValue<TimePoint*>(),
    //     //                     t0.GetDuration().GetCount());
    //
    //     TimePoint t1(Duration(0x0123456789abcdefLL));
    //     nsfx::Variant  v1(t1);
    //     nsfx::Variant  r1 = v1.MakeRef();
    //     nsfx::Variant  r2(r0);
    //     NSFX_TEST_EXPECT_EQ(r2.GetType(), nsfx::VT_TIME_POINT | nsfx::VT_BYREF);
    //     // NSFX_TEST_EXPECT_EQ(*r2.GetValue<TimePoint*>(),
    //     //                     t0.GetDuration().GetCount());
    //
    //     NSFX_TEST_EXPECT(r0 == r0);
    //     NSFX_TEST_EXPECT(r0 != r1);
    //     NSFX_TEST_EXPECT(r0 == r2);
    //
    //     r0.CarryValue<TimePoint>(t1);
    //     NSFX_TEST_EXPECT_EQ(v0.GetValue<TimePoint>(), t1);
    // }

    // TODO: IObject*

    NSFX_TEST_CASE(Swap)
    {
        Duration d(0xfedcba9876543210LL);
        nsfx::Variant  v0(d);

        uint64_t u = 0xfedcba9876543210ULL;
        nsfx::Variant  v1(u);

        boost::swap(v0, v1);

        NSFX_TEST_EXPECT_EQ(v0.GetType(), nsfx::VT_UINT64);
        NSFX_TEST_EXPECT_EQ(v0.GetValue<uint64_t>(), u);

        NSFX_TEST_EXPECT_EQ(v1.GetType(), nsfx::VT_DURATION);
        NSFX_TEST_EXPECT_EQ(v1.GetValue<Duration>(), d);
    }
}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

