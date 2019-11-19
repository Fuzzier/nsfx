/**
 * @file
 *
 * @brief Test BiArray.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2019-11-19
 *
 * @copyright Copyright (c) 2019.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/utility/bi-array.h>
#include <iostream>


NSFX_TEST_SUITE(BiArray)
{
    NSFX_TEST_SUITE(POD)
    {
        enum { I = 1 };
        typedef nsfx::BiArray<int, I>  ArrayType;

        NSFX_TEST_CASE(Ctor0)
        {
            ArrayType ar;
            NSFX_TEST_EXPECT_EQ(ar.size(), 0);
            NSFX_TEST_EXPECT(ar.empty());
        }

        NSFX_TEST_CASE(Ctor1)
        {
            ArrayType ar(3);
            NSFX_TEST_EXPECT_EQ(ar.size(), 3);
            NSFX_TEST_EXPECT(!ar.empty());
        }

        NSFX_TEST_CASE(InitialValue)
        {
            ArrayType ar(3);
            for (size_t i = I; i < I + ar.size(); ++i)
            {
                NSFX_TEST_EXPECT_EQ(ar.at(i), 0);
                NSFX_TEST_EXPECT_EQ(ar[i], 0);
                NSFX_TEST_EXPECT_EQ(ar(i), 0);
            }
            const ArrayType& car = ar;
            for (size_t i = I; i < I + ar.size(); ++i)
            {
                NSFX_TEST_EXPECT_EQ(car.at(i), 0);
                NSFX_TEST_EXPECT_EQ(car[i], 0);
                NSFX_TEST_EXPECT_EQ(car(i), 0);
            }
        }

        NSFX_TEST_CASE(FrontBack)
        {
            ArrayType ar(3);
            ar(I) = 12;
            ar(I+1) = 34;
            ar(I+2) = 56;
            NSFX_TEST_EXPECT_EQ(ar.front(), 12);
            NSFX_TEST_EXPECT_EQ(ar.back(), 56);
            const ArrayType& car = ar;
            NSFX_TEST_EXPECT_EQ(car.front(), 12);
            NSFX_TEST_EXPECT_EQ(car.back(), 56);
        }

        NSFX_TEST_CASE(Data)
        {
            ArrayType ar(3);
            ar(I) = 12;
            ar(I+1) = 34;
            ar(I+2) = 56;
            ArrayType::pointer p = ar.data();
            NSFX_TEST_EXPECT_EQ(*p++, 12);
            NSFX_TEST_EXPECT_EQ(*p++, 34);
            NSFX_TEST_EXPECT_EQ(*p++, 56);
            const ArrayType& car = ar;
            ArrayType::const_pointer cp = car.data();
            NSFX_TEST_EXPECT_EQ(*cp++, 12);
            NSFX_TEST_EXPECT_EQ(*cp++, 34);
            NSFX_TEST_EXPECT_EQ(*cp++, 56);
        }

        NSFX_TEST_CASE(Fill)
        {
            ArrayType ar(3);
            ar.fill(0x12345678);
            for (size_t i = I; i < I + ar.size(); ++i)
            {
                NSFX_TEST_EXPECT_EQ(ar(i), 0x12345678);
            }
        }

        NSFX_TEST_CASE(Swap)
        {
            ArrayType ar1(3);
            ar1.fill(0x12345678);
            ArrayType ar2(3);
            ar2.fill(0x9abcdef0);
            boost::swap(ar1, ar2);
            NSFX_TEST_EXPECT_EQ(ar1(I), 0x9abcdef0);
            NSFX_TEST_EXPECT_EQ(ar2(I), 0x12345678);
        }

        NSFX_TEST_CASE(Iterator)
        {
            ArrayType ar(3);
            ar(I) = 12;
            ar(I+1) = 34;
            ar(I+2) = 56;
            // Copy-constructible.
            ArrayType::iterator it = ar.begin();
            NSFX_TEST_EXPECT_EQ(*it, ar(I));
            // Copy-assignable.
            it = ar.begin();
            NSFX_TEST_EXPECT_EQ(*it, ar(I));
            // Increment.
            NSFX_TEST_EXPECT_EQ(*it++, ar(I));
            NSFX_TEST_EXPECT_EQ(*it, ar(I+1));
            NSFX_TEST_EXPECT_EQ(*++it, ar(I+2));
            // Operator ==, !=.
            NSFX_TEST_EXPECT(it == it);
            NSFX_TEST_EXPECT(it != ar.begin());
            // Dereference as rvalue.
            it = ar.begin();
            *it = 1200;
            NSFX_TEST_EXPECT_EQ(ar(I), 1200);
            *it = 12;
            // Default constructible.
            ArrayType::iterator it2;
            // Decrement.
            it = ar.end();
            NSFX_TEST_EXPECT_EQ(*--it, ar(I+2));
            NSFX_TEST_EXPECT_EQ(*it--, ar(I+2));
            NSFX_TEST_EXPECT_EQ(*it, ar(I+1));
            // Operator +, -.
            it = ar.begin() + 2;
            NSFX_TEST_EXPECT_EQ(*it, ar(I+2));
            it = it - 2;
            NSFX_TEST_EXPECT_EQ(*it, ar(I));
            it = 2 + it;
            NSFX_TEST_EXPECT_EQ(*it, ar(I+2));
            NSFX_TEST_EXPECT_EQ(it - ar.begin(), 2);
            // Operator <, >, <=, >=.
            NSFX_TEST_EXPECT(ar.begin() < it);
            NSFX_TEST_EXPECT(it > ar.begin());
            NSFX_TEST_EXPECT(ar.begin() <= it);
            NSFX_TEST_EXPECT(it <= it);
            NSFX_TEST_EXPECT(it >= ar.begin());
            NSFX_TEST_EXPECT(it >= it);
            // Operator +=, -=.
            it = ar.begin();
            it += 2;
            NSFX_TEST_EXPECT_EQ(*it, ar(I+2));
            it -= 2;
            NSFX_TEST_EXPECT_EQ(*it, ar(I));
            // Operator [].
            it = ar.begin();
            NSFX_TEST_EXPECT_EQ(it[0], ar(I));
            NSFX_TEST_EXPECT_EQ(it[1], ar(I+1));
            NSFX_TEST_EXPECT_EQ(it[2], ar(I+2));
        }

        NSFX_TEST_CASE(ConstIterator)
        {
            ArrayType ar(3);
            ar(I) = 12;
            ar(I+1) = 34;
            ar(I+2) = 56;
            // Copy-constructible.
            ArrayType::const_iterator it = ar.cbegin();
            NSFX_TEST_EXPECT_EQ(*it, ar(I));
            NSFX_TEST_EXPECT(ar.cbegin() == ArrayType::const_iterator(ar.begin()));
            NSFX_TEST_EXPECT(ar.cend() == ArrayType::const_iterator(ar.end()));
            // Copy-assignable.
            it = ar.begin();
            it = ar.cbegin();
            NSFX_TEST_EXPECT_EQ(*it, ar(I));
            // Increment.
            NSFX_TEST_EXPECT_EQ(*it++, ar(I));
            NSFX_TEST_EXPECT_EQ(*it, ar(I+1));
            NSFX_TEST_EXPECT_EQ(*++it, ar(I+2));
            // Operator ==, !=.
            NSFX_TEST_EXPECT(it == it);
            NSFX_TEST_EXPECT(it != ar.cbegin());
            // Dereference as lvalue.
            it = ar.cbegin();
            NSFX_TEST_EXPECT_EQ(*it, 12);
            // Default constructible.
            ArrayType::const_iterator it2;
            // Decrement.
            it = ar.cend();
            NSFX_TEST_EXPECT_EQ(*--it, ar(I+2));
            NSFX_TEST_EXPECT_EQ(*it--, ar(I+2));
            NSFX_TEST_EXPECT_EQ(*it, ar(I+1));
            // Operator +, -.
            it = ar.cbegin() + 2;
            NSFX_TEST_EXPECT_EQ(*it, ar(I+2));
            it = it - 2;
            NSFX_TEST_EXPECT_EQ(*it, ar(I));
            it = 2 + it;
            NSFX_TEST_EXPECT_EQ(*it, ar(I+2));
            NSFX_TEST_EXPECT_EQ(it - ar.cbegin(), 2);
            // Operator <, >, <=, >=.
            NSFX_TEST_EXPECT(ar.cbegin() < it);
            NSFX_TEST_EXPECT(it > ar.cbegin());
            NSFX_TEST_EXPECT(ar.cbegin() <= it);
            NSFX_TEST_EXPECT(it <= it);
            NSFX_TEST_EXPECT(it >= ar.cbegin());
            NSFX_TEST_EXPECT(it >= it);
            // Operator +=, -=.
            it = ar.cbegin();
            it += 2;
            NSFX_TEST_EXPECT_EQ(*it, ar(I+2));
            it -= 2;
            NSFX_TEST_EXPECT_EQ(*it, ar(I));
            // Operator [].
            it = ar.cbegin();
            NSFX_TEST_EXPECT_EQ(it[0], ar(I));
            NSFX_TEST_EXPECT_EQ(it[1], ar(I+1));
            NSFX_TEST_EXPECT_EQ(it[2], ar(I+2));
        }

    }
}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cerr);
    nsfx::test::runner::Run();

    return 0;
}

