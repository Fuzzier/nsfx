/**
 * @file
 *
 * @brief Test BiVector.
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
#include <nsfx/utility/bi-vector.h>
#include <iostream>


NSFX_TEST_SUITE(BiVector)
{
    NSFX_TEST_SUITE(Primitive)/*{{{*/
    {
        enum { I = 1 };
        typedef nsfx::BiVector<int, I>  VectorType;

        NSFX_TEST_CASE(Ctor0)
        {
            VectorType vt;
            NSFX_TEST_EXPECT_EQ(vt.capacity(), 0);
            NSFX_TEST_EXPECT_EQ(vt.size(), 0);
            NSFX_TEST_EXPECT(vt.empty());
        }

        NSFX_TEST_CASE(Ctor1)
        {
            VectorType vt(3);
            vt.push_back(12);
            NSFX_TEST_EXPECT_EQ(vt.capacity(), 3);
            NSFX_TEST_EXPECT_EQ(vt.size(), 1);
            NSFX_TEST_EXPECT(!vt.empty());
        }

        NSFX_TEST_CASE(Copy)
        {
            VectorType vt(3);
            vt.push_back(12);
            vt.push_back(34);
            vt.push_back(56);
            NSFX_TEST_EXPECT_EQ(vt.capacity(), 3);
            NSFX_TEST_EXPECT_EQ(vt.size(), 3);
            NSFX_TEST_EXPECT(!vt.empty());
            VectorType vt2(vt);
            NSFX_TEST_EXPECT_EQ(vt2.capacity(), 3);
            NSFX_TEST_EXPECT_EQ(vt2.size(), 3);
            NSFX_TEST_EXPECT_EQ(vt2(I), 12);
            NSFX_TEST_EXPECT_EQ(vt2(I+1), 34);
            NSFX_TEST_EXPECT_EQ(vt2(I+2), 56);
            VectorType vt3(4);
            vt3 = vt2;
            NSFX_TEST_EXPECT_EQ(vt3.capacity(), 3);
            NSFX_TEST_EXPECT_EQ(vt3.size(), 3);
            NSFX_TEST_EXPECT_EQ(vt3(I), 12);
            NSFX_TEST_EXPECT_EQ(vt3(I+1), 34);
            NSFX_TEST_EXPECT_EQ(vt3(I+2), 56);
        }

        NSFX_TEST_CASE(Move)
        {
            VectorType vt(3);
            vt.push_back(12);
            vt.push_back(34);
            vt.push_back(56);
            VectorType vt2(std::move(vt));
            NSFX_TEST_EXPECT_EQ(vt.capacity(), 0);
            NSFX_TEST_EXPECT_EQ(vt.size(), 0);
            NSFX_TEST_EXPECT_EQ(vt2.capacity(), 3);
            NSFX_TEST_EXPECT_EQ(vt2.size(), 3);
            NSFX_TEST_EXPECT_EQ(vt2(I), 12);
            NSFX_TEST_EXPECT_EQ(vt2(I+1), 34);
            NSFX_TEST_EXPECT_EQ(vt2(I+2), 56);
            VectorType vt3(4);
            vt3 = std::move(vt2);
            NSFX_TEST_EXPECT_EQ(vt2.capacity(), 0);
            NSFX_TEST_EXPECT_EQ(vt2.size(), 0);
            NSFX_TEST_EXPECT_EQ(vt3.capacity(), 3);
            NSFX_TEST_EXPECT_EQ(vt3.size(), 3);
            NSFX_TEST_EXPECT_EQ(vt3(I), 12);
            NSFX_TEST_EXPECT_EQ(vt3(I+1), 34);
            NSFX_TEST_EXPECT_EQ(vt3(I+2), 56);
        }

        NSFX_TEST_CASE(FrontBack)
        {
            VectorType vt(3);
            vt.push_back(12);
            vt.push_back(34);
            vt.push_back(56);
            NSFX_TEST_EXPECT_EQ(vt.front(), 12);
            NSFX_TEST_EXPECT_EQ(vt.back(), 56);
            const VectorType& car = vt;
            NSFX_TEST_EXPECT_EQ(car.front(), 12);
            NSFX_TEST_EXPECT_EQ(car.back(), 56);
        }

        NSFX_TEST_CASE(Data)
        {
            VectorType vt(3);
            vt.push_back(12);
            vt.push_back(34);
            vt.push_back(56);
            VectorType::pointer p = vt.data();
            NSFX_TEST_EXPECT_EQ(*p++, 12);
            NSFX_TEST_EXPECT_EQ(*p++, 34);
            NSFX_TEST_EXPECT_EQ(*p++, 56);
            const VectorType& car = vt;
            VectorType::const_pointer cp = car.data();
            NSFX_TEST_EXPECT_EQ(*cp++, 12);
            NSFX_TEST_EXPECT_EQ(*cp++, 34);
            NSFX_TEST_EXPECT_EQ(*cp++, 56);
        }

        NSFX_TEST_CASE(Clear)
        {
            VectorType vt(3);
            vt.push_back(12);
            vt.push_back(34);
            vt.push_back(56);
            vt.clear();
            NSFX_TEST_EXPECT_EQ(vt.size(), 0);
        }

        NSFX_TEST_CASE(Assign)
        {
            VectorType vt(3);
            vt.push_back(0xffffffff);
            // Fill & expand.
            vt.assign(3, 0x12345678);
            NSFX_TEST_EXPECT_EQ(vt.size(), 3);
            for (size_t i = I; i < I + vt.size(); ++i)
            {
                NSFX_TEST_EXPECT_EQ(vt(i), 0x12345678);
                std::vector<int> v;
                v.assign(3, 0x1);
            }
            // Fill & replace.
            vt.assign(2, 0x2468ace0);
            NSFX_TEST_EXPECT_EQ(vt.size(), 3);
            NSFX_TEST_EXPECT_EQ(vt(I),   0x2468ace0);
            NSFX_TEST_EXPECT_EQ(vt(I+1), 0x2468ace0);
            NSFX_TEST_EXPECT_EQ(vt(I+2), 0x12345678);
            // Reset.
            vt.clear();
            vt.push_back(0xffffffff);
            // Range & expand.
            int x[] = { 0x11111111, 0x22222222, 0x33333333 };
            vt.assign(x, x+3);
            NSFX_TEST_EXPECT_EQ(vt.size(), 3);
            NSFX_TEST_EXPECT_EQ(vt(I),   x[0]);
            NSFX_TEST_EXPECT_EQ(vt(I+1), x[1]);
            NSFX_TEST_EXPECT_EQ(vt(I+2), x[2]);
            // Range & replace.
            int y[] = { 0x44444444, 0x55555555 };
            vt.assign(y, y+2);
            NSFX_TEST_EXPECT_EQ(vt.size(), 3);
            NSFX_TEST_EXPECT_EQ(vt(I),   y[0]);
            NSFX_TEST_EXPECT_EQ(vt(I+1), y[1]);
            NSFX_TEST_EXPECT_EQ(vt(I+2), x[2]);
        }

        NSFX_TEST_CASE(Swap)
        {
            VectorType vt1(3);
            vt1.assign(3, 0x12345678);
            VectorType vt2(3);
            vt2.assign(3, 0x9abcdef0);
            boost::swap(vt1, vt2);
            NSFX_TEST_EXPECT_EQ(vt1(I), 0x9abcdef0);
            NSFX_TEST_EXPECT_EQ(vt2(I), 0x12345678);
        }

        NSFX_TEST_CASE(Iterator)
        {
            VectorType vt(3);
            vt.push_back(12);
            vt.push_back(34);
            vt.push_back(56);
            // Copy-constructible.
            VectorType::iterator it = vt.begin();
            NSFX_TEST_EXPECT_EQ(*it, vt(I));
            // Copy-assignable.
            it = vt.begin();
            NSFX_TEST_EXPECT_EQ(*it, vt(I));
            // Increment.
            NSFX_TEST_EXPECT_EQ(*it++, vt(I));
            NSFX_TEST_EXPECT_EQ(*it, vt(I+1));
            NSFX_TEST_EXPECT_EQ(*++it, vt(I+2));
            // Operator ==, !=.
            NSFX_TEST_EXPECT(it == it);
            NSFX_TEST_EXPECT(it != vt.begin());
            // Operator *, ->.
            // NOTE: primitive type has no operator->.
            it = vt.begin();
            *it = 1200;
            NSFX_TEST_EXPECT_EQ(vt(I), 1200);
            *it = 12;
            // Default constructible.
            VectorType::iterator it2;
            // Decrement.
            it = vt.end();
            NSFX_TEST_EXPECT_EQ(*--it, vt(I+vt.size()-1));
            NSFX_TEST_EXPECT_EQ(*it--, vt(I+vt.size()-1));
            NSFX_TEST_EXPECT_EQ(*it, vt(I+vt.size()-2));
            // Operator +, -.
            it = vt.begin() + 2;
            NSFX_TEST_EXPECT_EQ(*it, vt(I+2));
            it = it - 2;
            NSFX_TEST_EXPECT_EQ(*it, vt(I));
            it = 2 + it;
            NSFX_TEST_EXPECT_EQ(*it, vt(I+2));
            NSFX_TEST_EXPECT_EQ(it - vt.begin(), 2);
            // Operator <, >, <=, >=.
            NSFX_TEST_EXPECT(vt.begin() < it);
            NSFX_TEST_EXPECT(it > vt.begin());
            NSFX_TEST_EXPECT(vt.begin() <= it);
            NSFX_TEST_EXPECT(it <= it);
            NSFX_TEST_EXPECT(it >= vt.begin());
            NSFX_TEST_EXPECT(it >= it);
            // Operator +=, -=.
            it = vt.begin();
            it += 2;
            NSFX_TEST_EXPECT_EQ(*it, vt(I+2));
            it -= 2;
            NSFX_TEST_EXPECT_EQ(*it, vt(I));
            // Operator [].
            it = vt.begin();
            NSFX_TEST_EXPECT_EQ(it[0], vt(I));
            NSFX_TEST_EXPECT_EQ(it[1], vt(I+1));
            NSFX_TEST_EXPECT_EQ(it[2], vt(I+2));
        }

        NSFX_TEST_CASE(ConstIterator)
        {
            VectorType vt(3);
            vt.push_back(12);
            vt.push_back(34);
            vt.push_back(56);
            // Copy-constructible.
            VectorType::const_iterator it = vt.cbegin();
            NSFX_TEST_EXPECT_EQ(*it, vt(I));
            // NOTE: copy-construct from iterator.
            NSFX_TEST_EXPECT(vt.cbegin() == VectorType::const_iterator(vt.begin()));
            NSFX_TEST_EXPECT(vt.cend() == VectorType::const_iterator(vt.end()));
            // Copy-assignable.
            it = vt.cbegin();
            // NOTE: copy-assign from iterator.
            it = vt.begin();
            NSFX_TEST_EXPECT_EQ(*it, vt(I));
            // Increment.
            NSFX_TEST_EXPECT_EQ(*it++, vt(I));
            NSFX_TEST_EXPECT_EQ(*it, vt(I+1));
            NSFX_TEST_EXPECT_EQ(*++it, vt(I+2));
            // Operator ==, !=.
            NSFX_TEST_EXPECT(it == it);
            NSFX_TEST_EXPECT(it != vt.cbegin());
            // NOTE: iterator ? const_iterator.
            NSFX_TEST_EXPECT(vt.begin() == vt.cbegin());
            NSFX_TEST_EXPECT(vt.begin() != vt.cend());
            // Operator *, ->.
            // NOTE: readonly, primitive type has no operator->.
            it = vt.cbegin();
            NSFX_TEST_EXPECT_EQ(*it, vt(I));
            // Default constructible.
            VectorType::const_iterator it2;
            // Decrement.
            it = vt.cend();
            NSFX_TEST_EXPECT_EQ(*--it, vt(I+vt.size()-1));
            NSFX_TEST_EXPECT_EQ(*it--, vt(I+vt.size()-1));
            NSFX_TEST_EXPECT_EQ(*it, vt(I+vt.size()-2));
            // Operator +, -.
            it = vt.cbegin() + 2;
            NSFX_TEST_EXPECT_EQ(*it, vt(I+2));
            it = it - 2;
            NSFX_TEST_EXPECT_EQ(*it, vt(I));
            it = 2 + it;
            NSFX_TEST_EXPECT_EQ(*it, vt(I+2));
            NSFX_TEST_EXPECT_EQ(it - vt.cbegin(), 2);
            // NOTE: iterator - const_iterator.
            NSFX_TEST_EXPECT_EQ(vt.end() - vt.cbegin(), vt.size());
            // Operator <, >, <=, >=.
            NSFX_TEST_EXPECT(vt.cbegin() < it);
            NSFX_TEST_EXPECT(it > vt.cbegin());
            NSFX_TEST_EXPECT(vt.cbegin() <= it);
            NSFX_TEST_EXPECT(it <= it);
            NSFX_TEST_EXPECT(it >= vt.cbegin());
            NSFX_TEST_EXPECT(it >= it);
            // NOTE: iterator ? const_iterator.
            NSFX_TEST_EXPECT(vt.begin() < vt.cend());
            NSFX_TEST_EXPECT(vt.begin() <= vt.cend());
            NSFX_TEST_EXPECT(vt.end() > vt.cbegin());
            NSFX_TEST_EXPECT(vt.end() >= vt.cbegin());
            // Operator +=, -=.
            it = vt.cbegin();
            it += 2;
            NSFX_TEST_EXPECT_EQ(*it, vt(I+2));
            it -= 2;
            NSFX_TEST_EXPECT_EQ(*it, vt(I));
            // Operator [].
            it = vt.cbegin();
            NSFX_TEST_EXPECT_EQ(it[0], vt(I));
            NSFX_TEST_EXPECT_EQ(it[1], vt(I+1));
            NSFX_TEST_EXPECT_EQ(it[2], vt(I+2));
        }

        NSFX_TEST_CASE(ReverseIterator)
        {
            VectorType vt(3);
            vt.push_back(12);
            vt.push_back(34);
            vt.push_back(56);
            VectorType::reverse_iterator it = vt.rbegin();
            NSFX_TEST_EXPECT_EQ(*it++, vt(I+2));
            NSFX_TEST_EXPECT_EQ(*it++, vt(I+1));
            NSFX_TEST_EXPECT_EQ(*it++, vt(I));
            NSFX_TEST_EXPECT(it == vt.rend());
            it = vt.rend();
            NSFX_TEST_EXPECT_EQ(*--it, vt(I));
            NSFX_TEST_EXPECT_EQ(*--it, vt(I+1));
            NSFX_TEST_EXPECT_EQ(*--it, vt(I+2));
            NSFX_TEST_EXPECT(it == vt.rbegin());
        }
    }/*}}}*/

    NSFX_TEST_SUITE(Class)/*{{{*/
    {
        struct A {
            A(void) : m_(1) {};
            A(int m) : m_(m) {};
            bool operator==(const A& rhs) const { return m_ == rhs.m_; }
            bool operator==(int rhs) const { return m_ == rhs; }
            A& operator=(int m) { m_ = m; return *this; }
            int m_;
        };
        enum { I = 1 };
        typedef nsfx::BiVector<A, I>  VectorType;

        NSFX_TEST_CASE(Ctor0)
        {
            VectorType vt;
            NSFX_TEST_EXPECT_EQ(vt.capacity(), 0);
            NSFX_TEST_EXPECT_EQ(vt.size(), 0);
            NSFX_TEST_EXPECT(vt.empty());
        }

        NSFX_TEST_CASE(Ctor1)
        {
            VectorType vt(3);
            A a1;
            vt.push_back(a1);
            NSFX_TEST_EXPECT_EQ(vt.capacity(), 3);
            NSFX_TEST_EXPECT_EQ(vt.size(), 1);
            NSFX_TEST_EXPECT(!vt.empty());
        }

        NSFX_TEST_CASE(Copy)
        {
            VectorType vt(3);
            vt.push_back(A(12));
            vt.push_back(A(34));
            vt.push_back(A(56));
            VectorType vt2(vt);
            NSFX_TEST_EXPECT(vt2.size() == 3);
            NSFX_TEST_EXPECT(vt2(I) == 12);
            NSFX_TEST_EXPECT(vt2(I+1) == 34);
            NSFX_TEST_EXPECT(vt2(I+2) == 56);
            VectorType vt3(4);
            vt3 = vt2;
            NSFX_TEST_EXPECT(vt3.size() == 3);
            NSFX_TEST_EXPECT(vt3(I) == 12);
            NSFX_TEST_EXPECT(vt3(I+1) == 34);
            NSFX_TEST_EXPECT(vt3(I+2) == 56);
        }

        NSFX_TEST_CASE(Move)
        {
            VectorType vt(3);
            vt.push_back(A(12));
            vt.push_back(A(34));
            vt.push_back(A(56));
            VectorType vt2(std::move(vt));
            NSFX_TEST_EXPECT_EQ(vt.size(), 0);
            NSFX_TEST_EXPECT_EQ(vt2.size(), 3);
            NSFX_TEST_EXPECT(vt2(I) == 12);
            NSFX_TEST_EXPECT(vt2(I+1) == 34);
            NSFX_TEST_EXPECT(vt2(I+2) == 56);
            VectorType vt3(4);
            vt3 = std::move(vt2);
            NSFX_TEST_EXPECT_EQ(vt2.size(), 0);
            NSFX_TEST_EXPECT_EQ(vt3.size(), 3);
            NSFX_TEST_EXPECT(vt3(I) == 12);
            NSFX_TEST_EXPECT(vt3(I+1) == 34);
            NSFX_TEST_EXPECT(vt3(I+2) == 56);
        }

    }/*}}}*/

}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cerr);
    nsfx::test::runner::Run();

    return 0;
}

