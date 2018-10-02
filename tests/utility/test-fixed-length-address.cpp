/**
 * @file
 *
 * @brief Test FixedLengthAddress.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-06-10
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/utility/fixed-length-address.h>
#include <iostream>


NSFX_TEST_SUITE(FixedLengthAddress)
{
    NSFX_TEST_SUITE(4)/*{{{*/
    {
        typedef nsfx::FixedLengthAddress<4>  Address;

        NSFX_TEST_CASE(Ctor0)
        {
            Address a(0);
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0);
        }

        NSFX_TEST_CASE(Ctor1)
        {
            Address a(0xff);
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0x0f);
        }

        NSFX_TEST_CASE(Increment)
        {
            Address a(0x0f);
            ++a;
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0);
            --a;
            NSFX_TEST_EXPECT_EQ(a++.GetValue(), 0x0f);
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0);
        }

        NSFX_TEST_CASE(Decrement)
        {
            Address a(0);
            --a;
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0x0f);
            ++a;
            NSFX_TEST_EXPECT_EQ(a--.GetValue(), 0);
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0x0f);
        }

        NSFX_TEST_CASE(Algorithm)
        {
            Address a;
            Address b;

            // +=
            a = Address(0);
            a += 0x0f;
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0x0f);

            a = Address(0);
            a += -0x0f;
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0x01);

            // -=
            a = Address(0);
            a -= 0x01;
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0x0f);

            a = Address(0);
            a -= -0x01;
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0x01);

            // +
            a = Address(0);
            b = a + 0x0f;
            NSFX_TEST_EXPECT_EQ(b.GetValue(), 0x0f);

            b = 0x0f + a;
            NSFX_TEST_EXPECT_EQ(b.GetValue(), 0x0f);

            // -
            b = a - 0x01;
            NSFX_TEST_EXPECT_EQ(b.GetValue(), 0x0f);

            NSFX_TEST_EXPECT_EQ(b - a,  0x0f);
            NSFX_TEST_EXPECT_EQ(a - b, -0x0f);

            //
            NSFX_TEST_EXPECT(a == a);
            NSFX_TEST_EXPECT(!(a != a));

            NSFX_TEST_EXPECT(a != b);
            NSFX_TEST_EXPECT(!(a == b));

            NSFX_TEST_EXPECT(a <= a);
            NSFX_TEST_EXPECT(!(a > a));

            NSFX_TEST_EXPECT(a >= a);
            NSFX_TEST_EXPECT(!(a < a));

            NSFX_TEST_EXPECT(a < b);
            NSFX_TEST_EXPECT(a <= b);
            NSFX_TEST_EXPECT(!(a > b));

            NSFX_TEST_EXPECT(b > a);
            NSFX_TEST_EXPECT(b >= a);
            NSFX_TEST_EXPECT(!(b < a));
        }
    }/*}}}*/

    NSFX_TEST_SUITE(8)/*{{{*/
    {
        typedef nsfx::FixedLengthAddress<8>  Address;

        NSFX_TEST_CASE(Ctor0)
        {
            Address a(0);
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0);
        }

        NSFX_TEST_CASE(Ctor1)
        {
            Address a(0xff);
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0xff);
        }

        NSFX_TEST_CASE(Increment)
        {
            Address a(0xff);
            ++a;
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0);
            --a;
            NSFX_TEST_EXPECT_EQ(a++.GetValue(), 0xff);
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0);
        }

        NSFX_TEST_CASE(Decrement)
        {
            Address a(0);
            --a;
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0xff);
            ++a;
            NSFX_TEST_EXPECT_EQ(a--.GetValue(), 0);
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0xff);
        }

        NSFX_TEST_CASE(Algorithm)
        {
            Address a;
            Address b;

            // +=
            a = Address(0);
            a += 0xff;
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0xff);

            a = Address(0);
            a += -0xff;
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0x01);

            // -=
            a = Address(0);
            a -= 0x01;
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0xff);

            a = Address(0);
            a -= -0x01;
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0x01);

            // +
            a = Address(0);
            b = a + 0xff;
            NSFX_TEST_EXPECT_EQ(b.GetValue(), 0xff);

            b = 0xff + a;
            NSFX_TEST_EXPECT_EQ(b.GetValue(), 0xff);

            // -
            b = a - 0x01;
            NSFX_TEST_EXPECT_EQ(b.GetValue(), 0xff);

            NSFX_TEST_EXPECT_EQ(b - a,  0xff);
            NSFX_TEST_EXPECT_EQ(a - b, -0xff);

            //
            NSFX_TEST_EXPECT(a == a);
            NSFX_TEST_EXPECT(!(a != a));

            NSFX_TEST_EXPECT(a != b);
            NSFX_TEST_EXPECT(!(a == b));

            NSFX_TEST_EXPECT(a <= a);
            NSFX_TEST_EXPECT(!(a > a));

            NSFX_TEST_EXPECT(a >= a);
            NSFX_TEST_EXPECT(!(a < a));

            NSFX_TEST_EXPECT(a < b);
            NSFX_TEST_EXPECT(a <= b);
            NSFX_TEST_EXPECT(!(a > b));

            NSFX_TEST_EXPECT(b > a);
            NSFX_TEST_EXPECT(b >= a);
            NSFX_TEST_EXPECT(!(b < a));
        }
    }/*}}}*/

    NSFX_TEST_SUITE(32)/*{{{*/
    {
        typedef nsfx::FixedLengthAddress<32>  Address;

        NSFX_TEST_CASE(Ctor0)
        {
            Address a(0);
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0);
        }

        NSFX_TEST_CASE(Ctor1)
        {
            Address a(0xffffffff);
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0xffffffff);
        }

        NSFX_TEST_CASE(Increment)
        {
            Address a(0xffffffff);
            ++a;
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0);
            --a;
            NSFX_TEST_EXPECT_EQ(a++.GetValue(), 0xffffffff);
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0);
        }

        NSFX_TEST_CASE(Decrement)
        {
            Address a(0);
            --a;
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0xffffffff);
            ++a;
            NSFX_TEST_EXPECT_EQ(a--.GetValue(), 0);
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0xffffffff);
        }

        NSFX_TEST_CASE(Algorithm)
        {
            Address a;
            Address b;

            // +=
            a = Address(0);
            a += 0xffffffff;
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0xffffffff);

            a = Address(0);
            a += 1;
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0x01);

            // -=
            a = Address(0);
            a -= 1;
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0xffffffff);

            a = Address(0);
            a -= -1;
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0x01);

            // +
            a = Address(0);
            b = a + 0xffffffff;
            NSFX_TEST_EXPECT_EQ(b.GetValue(), 0xffffffff);

            b = 0xffffffff + a;
            NSFX_TEST_EXPECT_EQ(b.GetValue(), 0xffffffff);

            // -
            b = a - 1;
            NSFX_TEST_EXPECT_EQ(b.GetValue(), 0xffffffff);

            NSFX_TEST_EXPECT_EQ(b - a, 0xffffffff);
            NSFX_TEST_EXPECT_EQ(a - b, 1);

            //
            NSFX_TEST_EXPECT(a == a);
            NSFX_TEST_EXPECT(!(a != a));

            NSFX_TEST_EXPECT(a != b);
            NSFX_TEST_EXPECT(!(a == b));

            NSFX_TEST_EXPECT(a <= a);
            NSFX_TEST_EXPECT(!(a > a));

            NSFX_TEST_EXPECT(a >= a);
            NSFX_TEST_EXPECT(!(a < a));

            NSFX_TEST_EXPECT(a < b);
            NSFX_TEST_EXPECT(a <= b);
            NSFX_TEST_EXPECT(!(a > b));

            NSFX_TEST_EXPECT(b > a);
            NSFX_TEST_EXPECT(b >= a);
            NSFX_TEST_EXPECT(!(b < a));
        }
    }/*}}}*/

    NSFX_TEST_SUITE(63)/*{{{*/
    {
        typedef nsfx::FixedLengthAddress<63>  Address;

        NSFX_TEST_CASE(Ctor0)
        {
            Address a(0);
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0);

            NSFX_TEST_EXPECT_EQ(Address::Bcast().GetValue(),
                                0x7fffffffffffffffLL);
        }

        NSFX_TEST_CASE(Ctor1)
        {
            Address a(0x7fffffffffffffffLL);
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0x7fffffffffffffffLL);
        }

        NSFX_TEST_CASE(Increment)
        {
            Address a(0x7fffffffffffffffLL);
            ++a;
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0);
            --a;
            NSFX_TEST_EXPECT_EQ(a++.GetValue(), 0x7fffffffffffffffLL);
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0);
        }

        NSFX_TEST_CASE(Decrement)
        {
            Address a(0);
            --a;
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0x7fffffffffffffffLL);
            ++a;
            NSFX_TEST_EXPECT_EQ(a--.GetValue(), 0);
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0x7fffffffffffffffLL);
        }

        NSFX_TEST_CASE(Algorithm)
        {
            Address a;
            Address b;

            // +=
            a = Address(0);
            a += 0x7fffffffffffffffLL;
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0x7fffffffffffffffLL);

            a = Address(0);
            a += -0x7fffffffffffffffLL;
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0x01);

            // -=
            a = Address(0);
            a -= 0x01;
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0x7fffffffffffffffLL);

            a = Address(0);
            a -= -0x01;
            NSFX_TEST_EXPECT_EQ(a.GetValue(), 0x01);

            // +
            a = Address(0);
            b = a + 0x7fffffffffffffffLL;
            NSFX_TEST_EXPECT_EQ(b.GetValue(), 0x7fffffffffffffffLL);

            b = 0x7fffffffffffffffLL + a;
            NSFX_TEST_EXPECT_EQ(b.GetValue(), 0x7fffffffffffffffLL);

            // -
            b = a - 0x01;
            NSFX_TEST_EXPECT_EQ(b.GetValue(), 0x7fffffffffffffffLL);

            NSFX_TEST_EXPECT_EQ(b - a,  0x7fffffffffffffffLL);
            NSFX_TEST_EXPECT_EQ(a - b, -0x7fffffffffffffffLL);

            //
            NSFX_TEST_EXPECT(a == a);
            NSFX_TEST_EXPECT(!(a != a));

            NSFX_TEST_EXPECT(a != b);
            NSFX_TEST_EXPECT(!(a == b));

            NSFX_TEST_EXPECT(a <= a);
            NSFX_TEST_EXPECT(!(a > a));

            NSFX_TEST_EXPECT(a >= a);
            NSFX_TEST_EXPECT(!(a < a));

            NSFX_TEST_EXPECT(a < b);
            NSFX_TEST_EXPECT(a <= b);
            NSFX_TEST_EXPECT(!(a > b));

            NSFX_TEST_EXPECT(b > a);
            NSFX_TEST_EXPECT(b >= a);
            NSFX_TEST_EXPECT(!(b < a));
        }
    }/*}}}*/
}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cerr);
    nsfx::test::runner::Run();

    return 0;
}

