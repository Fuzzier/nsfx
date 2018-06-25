/**
 * @file
 *
 * @brief Test Interoperability.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-06-25
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/interop/invoke/invoker.h>
#include <nsfx/chrono/virtual-time-point.h>
#include <iostream>


NSFX_TEST_SUITE(Invoker)
{
    typedef nsfx::chrono::VirtualTimePoint  TimePoint;
    typedef TimePoint::Duration             Duration;

    static const uint32_t  rr = 0xab;

    static const int64_t   ii = 0xf7;
    static const double    ij = 1.23456;
    static const double    oj = 6.54321;
    static const Duration  ix(12345);
    static const TimePoint iy(Duration(67890));
    static const TimePoint oy(Duration(54321));

    struct Test
    {
        void V0(void)
        {
        }

        void V1(int64_t i)
        {
            NSFX_TEST_EXPECT_EQ(i, ii);
        }

        void V2(int64_t i, double* j)
        {
            NSFX_TEST_EXPECT_EQ(i, ii);
            NSFX_TEST_EXPECT_EQ(*j, ij);
            *j = oj;
        }

        void V3(int64_t i, double* j, Duration x)
        {
            NSFX_TEST_EXPECT_EQ(i, ii);
            NSFX_TEST_EXPECT_EQ(*j, ij);
            NSFX_TEST_EXPECT_EQ(x, ix);
            *j = oj;
        }

        void V4(int64_t i, double* j, Duration x, TimePoint* y)
        {
            NSFX_TEST_EXPECT_EQ(i, ii);
            NSFX_TEST_EXPECT_EQ(*j, ij);
            NSFX_TEST_EXPECT_EQ(x, ix);
            NSFX_TEST_EXPECT_EQ(*y, iy);
            *j = oj;
            *y = oy;
        }

        uint32_t R0(void)
        {
            return rr;
        }

        uint32_t R1(int64_t i)
        {
            return rr;
        }

        uint32_t R2(int64_t i, double* j)
        {
            *j = oj;
            return rr;
        }

        uint32_t R3(int64_t i, double* j, Duration x)
        {
            *j = oj;
            return rr;
        }

        uint32_t R4(int64_t i, double* j, Duration x, TimePoint* y)
        {
            *j = oj;
            *y = oy;
            return rr;
        }

    };

    NSFX_TEST_SUITE(V)
    {
        NSFX_TEST_CASE(0)
        {
            Test t;
            nsfx::Variant args[4] = { ii, ij, ix, iy };
            nsfx::aux::Invoker  m;
            nsfx::Variant ret;
            m.Invoke(&t, &Test::V0, nullptr, 0, nullptr);
            m.Invoke(&t, &Test::V0, &ret, 0, nullptr);
            NSFX_TEST_EXPECT_EQ(ret.GetType(), nsfx::VT_EMPTY);
        }

        NSFX_TEST_CASE(1)
        {
            Test t;
            nsfx::Variant args[4] = { ii, ij, ix, iy };
            nsfx::aux::Invoker  m;
            nsfx::Variant ret;
            m.Invoke(&t, &Test::V1, nullptr, 1, args);
        }

        NSFX_TEST_CASE(2)
        {
            Test t;
            nsfx::Variant args[4] = { ii, ij, ix, iy };
            nsfx::aux::Invoker  m;
            nsfx::Variant ret;
            m.Invoke(&t, &Test::V2, nullptr, 2, args);
            NSFX_TEST_EXPECT_EQ(args[1].GetValue<double>(), oj);
        }

        NSFX_TEST_CASE(3)
        {
            Test t;
            nsfx::Variant args[4] = { ii, ij, ix, iy };
            nsfx::aux::Invoker  m;
            nsfx::Variant ret;
            m.Invoke(&t, &Test::V3, nullptr, 3, args);
            NSFX_TEST_EXPECT_EQ(args[1].GetValue<double>(), oj);
        }

        NSFX_TEST_CASE(4)
        {
            Test t;
            nsfx::Variant args[4] = { ii, ij, ix, iy };
            nsfx::aux::Invoker  m;
            nsfx::Variant ret;
            m.Invoke(&t, &Test::V4, nullptr, 4, args);
            NSFX_TEST_EXPECT_EQ(args[1].GetValue<double>(), oj);
            NSFX_TEST_EXPECT_EQ(args[3].GetValue<TimePoint>(), oy);
        }
    }

    NSFX_TEST_SUITE(R)
    {
        NSFX_TEST_CASE(r0)
        {
            Test t;
            nsfx::Variant args[4] = { ii, ij, ix, iy };
            nsfx::aux::Invoker  m;
            nsfx::Variant ret;
            m.Invoke(&t, &Test::R0, &ret, 0, nullptr);
            NSFX_TEST_EXPECT_EQ(ret.GetValue<uint32_t>(), rr);
        }

        NSFX_TEST_CASE(r1)
        {
            Test t;
            nsfx::Variant args[4] = { ii, ij, ix, iy };
            nsfx::aux::Invoker  m;
            nsfx::Variant ret;
            m.Invoke(&t, &Test::R1, &ret, 1, args);
            NSFX_TEST_EXPECT_EQ(ret.GetValue<uint32_t>(), rr);
        }

        NSFX_TEST_CASE(r2)
        {
            Test t;
            nsfx::Variant args[4] = { ii, ij, ix, iy };
            nsfx::aux::Invoker  m;
            nsfx::Variant ret;
            m.Invoke(&t, &Test::R2, &ret, 2, args);
            NSFX_TEST_EXPECT_EQ(args[1].GetValue<double>(), oj);
            NSFX_TEST_EXPECT_EQ(ret.GetValue<uint32_t>(), rr);
        }

        NSFX_TEST_CASE(r3)
        {
            Test t;
            nsfx::Variant args[4] = { ii, ij, ix, iy };
            nsfx::aux::Invoker  m;
            nsfx::Variant ret;
            m.Invoke(&t, &Test::R3, &ret, 3, args);
            NSFX_TEST_EXPECT_EQ(args[1].GetValue<double>(), oj);
            NSFX_TEST_EXPECT_EQ(ret.GetValue<uint32_t>(), rr);
        }

        NSFX_TEST_CASE(r4)
        {
            Test t;
            nsfx::Variant args[4] = { ii, ij, ix, iy };
            nsfx::aux::Invoker  m;
            nsfx::Variant ret;
            m.Invoke(&t, &Test::R4, &ret, 4, args);
            NSFX_TEST_EXPECT_EQ(args[1].GetValue<double>(), oj);
            NSFX_TEST_EXPECT_EQ(args[3].GetValue<TimePoint>(), oy);
            NSFX_TEST_EXPECT_EQ(ret.GetValue<uint32_t>(), rr);
        }

        NSFX_TEST_CASE(v0)
        {
            Test t;
            nsfx::Variant args[4] = { ii, ij, ix, iy };
            nsfx::aux::Invoker  m;
            nsfx::Variant ret;
            m.Invoke(&t, &Test::R0, nullptr, 0, nullptr);
        }

        NSFX_TEST_CASE(v1)
        {
            Test t;
            nsfx::Variant args[4] = { ii, ij, ix, iy };
            nsfx::aux::Invoker  m;
            nsfx::Variant ret;
            m.Invoke(&t, &Test::R1, nullptr, 1, args);
        }

        NSFX_TEST_CASE(v2)
        {
            Test t;
            nsfx::Variant args[4] = { ii, ij, ix, iy };
            nsfx::aux::Invoker  m;
            nsfx::Variant ret;
            m.Invoke(&t, &Test::R2, nullptr, 2, args);
            NSFX_TEST_EXPECT_EQ(args[1].GetValue<double>(), oj);
        }

        NSFX_TEST_CASE(v3)
        {
            Test t;
            nsfx::Variant args[4] = { ii, ij, ix, iy };
            nsfx::aux::Invoker  m;
            nsfx::Variant ret;
            m.Invoke(&t, &Test::R3, nullptr, 3, args);
            NSFX_TEST_EXPECT_EQ(args[1].GetValue<double>(), oj);
        }

        NSFX_TEST_CASE(v4)
        {
            Test t;
            nsfx::Variant args[4] = { ii, ij, ix, iy };
            nsfx::aux::Invoker  m;
            nsfx::Variant ret;
            m.Invoke(&t, &Test::R4, &ret, 4, args);
            NSFX_TEST_EXPECT_EQ(args[1].GetValue<double>(), oj);
            NSFX_TEST_EXPECT_EQ(args[3].GetValue<TimePoint>(), oy);
        }

        NSFX_TEST_CASE(a0)
        {
            Test t;
            nsfx::Variant args[4];
            args[0] = ii;
            args[2] = ix;
            nsfx::aux::Invoker  m;
            nsfx::Variant ret;
            m.Invoke(&t, &Test::R0, &ret, 0, nullptr);
        }

        NSFX_TEST_CASE(a1)
        {
            Test t;
            nsfx::Variant args[4];
            args[0] = ii;
            args[2] = ix;
            nsfx::aux::Invoker  m;
            nsfx::Variant ret;
            m.Invoke(&t, &Test::R1, &ret, 1, args);
        }

        NSFX_TEST_CASE(a2)
        {
            Test t;
            nsfx::Variant args[4];
            args[0] = ii;
            args[2] = ix;
            nsfx::aux::Invoker  m;
            nsfx::Variant ret;
            m.Invoke(&t, &Test::R2, &ret, 2, args);
            NSFX_TEST_EXPECT_EQ(args[1].GetValue<double>(), oj);
        }

        NSFX_TEST_CASE(a3)
        {
            Test t;
            nsfx::Variant args[4];
            args[0] = ii;
            args[2] = ix;
            nsfx::aux::Invoker  m;
            nsfx::Variant ret;
            m.Invoke(&t, &Test::R3, &ret, 3, args);
            NSFX_TEST_EXPECT_EQ(args[1].GetValue<double>(), oj);
        }

        NSFX_TEST_CASE(a4)
        {
            Test t;
            nsfx::Variant args[4];
            args[0] = ii;
            args[2] = ix;
            nsfx::aux::Invoker  m;
            nsfx::Variant ret;
            m.Invoke(&t, &Test::R4, &ret, 4, args);
            NSFX_TEST_EXPECT_EQ(args[1].GetValue<double>(), oj);
            NSFX_TEST_EXPECT_EQ(args[3].GetValue<TimePoint>(), oy);
        }
    }

}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cout);
    nsfx::test::runner::Run();

    return 0;
}

