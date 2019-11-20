/**
 * @file
 *
 * @brief Test BiMatrix.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2019-11-20
 *
 * @copyright Copyright (c) 2019.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#include <nsfx/test.h>
#include <nsfx/utility/bi-matrix.h>
#include <iostream>


NSFX_TEST_SUITE(BiMatrix)
{
    // NOTE: const iterator differs from iterator
    // XXX: column major differs from row major
    NSFX_TEST_SUITE(RowMajor)
    {
        NSFX_TEST_SUITE(Primitive)/*{{{*/
        {
            enum { I = 1, J = 2 };
            typedef nsfx::BiMatrix<int, I, J, nsfx::bi_matrix_row_major>
                    MatrixType;

            NSFX_TEST_CASE(Ctor0)
            {
                MatrixType mx;
                NSFX_TEST_EXPECT_EQ(mx.size(), 0);
                NSFX_TEST_EXPECT_EQ(mx.size1(), 0);
                NSFX_TEST_EXPECT_EQ(mx.size2(), 0);
                NSFX_TEST_EXPECT(mx.empty());
            }

            NSFX_TEST_CASE(Ctor1)
            {
                MatrixType mx(4, 3);
                NSFX_TEST_EXPECT_EQ(mx.size(), 12);
                NSFX_TEST_EXPECT_EQ(mx.size1(), 4);
                NSFX_TEST_EXPECT_EQ(mx.size2(), 3);
                NSFX_TEST_EXPECT(!mx.empty());
            }

            NSFX_TEST_CASE(Copy)
            {
                MatrixType mx(4, 3);
                mx(I,   J) = 11; mx(I,   J+1) = 12; mx(I,   J+2) = 13;
                mx(I+1, J) = 21; mx(I+1, J+1) = 22; mx(I+1, J+2) = 23;
                mx(I+2, J) = 31; mx(I+2, J+1) = 32; mx(I+2, J+2) = 33;
                mx(I+3, J) = 41; mx(I+3, J+1) = 42; mx(I+3, J+2) = 43;
                MatrixType mx2(mx);
                NSFX_TEST_EXPECT_EQ(mx2.size1(), 4);
                NSFX_TEST_EXPECT_EQ(mx2.size2(), 3);
                NSFX_TEST_EXPECT_EQ(mx2(I,J), 11);
                NSFX_TEST_EXPECT_EQ(mx2(I+3,J+2), 43);
                MatrixType mx3(1, 2);
                mx3 = mx2;
                NSFX_TEST_EXPECT_EQ(mx3.size1(), 4);
                NSFX_TEST_EXPECT_EQ(mx3.size2(), 3);
                NSFX_TEST_EXPECT_EQ(mx3(I,J), 11);
                NSFX_TEST_EXPECT_EQ(mx3(I+3,J+2), 43);
            }

            NSFX_TEST_CASE(Move)
            {
                MatrixType mx(4, 3);
                mx(I,   J) = 11; mx(I,   J+1) = 12; mx(I,   J+2) = 13;
                mx(I+1, J) = 21; mx(I+1, J+1) = 22; mx(I+1, J+2) = 23;
                mx(I+2, J) = 31; mx(I+2, J+1) = 32; mx(I+2, J+2) = 33;
                mx(I+3, J) = 41; mx(I+3, J+1) = 42; mx(I+3, J+2) = 43;
                MatrixType mx2(std::move(mx));
                NSFX_TEST_EXPECT_EQ(mx.size1(), 0);
                NSFX_TEST_EXPECT_EQ(mx.size2(), 0);
                NSFX_TEST_EXPECT_EQ(mx2.size1(), 4);
                NSFX_TEST_EXPECT_EQ(mx2.size2(), 3);
                NSFX_TEST_EXPECT_EQ(mx2(I,J), 11);
                NSFX_TEST_EXPECT_EQ(mx2(I+3,J+2), 43);
                MatrixType mx3(1, 2);
                mx3 = std::move(mx2);
                NSFX_TEST_EXPECT_EQ(mx2.size1(), 0);
                NSFX_TEST_EXPECT_EQ(mx2.size2(), 0);
                NSFX_TEST_EXPECT_EQ(mx3.size1(), 4);
                NSFX_TEST_EXPECT_EQ(mx3.size2(), 3);
                NSFX_TEST_EXPECT_EQ(mx3(I,J), 11);
                NSFX_TEST_EXPECT_EQ(mx3(I+3,J+2), 43);
            }

            NSFX_TEST_CASE(InitialValue)
            {
                MatrixType mx(4, 3);
                for (size_t i = I; i < I + mx.size1(); ++i)
                for (size_t j = J; j < J + mx.size2(); ++j)
                {
                    NSFX_TEST_EXPECT_EQ(mx.at(i,j), 0);
                    NSFX_TEST_EXPECT_EQ(mx(i,j), 0);
                }
                for (size_t i = 0; i < mx.size(); ++i)
                {
                    NSFX_TEST_EXPECT_EQ(mx(i), 0);
                    NSFX_TEST_EXPECT_EQ(mx[i], 0);
                }
                const MatrixType& cmx = mx;
                for (size_t i = I; i < I + mx.size1(); ++i)
                for (size_t j = J; j < J + mx.size2(); ++j)
                {
                    NSFX_TEST_EXPECT_EQ(cmx.at(i,j), 0);
                    NSFX_TEST_EXPECT_EQ(cmx(i,j), 0);
                }
                for (size_t i = 0; i < mx.size(); ++i)
                {
                    NSFX_TEST_EXPECT_EQ(cmx(i), 0);
                    NSFX_TEST_EXPECT_EQ(cmx[i], 0);
                }
            }

            NSFX_TEST_CASE(FrontBack)
            {
                MatrixType mx(4, 3);
                mx(I,   J) = 11; mx(I,   J+1) = 12; mx(I,   J+2) = 13;
                mx(I+1, J) = 21; mx(I+1, J+1) = 22; mx(I+1, J+2) = 23;
                mx(I+2, J) = 31; mx(I+2, J+1) = 32; mx(I+2, J+2) = 33;
                mx(I+3, J) = 41; mx(I+3, J+1) = 42; mx(I+3, J+2) = 43;
                NSFX_TEST_EXPECT_EQ(mx.front(), 11);
                NSFX_TEST_EXPECT_EQ(mx.back(), 43);
                const MatrixType& cmx = mx;
                NSFX_TEST_EXPECT_EQ(cmx.front(), 11);
                NSFX_TEST_EXPECT_EQ(cmx.back(), 43);
            }

            NSFX_TEST_CASE(Data)
            {
                MatrixType mx(4, 3);
                mx(I,   J) = 11; mx(I,   J+1) = 12; mx(I,   J+2) = 13;
                mx(I+1, J) = 21; mx(I+1, J+1) = 22; mx(I+1, J+2) = 23;
                mx(I+2, J) = 31; mx(I+2, J+1) = 32; mx(I+2, J+2) = 33;
                mx(I+3, J) = 41; mx(I+3, J+1) = 42; mx(I+3, J+2) = 43;
                MatrixType::pointer p = mx.data();
                // Row major.
                NSFX_TEST_EXPECT_EQ(*p++, 11);
                NSFX_TEST_EXPECT_EQ(*p++, 12);
                NSFX_TEST_EXPECT_EQ(*p++, 13);
                NSFX_TEST_EXPECT_EQ(*p++, 21);
                NSFX_TEST_EXPECT_EQ(*p++, 22);
                NSFX_TEST_EXPECT_EQ(*p++, 23);
                NSFX_TEST_EXPECT_EQ(*p++, 31);
                NSFX_TEST_EXPECT_EQ(*p++, 32);
                NSFX_TEST_EXPECT_EQ(*p++, 33);
                NSFX_TEST_EXPECT_EQ(*p++, 41);
                NSFX_TEST_EXPECT_EQ(*p++, 42);
                NSFX_TEST_EXPECT_EQ(*p++, 43);
                const MatrixType& cmx = mx;
                MatrixType::const_pointer cp = cmx.data();
                NSFX_TEST_EXPECT_EQ(*cp++, 11);
                NSFX_TEST_EXPECT_EQ(*cp++, 12);
                NSFX_TEST_EXPECT_EQ(*cp++, 13);
                NSFX_TEST_EXPECT_EQ(*cp++, 21);
                NSFX_TEST_EXPECT_EQ(*cp++, 22);
                NSFX_TEST_EXPECT_EQ(*cp++, 23);
                NSFX_TEST_EXPECT_EQ(*cp++, 31);
                NSFX_TEST_EXPECT_EQ(*cp++, 32);
                NSFX_TEST_EXPECT_EQ(*cp++, 33);
                NSFX_TEST_EXPECT_EQ(*cp++, 41);
                NSFX_TEST_EXPECT_EQ(*cp++, 42);
                NSFX_TEST_EXPECT_EQ(*cp++, 43);
            }

            NSFX_TEST_CASE(Fill)
            {
                MatrixType mx(4, 3);
                mx.fill(0x12345678);
                for (size_t i = 0; i < mx.size(); ++i)
                {
                    NSFX_TEST_EXPECT_EQ(mx(i), 0x12345678);
                }
                mx.fill1(I, 0x9abcdef0);
                for (size_t j = J; j < J + mx.size2(); ++j)
                {
                    NSFX_TEST_EXPECT_EQ(mx(I,j), 0x9abcdef0);
                }
                mx.fill2(J, 0x2468ace0);
                for (size_t i = I; i < I + mx.size1(); ++i)
                {
                    NSFX_TEST_EXPECT_EQ(mx(i,J), 0x2468ace0);
                }
            }

            NSFX_TEST_CASE(Swap)
            {
                MatrixType mx1(4, 3);
                mx1.fill(0x12345678);
                MatrixType mx2(4, 3);
                mx2.fill(0x9abcdef0);
                boost::swap(mx1, mx2);
                NSFX_TEST_EXPECT_EQ(mx1(0), 0x9abcdef0);
                NSFX_TEST_EXPECT_EQ(mx2(0), 0x12345678);
            }

            NSFX_TEST_CASE(Iterator)
            {
                MatrixType mx(4, 3);
                mx(I,   J) = 11; mx(I,   J+1) = 12; mx(I,   J+2) = 13;
                mx(I+1, J) = 21; mx(I+1, J+1) = 22; mx(I+1, J+2) = 23;
                mx(I+2, J) = 31; mx(I+2, J+1) = 32; mx(I+2, J+2) = 33;
                mx(I+3, J) = 41; mx(I+3, J+1) = 42; mx(I+3, J+2) = 43;
                // Copy-constructible.
                MatrixType::iterator it = mx.begin();
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Copy-assignable.
                it = mx.begin();
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Increment.
                NSFX_TEST_EXPECT_EQ(*it++, mx(I,J));
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J+1));
                NSFX_TEST_EXPECT_EQ(*++it, mx(I,J+2));
                // Operator ==, !=.
                NSFX_TEST_EXPECT(it == it);
                NSFX_TEST_EXPECT(it != mx.begin());
                // Operator *, ->.
                // NOTE: primitive type has no operator->.
                it = mx.begin();
                *it = 1100;
                NSFX_TEST_EXPECT_EQ(mx(I,J), 1100);
                *it = 11;
                // Default constructible.
                MatrixType::iterator it2;
                // Decrement.
                it = mx.end();
                NSFX_TEST_EXPECT_EQ(*--it, mx(I+mx.size1()-1,J+mx.size2()-1));
                NSFX_TEST_EXPECT_EQ(*it--, mx(I+mx.size1()-1,J+mx.size2()-1));
                NSFX_TEST_EXPECT_EQ(*it, mx(I+mx.size1()-1,J+mx.size2()-2));
                // Operator +, -.
                it = mx.begin() + 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J+2));
                it = it - 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                it = 2 + it;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J+2));
                NSFX_TEST_EXPECT_EQ(it - mx.begin(), 2);
                // Operator <, >, <=, >=.
                NSFX_TEST_EXPECT(mx.begin() < it);
                NSFX_TEST_EXPECT(it > mx.begin());
                NSFX_TEST_EXPECT(mx.begin() <= it);
                NSFX_TEST_EXPECT(it <= it);
                NSFX_TEST_EXPECT(it >= mx.begin());
                NSFX_TEST_EXPECT(it >= it);
                // Operator +=, -=.
                it = mx.begin();
                it += 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J+2));
                it -= 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Operator [].
                it = mx.begin();
                NSFX_TEST_EXPECT_EQ(it[0], mx(I,J));
                NSFX_TEST_EXPECT_EQ(it[1], mx(I,J+1));
                NSFX_TEST_EXPECT_EQ(it[2], mx(I,J+2));
            }

            NSFX_TEST_CASE(ConstIterator)
            {
                MatrixType mx(4, 3);
                mx(I,   J) = 11; mx(I,   J+1) = 12; mx(I,   J+2) = 13;
                mx(I+1, J) = 21; mx(I+1, J+1) = 22; mx(I+1, J+2) = 23;
                mx(I+2, J) = 31; mx(I+2, J+1) = 32; mx(I+2, J+2) = 33;
                mx(I+3, J) = 41; mx(I+3, J+1) = 42; mx(I+3, J+2) = 43;
                // Copy-constructible.
                MatrixType::const_iterator it = mx.cbegin();
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // NOTE: copy-construct from iterator.
                NSFX_TEST_EXPECT(mx.cbegin() == MatrixType::const_iterator(mx.begin()));
                NSFX_TEST_EXPECT(mx.cend() == MatrixType::const_iterator(mx.end()));
                // Copy-assignable.
                it = mx.cbegin();
                // NOTE: copy-assign from iterator.
                it = mx.begin();
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Increment.
                NSFX_TEST_EXPECT_EQ(*it++, mx(I,J));
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J+1));
                NSFX_TEST_EXPECT_EQ(*++it, mx(I,J+2));
                // Operator ==, !=.
                NSFX_TEST_EXPECT(it == it);
                NSFX_TEST_EXPECT(it != mx.cbegin());
                // NOTE: iterator ? const_iterator.
                NSFX_TEST_EXPECT(mx.begin() == mx.cbegin());
                NSFX_TEST_EXPECT(mx.begin() != mx.cend());
                // Operator *, ->.
                // NOTE: readonly, primitive type has no operator->.
                it = mx.cbegin();
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Default constructible.
                MatrixType::const_iterator it2;
                // Decrement.
                it = mx.cend();
                NSFX_TEST_EXPECT_EQ(*--it, mx(I+mx.size1()-1,J+mx.size2()-1));
                NSFX_TEST_EXPECT_EQ(*it--, mx(I+mx.size1()-1,J+mx.size2()-1));
                NSFX_TEST_EXPECT_EQ(*it, mx(I+mx.size1()-1,J+mx.size2()-2));
                // Operator +, -.
                it = mx.cbegin() + 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J+2));
                it = it - 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                it = 2 + it;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J+2));
                NSFX_TEST_EXPECT_EQ(it - mx.cbegin(), 2);
                // NOTE: iterator - const_iterator.
                NSFX_TEST_EXPECT_EQ(mx.end() - mx.cbegin(), mx.size());
                // Operator <, >, <=, >=.
                NSFX_TEST_EXPECT(mx.cbegin() < it);
                NSFX_TEST_EXPECT(it > mx.cbegin());
                NSFX_TEST_EXPECT(mx.cbegin() <= it);
                NSFX_TEST_EXPECT(it <= it);
                NSFX_TEST_EXPECT(it >= mx.cbegin());
                NSFX_TEST_EXPECT(it >= it);
                // NOTE: iterator ? const_iterator.
                NSFX_TEST_EXPECT(mx.begin() < mx.cend());
                NSFX_TEST_EXPECT(mx.begin() <= mx.cend());
                NSFX_TEST_EXPECT(mx.end() > mx.cbegin());
                NSFX_TEST_EXPECT(mx.end() >= mx.cbegin());
                // Operator +=, -=.
                it = mx.cbegin();
                it += 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J+2));
                it -= 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Operator [].
                it = mx.cbegin();
                NSFX_TEST_EXPECT_EQ(it[0], mx(I,J));
                NSFX_TEST_EXPECT_EQ(it[1], mx(I,J+1));
                NSFX_TEST_EXPECT_EQ(it[2], mx(I,J+2));
            }

            NSFX_TEST_CASE(RowIterator)
            {
                MatrixType mx(4, 3);
                mx(I,   J) = 11; mx(I,   J+1) = 12; mx(I,   J+2) = 13;
                mx(I+1, J) = 21; mx(I+1, J+1) = 22; mx(I+1, J+2) = 23;
                mx(I+2, J) = 31; mx(I+2, J+1) = 32; mx(I+2, J+2) = 33;
                mx(I+3, J) = 41; mx(I+3, J+1) = 42; mx(I+3, J+2) = 43;
                // Copy-constructible.
                MatrixType::row_iterator it = mx.begin1(I);
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Copy-assignable.
                it = mx.begin1(I);
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Increment.
                NSFX_TEST_EXPECT_EQ(*it++, mx(I,J));
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J+1));
                NSFX_TEST_EXPECT_EQ(*++it, mx(I,J+2));
                // Operator ==, !=.
                NSFX_TEST_EXPECT(it == it);
                NSFX_TEST_EXPECT(it != mx.begin1(I));
                // Operator *, ->.
                // NOTE: primitive type has no operator->.
                it = mx.begin1(I);
                *it = 1100;
                NSFX_TEST_EXPECT_EQ(mx(I,J), 1100);
                *it = 11;
                // Default constructible.
                MatrixType::row_iterator it2;
                // Decrement.
                it = mx.end1(I);
                NSFX_TEST_EXPECT_EQ(*--it, mx(I,J+mx.size2()-1));
                NSFX_TEST_EXPECT_EQ(*it--, mx(I,J+mx.size2()-1));
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J+mx.size2()-2));
                // Operator +, -.
                it = mx.begin1(I) + 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J+2));
                it = it - 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                it = 2 + it;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J+2));
                NSFX_TEST_EXPECT_EQ(it - mx.begin1(I), 2);
                // Operator <, >, <=, >=.
                NSFX_TEST_EXPECT(mx.begin1(I) < it);
                NSFX_TEST_EXPECT(it > mx.begin1(I));
                NSFX_TEST_EXPECT(mx.begin1(I) <= it);
                NSFX_TEST_EXPECT(it <= it);
                NSFX_TEST_EXPECT(it >= mx.begin1(I));
                NSFX_TEST_EXPECT(it >= it);
                // Operator +=, -=.
                it = mx.begin1(I);
                it += 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J+2));
                it -= 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Operator [].
                it = mx.begin1(I);
                NSFX_TEST_EXPECT_EQ(it[0], mx(I,J));
                NSFX_TEST_EXPECT_EQ(it[1], mx(I,J+1));
                NSFX_TEST_EXPECT_EQ(it[2], mx(I,J+2));
            }

            NSFX_TEST_CASE(ConstRowIterator)
            {
                MatrixType mx(4, 3);
                mx(I,   J) = 11; mx(I,   J+1) = 12; mx(I,   J+2) = 13;
                mx(I+1, J) = 21; mx(I+1, J+1) = 22; mx(I+1, J+2) = 23;
                mx(I+2, J) = 31; mx(I+2, J+1) = 32; mx(I+2, J+2) = 33;
                mx(I+3, J) = 41; mx(I+3, J+1) = 42; mx(I+3, J+2) = 43;
                // Copy-constructible.
                MatrixType::const_row_iterator it = mx.cbegin1(I);
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // NOTE: conversion from iterator.
                NSFX_TEST_EXPECT(mx.cbegin1(I) == MatrixType::const_row_iterator(mx.begin1(I)));
                NSFX_TEST_EXPECT(mx.cend1(I) == MatrixType::const_row_iterator(mx.end1(I)));
                // Copy-assignable.
                it = mx.cbegin1(I);
                // NOTE: copy-assign from iterator.
                it = mx.begin1(I);
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Increment.
                NSFX_TEST_EXPECT_EQ(*it++, mx(I,J));
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J+1));
                NSFX_TEST_EXPECT_EQ(*++it, mx(I,J+2));
                // Operator ==, !=.
                NSFX_TEST_EXPECT(it == it);
                NSFX_TEST_EXPECT(it != mx.cbegin1(I));
                // NOTE: iterator ? const_iterator.
                NSFX_TEST_EXPECT(mx.begin1(I) == mx.cbegin1(I));
                NSFX_TEST_EXPECT(mx.begin1(I) != mx.cend1(I));
                // Operator *, ->.
                // NOTE: readonly, primitive type has no operator->.
                it = mx.begin1(I);
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Default constructible.
                MatrixType::const_row_iterator it2;
                // Decrement.
                it = mx.end1(I);
                NSFX_TEST_EXPECT_EQ(*--it, mx(I,J+mx.size2()-1));
                NSFX_TEST_EXPECT_EQ(*it--, mx(I,J+mx.size2()-1));
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J+mx.size2()-2));
                // Operator +, -.
                it = mx.begin1(I) + 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J+2));
                it = it - 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                it = 2 + it;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J+2));
                NSFX_TEST_EXPECT_EQ(it - mx.begin1(I), 2);
                // NOTE: iterator - const_iterator.
                NSFX_TEST_EXPECT_EQ(mx.end1(I) - mx.cbegin1(I), mx.size2());
                // Operator <, >, <=, >=.
                NSFX_TEST_EXPECT(mx.cbegin1(I) < it);
                NSFX_TEST_EXPECT(it > mx.cbegin1(I));
                NSFX_TEST_EXPECT(mx.cbegin1(I) <= it);
                NSFX_TEST_EXPECT(it <= it);
                NSFX_TEST_EXPECT(it >= mx.cbegin1(I));
                NSFX_TEST_EXPECT(it >= it);
                // NOTE: iterator ? const_iterator.
                NSFX_TEST_EXPECT(mx.begin1(I) < mx.cend1(I));
                NSFX_TEST_EXPECT(mx.begin1(I) <= mx.cend1(I));
                NSFX_TEST_EXPECT(mx.end1(I) > mx.cbegin1(I));
                NSFX_TEST_EXPECT(mx.end1(I) >= mx.cbegin1(I));
                // Operator +=, -=.
                it = mx.begin1(I);
                it += 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J+2));
                it -= 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Operator [].
                it = mx.begin1(I);
                NSFX_TEST_EXPECT_EQ(it[0], mx(I,J));
                NSFX_TEST_EXPECT_EQ(it[1], mx(I,J+1));
                NSFX_TEST_EXPECT_EQ(it[2], mx(I,J+2));
            }

            NSFX_TEST_CASE(ColumnIterator)
            {
                MatrixType mx(4, 3);
                mx(I,   J) = 11; mx(I,   J+1) = 12; mx(I,   J+2) = 13;
                mx(I+1, J) = 21; mx(I+1, J+1) = 22; mx(I+1, J+2) = 23;
                mx(I+2, J) = 31; mx(I+2, J+1) = 32; mx(I+2, J+2) = 33;
                mx(I+3, J) = 41; mx(I+3, J+1) = 42; mx(I+3, J+2) = 43;
                // Copy-constructible.
                MatrixType::column_iterator it = mx.begin2(J);
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Copy-assignable.
                it = mx.begin2(J);
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Increment.
                NSFX_TEST_EXPECT_EQ(*it++, mx(I,J));
                NSFX_TEST_EXPECT_EQ(*it, mx(I+1,J));
                NSFX_TEST_EXPECT_EQ(*++it, mx(I+2,J));
                // Operator ==, !=.
                NSFX_TEST_EXPECT(it == it);
                NSFX_TEST_EXPECT(it != mx.begin2(J));
                // Operator *, ->.
                // NOTE: primitive type has no operator->.
                it = mx.begin2(J);
                *it = 1100;
                NSFX_TEST_EXPECT_EQ(mx(I,J), 1100);
                *it = 11;
                // Default constructible.
                MatrixType::column_iterator it2;
                // Decrement.
                it = mx.end2(J);
                NSFX_TEST_EXPECT_EQ(*--it, mx(I+mx.size1()-1,J));
                NSFX_TEST_EXPECT_EQ(*it--, mx(I+mx.size1()-1,J));
                NSFX_TEST_EXPECT_EQ(*it, mx(I+mx.size1()-2,J));
                // Operator +, -.
                it = mx.begin2(J) + 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I+2,J));
                it = it - 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                it = 2 + it;
                NSFX_TEST_EXPECT_EQ(*it, mx(I+2,J));
                NSFX_TEST_EXPECT_EQ(it - mx.begin2(J), 2);
                // Operator <, >, <=, >=.
                NSFX_TEST_EXPECT(mx.begin2(J) < it);
                NSFX_TEST_EXPECT(it > mx.begin2(J));
                NSFX_TEST_EXPECT(mx.begin2(J) <= it);
                NSFX_TEST_EXPECT(it <= it);
                NSFX_TEST_EXPECT(it >= mx.begin2(J));
                NSFX_TEST_EXPECT(it >= it);
                // Operator +=, -=.
                it = mx.begin2(J);
                it += 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I+2,J));
                it -= 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Operator [].
                it = mx.begin2(J);
                NSFX_TEST_EXPECT_EQ(it[0], mx(I,J));
                NSFX_TEST_EXPECT_EQ(it[1], mx(I+1,J));
                NSFX_TEST_EXPECT_EQ(it[2], mx(I+2,J));
            }

            NSFX_TEST_CASE(ConstColumnIterator)
            {
                MatrixType mx(4, 3);
                mx(I,   J) = 11; mx(I,   J+1) = 12; mx(I,   J+2) = 13;
                mx(I+1, J) = 21; mx(I+1, J+1) = 22; mx(I+1, J+2) = 23;
                mx(I+2, J) = 31; mx(I+2, J+1) = 32; mx(I+2, J+2) = 33;
                mx(I+3, J) = 41; mx(I+3, J+1) = 42; mx(I+3, J+2) = 43;
                // Copy-constructible.
                MatrixType::const_column_iterator it = mx.begin2(J);
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // NOTE: copy-construct from iterator.
                NSFX_TEST_EXPECT(mx.cbegin2(J) == MatrixType::const_column_iterator(mx.begin2(J)));
                NSFX_TEST_EXPECT(mx.cend2(J) == MatrixType::const_column_iterator(mx.end2(J)));
                // Copy-assignable.
                it = mx.cbegin2(J);
                // NOTE: copy-assign from iterator.
                it = mx.begin2(J);
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Increment.
                NSFX_TEST_EXPECT_EQ(*it++, mx(I,J));
                NSFX_TEST_EXPECT_EQ(*it, mx(I+1,J));
                NSFX_TEST_EXPECT_EQ(*++it, mx(I+2,J));
                // Operator ==, !=.
                NSFX_TEST_EXPECT(it == it);
                NSFX_TEST_EXPECT(it != mx.cbegin2(J));
                // NOTE: iterator ? const_iterator.
                NSFX_TEST_EXPECT(mx.begin2(J) == mx.cbegin2(J));
                NSFX_TEST_EXPECT(mx.begin2(J) != mx.cend2(J));
                // Operator *, ->.
                // NOTE: readonly, primitive type has no operator->.
                it = mx.cbegin2(J);
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Default constructible.
                MatrixType::column_iterator it2;
                // Decrement.
                it = mx.end2(J);
                NSFX_TEST_EXPECT_EQ(*--it, mx(I+mx.size1()-1,J));
                NSFX_TEST_EXPECT_EQ(*it--, mx(I+mx.size1()-1,J));
                NSFX_TEST_EXPECT_EQ(*it, mx(I+mx.size1()-2,J));
                // Operator +, -.
                it = mx.begin2(J) + 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I+2,J));
                it = it - 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                it = 2 + it;
                NSFX_TEST_EXPECT_EQ(*it, mx(I+2,J));
                NSFX_TEST_EXPECT_EQ(it - mx.begin2(J), 2);
                // NOTE: iterator - const_iterator.
                NSFX_TEST_EXPECT_EQ(mx.end2(J) - mx.cbegin2(J), mx.size1());
                // Operator <, >, <=, >=.
                NSFX_TEST_EXPECT(mx.cbegin2(J) < it);
                NSFX_TEST_EXPECT(it > mx.cbegin2(J));
                NSFX_TEST_EXPECT(mx.cbegin2(J) <= it);
                NSFX_TEST_EXPECT(it <= it);
                NSFX_TEST_EXPECT(it >= mx.cbegin2(J));
                NSFX_TEST_EXPECT(it >= it);
                // NOTE: iterator ? const_iterator.
                NSFX_TEST_EXPECT(mx.begin2(J) < mx.cend2(J));
                NSFX_TEST_EXPECT(mx.begin2(J) <= mx.cend2(J));
                NSFX_TEST_EXPECT(mx.end2(J) > mx.cbegin2(J));
                NSFX_TEST_EXPECT(mx.end2(J) >= mx.cbegin2(J));
                // Operator +=, -=.
                it = mx.begin2(J);
                it += 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I+2,J));
                it -= 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Operator [].
                it = mx.begin2(J);
                NSFX_TEST_EXPECT_EQ(it[0], mx(I,J));
                NSFX_TEST_EXPECT_EQ(it[1], mx(I+1,J));
                NSFX_TEST_EXPECT_EQ(it[2], mx(I+2,J));
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
            enum { I = 1, J = 2 };
            typedef nsfx::BiMatrix<A, I, J, nsfx::bi_matrix_row_major>
                    MatrixType;

            NSFX_TEST_CASE(Ctor0)
            {
                MatrixType mx;
                NSFX_TEST_EXPECT_EQ(mx.size(), 0);
                NSFX_TEST_EXPECT_EQ(mx.size1(), 0);
                NSFX_TEST_EXPECT_EQ(mx.size2(), 0);
                NSFX_TEST_EXPECT(mx.empty());
            }

            NSFX_TEST_CASE(Ctor1)
            {
                MatrixType mx(4, 3);
                NSFX_TEST_EXPECT_EQ(mx.size(), 12);
                NSFX_TEST_EXPECT_EQ(mx.size1(), 4);
                NSFX_TEST_EXPECT_EQ(mx.size2(), 3);
                NSFX_TEST_EXPECT(!mx.empty());
            }

            NSFX_TEST_CASE(Copy)
            {
                MatrixType mx(4, 3);
                mx(I,   J) = 11; mx(I,   J+1) = 12; mx(I,   J+2) = 13;
                mx(I+1, J) = 21; mx(I+1, J+1) = 22; mx(I+1, J+2) = 23;
                mx(I+2, J) = 31; mx(I+2, J+1) = 32; mx(I+2, J+2) = 33;
                mx(I+3, J) = 41; mx(I+3, J+1) = 42; mx(I+3, J+2) = 43;
                MatrixType mx2(mx);
                NSFX_TEST_EXPECT(mx2.size1() == 4);
                NSFX_TEST_EXPECT(mx2.size2() == 3);
                NSFX_TEST_EXPECT(mx2(I,J) == 11);
                NSFX_TEST_EXPECT(mx2(I+3,J+2) == 43);
                MatrixType mx3(1, 2);
                mx3 = mx2;
                NSFX_TEST_EXPECT(mx3.size1() == 4);
                NSFX_TEST_EXPECT(mx3.size2() == 3);
                NSFX_TEST_EXPECT(mx3(I,J) == 11);
                NSFX_TEST_EXPECT(mx3(I+3,J+2) == 43);
            }

            NSFX_TEST_CASE(Move)
            {
                MatrixType mx(4, 3);
                mx(I,   J) = 11; mx(I,   J+1) = 12; mx(I,   J+2) = 13;
                mx(I+1, J) = 21; mx(I+1, J+1) = 22; mx(I+1, J+2) = 23;
                mx(I+2, J) = 31; mx(I+2, J+1) = 32; mx(I+2, J+2) = 33;
                mx(I+3, J) = 41; mx(I+3, J+1) = 42; mx(I+3, J+2) = 43;
                MatrixType mx2(std::move(mx));
                NSFX_TEST_EXPECT(mx.size1() == 0);
                NSFX_TEST_EXPECT(mx.size2() == 0);
                NSFX_TEST_EXPECT(mx2.size1() == 4);
                NSFX_TEST_EXPECT(mx2.size2() == 3);
                NSFX_TEST_EXPECT(mx2(I,J) == 11);
                NSFX_TEST_EXPECT(mx2(I+3,J+2) == 43);
                MatrixType mx3(1, 2);
                mx3 = std::move(mx2);
                NSFX_TEST_EXPECT(mx2.size1() == 0);
                NSFX_TEST_EXPECT(mx2.size2() == 0);
                NSFX_TEST_EXPECT(mx3.size1() == 4);
                NSFX_TEST_EXPECT(mx3.size2() == 3);
                NSFX_TEST_EXPECT(mx3(I,J) == 11);
                NSFX_TEST_EXPECT(mx3(I+3,J+2) == 43);
            }

            NSFX_TEST_CASE(InitialValue)
            {
                MatrixType mx(4, 3);
                for (size_t i = I; i < I + mx.size1(); ++i)
                for (size_t j = J; j < J + mx.size2(); ++j)
                {
                    NSFX_TEST_EXPECT(mx.at(i,j) == 1);
                    NSFX_TEST_EXPECT(mx(i,j) == 1);
                }
                for (size_t i = 0; i < mx.size(); ++i)
                {
                    NSFX_TEST_EXPECT(mx(i) == 1);
                    NSFX_TEST_EXPECT(mx[i] == 1);
                }
                const MatrixType& cmx = mx;
                for (size_t i = I; i < I + mx.size1(); ++i)
                for (size_t j = J; j < J + mx.size2(); ++j)
                {
                    NSFX_TEST_EXPECT(cmx.at(i,j) == 1);
                    NSFX_TEST_EXPECT(cmx(i,j) == 1);
                }
                for (size_t i = 0; i < mx.size(); ++i)
                {
                    NSFX_TEST_EXPECT(cmx(i) == 1);
                    NSFX_TEST_EXPECT(cmx[i] == 1);
                }
            }

        }/*}}}*/

    }

    NSFX_TEST_SUITE(ColumnMajor)
    {
        NSFX_TEST_SUITE(Primitive)/*{{{*/
        {
            enum { I = 1, J = 2 };
            typedef nsfx::BiMatrix<int, I, J, nsfx::bi_matrix_column_major>
                    MatrixType;

            NSFX_TEST_CASE(Ctor0)
            {
                MatrixType mx;
                NSFX_TEST_EXPECT_EQ(mx.size(), 0);
                NSFX_TEST_EXPECT_EQ(mx.size1(), 0);
                NSFX_TEST_EXPECT_EQ(mx.size2(), 0);
                NSFX_TEST_EXPECT(mx.empty());
            }

            NSFX_TEST_CASE(Ctor1)
            {
                MatrixType mx(4, 3);
                NSFX_TEST_EXPECT_EQ(mx.size(), 12);
                NSFX_TEST_EXPECT_EQ(mx.size1(), 4);
                NSFX_TEST_EXPECT_EQ(mx.size2(), 3);
                NSFX_TEST_EXPECT(!mx.empty());
            }

            NSFX_TEST_CASE(InitialValue)
            {
                MatrixType mx(4, 3);
                for (size_t i = I; i < I + mx.size1(); ++i)
                for (size_t j = J; j < J + mx.size2(); ++j)
                {
                    NSFX_TEST_EXPECT_EQ(mx.at(i,j), 0);
                    NSFX_TEST_EXPECT_EQ(mx(i,j), 0);
                }
                for (size_t i = 0; i < mx.size(); ++i)
                {
                    NSFX_TEST_EXPECT_EQ(mx(i), 0);
                    NSFX_TEST_EXPECT_EQ(mx[i], 0);
                }
                const MatrixType& cmx = mx;
                for (size_t i = I; i < I + mx.size1(); ++i)
                for (size_t j = J; j < J + mx.size2(); ++j)
                {
                    NSFX_TEST_EXPECT_EQ(cmx.at(i,j), 0);
                    NSFX_TEST_EXPECT_EQ(cmx(i,j), 0);
                }
                for (size_t i = 0; i < mx.size(); ++i)
                {
                    NSFX_TEST_EXPECT_EQ(cmx(i), 0);
                    NSFX_TEST_EXPECT_EQ(cmx[i], 0);
                }
            }

            NSFX_TEST_CASE(FrontBack)
            {
                MatrixType mx(4, 3);
                mx(I,   J) = 11; mx(I,   J+1) = 12; mx(I,   J+2) = 13;
                mx(I+1, J) = 21; mx(I+1, J+1) = 22; mx(I+1, J+2) = 23;
                mx(I+2, J) = 31; mx(I+2, J+1) = 32; mx(I+2, J+2) = 33;
                mx(I+3, J) = 41; mx(I+3, J+1) = 42; mx(I+3, J+2) = 43;
                NSFX_TEST_EXPECT_EQ(mx.front(), 11);
                NSFX_TEST_EXPECT_EQ(mx.back(), 43);
                const MatrixType& cmx = mx;
                NSFX_TEST_EXPECT_EQ(cmx.front(), 11);
                NSFX_TEST_EXPECT_EQ(cmx.back(), 43);
            }

            NSFX_TEST_CASE(Data)
            {
                MatrixType mx(4, 3);
                mx(I,   J) = 11; mx(I,   J+1) = 12; mx(I,   J+2) = 13;
                mx(I+1, J) = 21; mx(I+1, J+1) = 22; mx(I+1, J+2) = 23;
                mx(I+2, J) = 31; mx(I+2, J+1) = 32; mx(I+2, J+2) = 33;
                mx(I+3, J) = 41; mx(I+3, J+1) = 42; mx(I+3, J+2) = 43;
                MatrixType::pointer p = mx.data();
                // XXX: Column major.
                NSFX_TEST_EXPECT_EQ(*p++, 11);
                NSFX_TEST_EXPECT_EQ(*p++, 21);
                NSFX_TEST_EXPECT_EQ(*p++, 31);
                NSFX_TEST_EXPECT_EQ(*p++, 41);
                NSFX_TEST_EXPECT_EQ(*p++, 12);
                NSFX_TEST_EXPECT_EQ(*p++, 22);
                NSFX_TEST_EXPECT_EQ(*p++, 32);
                NSFX_TEST_EXPECT_EQ(*p++, 42);
                NSFX_TEST_EXPECT_EQ(*p++, 13);
                NSFX_TEST_EXPECT_EQ(*p++, 23);
                NSFX_TEST_EXPECT_EQ(*p++, 33);
                NSFX_TEST_EXPECT_EQ(*p++, 43);
                const MatrixType& cmx = mx;
                MatrixType::const_pointer cp = cmx.data();
                NSFX_TEST_EXPECT_EQ(*cp++, 11);
                NSFX_TEST_EXPECT_EQ(*cp++, 21);
                NSFX_TEST_EXPECT_EQ(*cp++, 31);
                NSFX_TEST_EXPECT_EQ(*cp++, 41);
                NSFX_TEST_EXPECT_EQ(*cp++, 12);
                NSFX_TEST_EXPECT_EQ(*cp++, 22);
                NSFX_TEST_EXPECT_EQ(*cp++, 32);
                NSFX_TEST_EXPECT_EQ(*cp++, 42);
                NSFX_TEST_EXPECT_EQ(*cp++, 13);
                NSFX_TEST_EXPECT_EQ(*cp++, 23);
                NSFX_TEST_EXPECT_EQ(*cp++, 33);
                NSFX_TEST_EXPECT_EQ(*cp++, 43);
            }

            NSFX_TEST_CASE(Fill)
            {
                MatrixType mx(4, 3);
                mx.fill(0x12345678);
                for (size_t i = 0; i < mx.size(); ++i)
                {
                    NSFX_TEST_EXPECT_EQ(mx(i), 0x12345678);
                }
                mx.fill1(I, 0x9abcdef0);
                for (size_t j = J; j < J + mx.size2(); ++j)
                {
                    NSFX_TEST_EXPECT_EQ(mx(I,j), 0x9abcdef0);
                }
                mx.fill2(J, 0x2468ace0);
                for (size_t i = I; i < I + mx.size1(); ++i)
                {
                    NSFX_TEST_EXPECT_EQ(mx(i,J), 0x2468ace0);
                }
            }

            NSFX_TEST_CASE(Swap)
            {
                MatrixType mx1(4, 3);
                mx1.fill(0x12345678);
                MatrixType mx2(4, 3);
                mx2.fill(0x9abcdef0);
                boost::swap(mx1, mx2);
                NSFX_TEST_EXPECT_EQ(mx1(0), 0x9abcdef0);
                NSFX_TEST_EXPECT_EQ(mx2(0), 0x12345678);
            }

            NSFX_TEST_CASE(Iterator)
            {
                MatrixType mx(4, 3);
                mx(I,   J) = 11; mx(I,   J+1) = 12; mx(I,   J+2) = 13;
                mx(I+1, J) = 21; mx(I+1, J+1) = 22; mx(I+1, J+2) = 23;
                mx(I+2, J) = 31; mx(I+2, J+1) = 32; mx(I+2, J+2) = 33;
                mx(I+3, J) = 41; mx(I+3, J+1) = 42; mx(I+3, J+2) = 43;
                // Copy-constructible.
                MatrixType::iterator it = mx.begin();
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Copy-assignable.
                it = mx.begin();
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Increment.
                NSFX_TEST_EXPECT_EQ(*it++, mx(I,J));
                NSFX_TEST_EXPECT_EQ(*it, mx(I+1,J)); // XXX
                NSFX_TEST_EXPECT_EQ(*++it, mx(I+2,J)); // XXX
                // Operator ==, !=.
                NSFX_TEST_EXPECT(it == it);
                NSFX_TEST_EXPECT(it != mx.begin());
                // Operator *, ->.
                // NOTE: primitive type has no operator->.
                it = mx.begin();
                *it = 1100;
                NSFX_TEST_EXPECT_EQ(mx(I,J), 1100);
                *it = 11;
                // Default constructible.
                MatrixType::iterator it2;
                // Decrement.
                it = mx.end();
                NSFX_TEST_EXPECT_EQ(*--it, mx(I+mx.size1()-1,J+mx.size2()-1));
                NSFX_TEST_EXPECT_EQ(*it--, mx(I+mx.size1()-1,J+mx.size2()-1));
                NSFX_TEST_EXPECT_EQ(*it, mx(I+mx.size1()-2,J+mx.size2()-1)); // XXX
                // Operator +, -.
                it = mx.begin() + 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I+2,J)); // XXX
                it = it - 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                it = 2 + it;
                NSFX_TEST_EXPECT_EQ(*it, mx(I+2,J)); // XXX
                NSFX_TEST_EXPECT_EQ(it - mx.begin(), 2);
                // Operator <, >, <=, >=.
                NSFX_TEST_EXPECT(mx.begin() < it);
                NSFX_TEST_EXPECT(it > mx.begin());
                NSFX_TEST_EXPECT(mx.begin() <= it);
                NSFX_TEST_EXPECT(it <= it);
                NSFX_TEST_EXPECT(it >= mx.begin());
                NSFX_TEST_EXPECT(it >= it);
                // Operator +=, -=.
                it = mx.begin();
                it += 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I+2,J)); // XXX
                it -= 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Operator [].
                it = mx.begin();
                NSFX_TEST_EXPECT_EQ(it[0], mx(I,J));
                NSFX_TEST_EXPECT_EQ(it[1], mx(I+1,J)); // XXX
                NSFX_TEST_EXPECT_EQ(it[2], mx(I+2,J)); // XXX
            }

            NSFX_TEST_CASE(ConstIterator)
            {
                MatrixType mx(4, 3);
                mx(I,   J) = 11; mx(I,   J+1) = 12; mx(I,   J+2) = 13;
                mx(I+1, J) = 21; mx(I+1, J+1) = 22; mx(I+1, J+2) = 23;
                mx(I+2, J) = 31; mx(I+2, J+1) = 32; mx(I+2, J+2) = 33;
                mx(I+3, J) = 41; mx(I+3, J+1) = 42; mx(I+3, J+2) = 43;
                // Copy-constructible.
                MatrixType::const_iterator it = mx.cbegin();
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // NOTE: copy-construct from iterator.
                NSFX_TEST_EXPECT(mx.cbegin() == MatrixType::const_iterator(mx.begin()));
                NSFX_TEST_EXPECT(mx.cend() == MatrixType::const_iterator(mx.end()));
                // Copy-assignable.
                it = mx.cbegin();
                // NOTE: copy-assign from iterator.
                it = mx.begin();
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Increment.
                NSFX_TEST_EXPECT_EQ(*it++, mx(I,J));
                NSFX_TEST_EXPECT_EQ(*it, mx(I+1,J));
                NSFX_TEST_EXPECT_EQ(*++it, mx(I+2,J));
                // Operator ==, !=.
                NSFX_TEST_EXPECT(it == it);
                NSFX_TEST_EXPECT(it != mx.cbegin());
                // NOTE: iterator ? const_iterator.
                NSFX_TEST_EXPECT(mx.begin() == mx.cbegin());
                NSFX_TEST_EXPECT(mx.begin() != mx.cend());
                // Operator *, ->.
                // NOTE: readonly, primitive type has no operator->.
                it = mx.cbegin();
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Default constructible.
                MatrixType::const_iterator it2;
                // Decrement.
                it = mx.cend();
                NSFX_TEST_EXPECT_EQ(*--it, mx(I+mx.size1()-1,J+mx.size2()-1));
                NSFX_TEST_EXPECT_EQ(*it--, mx(I+mx.size1()-1,J+mx.size2()-1));
                NSFX_TEST_EXPECT_EQ(*it, mx(I+mx.size1()-2,J+mx.size2()-1)); // XXX
                // Operator +, -.
                it = mx.cbegin() + 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I+2,J)); // XXX
                it = it - 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                it = 2 + it;
                NSFX_TEST_EXPECT_EQ(*it, mx(I+2,J)); // XXX
                NSFX_TEST_EXPECT_EQ(it - mx.cbegin(), 2);
                // NOTE: iterator - const_iterator.
                NSFX_TEST_EXPECT_EQ(mx.end() - mx.cbegin(), mx.size());
                // Operator <, >, <=, >=.
                NSFX_TEST_EXPECT(mx.cbegin() < it);
                NSFX_TEST_EXPECT(it > mx.cbegin());
                NSFX_TEST_EXPECT(mx.cbegin() <= it);
                NSFX_TEST_EXPECT(it <= it);
                NSFX_TEST_EXPECT(it >= mx.cbegin());
                NSFX_TEST_EXPECT(it >= it);
                // NOTE: iterator ? const_iterator.
                NSFX_TEST_EXPECT(mx.begin() < mx.cend());
                NSFX_TEST_EXPECT(mx.begin() <= mx.cend());
                NSFX_TEST_EXPECT(mx.end() > mx.cbegin());
                NSFX_TEST_EXPECT(mx.end() >= mx.cbegin());
                // Operator +=, -=.
                it = mx.cbegin();
                it += 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I+2,J)); // XXX
                it -= 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Operator [].
                it = mx.cbegin();
                NSFX_TEST_EXPECT_EQ(it[0], mx(I,J));
                NSFX_TEST_EXPECT_EQ(it[1], mx(I+1,J)); // XXX
                NSFX_TEST_EXPECT_EQ(it[2], mx(I+2,J)); // XXX
            }

            NSFX_TEST_CASE(RowIterator)
            {
                MatrixType mx(4, 3);
                mx(I,   J) = 11; mx(I,   J+1) = 12; mx(I,   J+2) = 13;
                mx(I+1, J) = 21; mx(I+1, J+1) = 22; mx(I+1, J+2) = 23;
                mx(I+2, J) = 31; mx(I+2, J+1) = 32; mx(I+2, J+2) = 33;
                mx(I+3, J) = 41; mx(I+3, J+1) = 42; mx(I+3, J+2) = 43;
                // Copy-constructible.
                MatrixType::row_iterator it = mx.begin1(I);
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Copy-assignable.
                it = mx.begin1(I);
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Increment.
                NSFX_TEST_EXPECT_EQ(*it++, mx(I,J));
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J+1));
                NSFX_TEST_EXPECT_EQ(*++it, mx(I,J+2));
                // Operator ==, !=.
                NSFX_TEST_EXPECT(it == it);
                NSFX_TEST_EXPECT(it != mx.begin1(I));
                // Operator *, ->.
                // NOTE: primitive type has no operator->.
                it = mx.begin1(I);
                *it = 1100;
                NSFX_TEST_EXPECT_EQ(mx(I,J), 1100);
                *it = 11;
                // Default constructible.
                MatrixType::row_iterator it2;
                // Decrement.
                it = mx.end1(I);
                NSFX_TEST_EXPECT_EQ(*--it, mx(I,J+mx.size2()-1));
                NSFX_TEST_EXPECT_EQ(*it--, mx(I,J+mx.size2()-1));
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J+mx.size2()-2));
                // Operator +, -.
                it = mx.begin1(I) + 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J+2));
                it = it - 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                it = 2 + it;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J+2));
                NSFX_TEST_EXPECT_EQ(it - mx.begin1(I), 2);
                // Operator <, >, <=, >=.
                NSFX_TEST_EXPECT(mx.begin1(I) < it);
                NSFX_TEST_EXPECT(it > mx.begin1(I));
                NSFX_TEST_EXPECT(mx.begin1(I) <= it);
                NSFX_TEST_EXPECT(it <= it);
                NSFX_TEST_EXPECT(it >= mx.begin1(I));
                NSFX_TEST_EXPECT(it >= it);
                // Operator +=, -=.
                it = mx.begin1(I);
                it += 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J+2));
                it -= 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Operator [].
                it = mx.begin1(I);
                NSFX_TEST_EXPECT_EQ(it[0], mx(I,J));
                NSFX_TEST_EXPECT_EQ(it[1], mx(I,J+1));
                NSFX_TEST_EXPECT_EQ(it[2], mx(I,J+2));
            }

            NSFX_TEST_CASE(ConstRowIterator)
            {
                MatrixType mx(4, 3);
                mx(I,   J) = 11; mx(I,   J+1) = 12; mx(I,   J+2) = 13;
                mx(I+1, J) = 21; mx(I+1, J+1) = 22; mx(I+1, J+2) = 23;
                mx(I+2, J) = 31; mx(I+2, J+1) = 32; mx(I+2, J+2) = 33;
                mx(I+3, J) = 41; mx(I+3, J+1) = 42; mx(I+3, J+2) = 43;
                // Copy-constructible.
                MatrixType::const_row_iterator it = mx.cbegin1(I);
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // NOTE: conversion from iterator.
                NSFX_TEST_EXPECT(mx.cbegin1(I) == MatrixType::const_row_iterator(mx.begin1(I)));
                NSFX_TEST_EXPECT(mx.cend1(I) == MatrixType::const_row_iterator(mx.end1(I)));
                // Copy-assignable.
                it = mx.cbegin1(I);
                // NOTE: copy-assign from iterator.
                it = mx.begin1(I);
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Increment.
                NSFX_TEST_EXPECT_EQ(*it++, mx(I,J));
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J+1));
                NSFX_TEST_EXPECT_EQ(*++it, mx(I,J+2));
                // Operator ==, !=.
                NSFX_TEST_EXPECT(it == it);
                NSFX_TEST_EXPECT(it != mx.cbegin1(I));
                // NOTE: iterator ? const_iterator.
                NSFX_TEST_EXPECT(mx.begin1(I) == mx.cbegin1(I));
                NSFX_TEST_EXPECT(mx.begin1(I) != mx.cend1(I));
                // Operator *, ->.
                // NOTE: readonly, primitive type has no operator->.
                it = mx.begin1(I);
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Default constructible.
                MatrixType::const_row_iterator it2;
                // Decrement.
                it = mx.end1(I);
                NSFX_TEST_EXPECT_EQ(*--it, mx(I,J+mx.size2()-1));
                NSFX_TEST_EXPECT_EQ(*it--, mx(I,J+mx.size2()-1));
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J+mx.size2()-2));
                // Operator +, -.
                it = mx.begin1(I) + 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J+2));
                it = it - 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                it = 2 + it;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J+2));
                NSFX_TEST_EXPECT_EQ(it - mx.begin1(I), 2);
                // NOTE: iterator - const_iterator.
                NSFX_TEST_EXPECT_EQ(mx.end1(I) - mx.cbegin1(I), mx.size2());
                // Operator <, >, <=, >=.
                NSFX_TEST_EXPECT(mx.cbegin1(I) < it);
                NSFX_TEST_EXPECT(it > mx.cbegin1(I));
                NSFX_TEST_EXPECT(mx.cbegin1(I) <= it);
                NSFX_TEST_EXPECT(it <= it);
                NSFX_TEST_EXPECT(it >= mx.cbegin1(I));
                NSFX_TEST_EXPECT(it >= it);
                // NOTE: iterator ? const_iterator.
                NSFX_TEST_EXPECT(mx.begin1(I) < mx.cend1(I));
                NSFX_TEST_EXPECT(mx.begin1(I) <= mx.cend1(I));
                NSFX_TEST_EXPECT(mx.end1(I) > mx.cbegin1(I));
                NSFX_TEST_EXPECT(mx.end1(I) >= mx.cbegin1(I));
                // Operator +=, -=.
                it = mx.begin1(I);
                it += 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J+2));
                it -= 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Operator [].
                it = mx.begin1(I);
                NSFX_TEST_EXPECT_EQ(it[0], mx(I,J));
                NSFX_TEST_EXPECT_EQ(it[1], mx(I,J+1));
                NSFX_TEST_EXPECT_EQ(it[2], mx(I,J+2));
            }

            NSFX_TEST_CASE(ColumnIterator)
            {
                MatrixType mx(4, 3);
                mx(I,   J) = 11; mx(I,   J+1) = 12; mx(I,   J+2) = 13;
                mx(I+1, J) = 21; mx(I+1, J+1) = 22; mx(I+1, J+2) = 23;
                mx(I+2, J) = 31; mx(I+2, J+1) = 32; mx(I+2, J+2) = 33;
                mx(I+3, J) = 41; mx(I+3, J+1) = 42; mx(I+3, J+2) = 43;
                // Copy-constructible.
                MatrixType::column_iterator it = mx.begin2(J);
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Copy-assignable.
                it = mx.begin2(J);
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Increment.
                NSFX_TEST_EXPECT_EQ(*it++, mx(I,J));
                NSFX_TEST_EXPECT_EQ(*it, mx(I+1,J));
                NSFX_TEST_EXPECT_EQ(*++it, mx(I+2,J));
                // Operator ==, !=.
                NSFX_TEST_EXPECT(it == it);
                NSFX_TEST_EXPECT(it != mx.begin2(J));
                // Operator *, ->.
                // NOTE: primitive type has no operator->.
                it = mx.begin2(J);
                *it = 1100;
                NSFX_TEST_EXPECT_EQ(mx(I,J), 1100);
                *it = 11;
                // Default constructible.
                MatrixType::column_iterator it2;
                // Decrement.
                it = mx.end2(J);
                NSFX_TEST_EXPECT_EQ(*--it, mx(I+mx.size1()-1,J));
                NSFX_TEST_EXPECT_EQ(*it--, mx(I+mx.size1()-1,J));
                NSFX_TEST_EXPECT_EQ(*it, mx(I+mx.size1()-2,J));
                // Operator +, -.
                it = mx.begin2(J) + 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I+2,J));
                it = it - 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                it = 2 + it;
                NSFX_TEST_EXPECT_EQ(*it, mx(I+2,J));
                NSFX_TEST_EXPECT_EQ(it - mx.begin2(J), 2);
                // Operator <, >, <=, >=.
                NSFX_TEST_EXPECT(mx.begin2(J) < it);
                NSFX_TEST_EXPECT(it > mx.begin2(J));
                NSFX_TEST_EXPECT(mx.begin2(J) <= it);
                NSFX_TEST_EXPECT(it <= it);
                NSFX_TEST_EXPECT(it >= mx.begin2(J));
                NSFX_TEST_EXPECT(it >= it);
                // Operator +=, -=.
                it = mx.begin2(J);
                it += 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I+2,J));
                it -= 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Operator [].
                it = mx.begin2(J);
                NSFX_TEST_EXPECT_EQ(it[0], mx(I,J));
                NSFX_TEST_EXPECT_EQ(it[1], mx(I+1,J));
                NSFX_TEST_EXPECT_EQ(it[2], mx(I+2,J));
            }

            NSFX_TEST_CASE(ConstColumnIterator)
            {
                MatrixType mx(4, 3);
                mx(I,   J) = 11; mx(I,   J+1) = 12; mx(I,   J+2) = 13;
                mx(I+1, J) = 21; mx(I+1, J+1) = 22; mx(I+1, J+2) = 23;
                mx(I+2, J) = 31; mx(I+2, J+1) = 32; mx(I+2, J+2) = 33;
                mx(I+3, J) = 41; mx(I+3, J+1) = 42; mx(I+3, J+2) = 43;
                // Copy-constructible.
                MatrixType::const_column_iterator it = mx.begin2(J);
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // NOTE: copy-construct from iterator.
                NSFX_TEST_EXPECT(mx.cbegin2(J) == MatrixType::const_column_iterator(mx.begin2(J)));
                NSFX_TEST_EXPECT(mx.cend2(J) == MatrixType::const_column_iterator(mx.end2(J)));
                // Copy-assignable.
                it = mx.cbegin2(J);
                // NOTE: copy-assign from iterator.
                it = mx.begin2(J);
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Increment.
                NSFX_TEST_EXPECT_EQ(*it++, mx(I,J));
                NSFX_TEST_EXPECT_EQ(*it, mx(I+1,J));
                NSFX_TEST_EXPECT_EQ(*++it, mx(I+2,J));
                // Operator ==, !=.
                NSFX_TEST_EXPECT(it == it);
                NSFX_TEST_EXPECT(it != mx.cbegin2(J));
                // NOTE: iterator ? const_iterator.
                NSFX_TEST_EXPECT(mx.begin2(J) == mx.cbegin2(J));
                NSFX_TEST_EXPECT(mx.begin2(J) != mx.cend2(J));
                // Operator *, ->.
                // NOTE: readonly, primitive type has no operator->.
                it = mx.cbegin2(J);
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Default constructible.
                MatrixType::column_iterator it2;
                // Decrement.
                it = mx.end2(J);
                NSFX_TEST_EXPECT_EQ(*--it, mx(I+mx.size1()-1,J));
                NSFX_TEST_EXPECT_EQ(*it--, mx(I+mx.size1()-1,J));
                NSFX_TEST_EXPECT_EQ(*it, mx(I+mx.size1()-2,J));
                // Operator +, -.
                it = mx.begin2(J) + 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I+2,J));
                it = it - 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                it = 2 + it;
                NSFX_TEST_EXPECT_EQ(*it, mx(I+2,J));
                NSFX_TEST_EXPECT_EQ(it - mx.begin2(J), 2);
                // NOTE: iterator - const_iterator.
                NSFX_TEST_EXPECT_EQ(mx.end2(J) - mx.cbegin2(J), mx.size1());
                // Operator <, >, <=, >=.
                NSFX_TEST_EXPECT(mx.cbegin2(J) < it);
                NSFX_TEST_EXPECT(it > mx.cbegin2(J));
                NSFX_TEST_EXPECT(mx.cbegin2(J) <= it);
                NSFX_TEST_EXPECT(it <= it);
                NSFX_TEST_EXPECT(it >= mx.cbegin2(J));
                NSFX_TEST_EXPECT(it >= it);
                // NOTE: iterator ? const_iterator.
                NSFX_TEST_EXPECT(mx.begin2(J) < mx.cend2(J));
                NSFX_TEST_EXPECT(mx.begin2(J) <= mx.cend2(J));
                NSFX_TEST_EXPECT(mx.end2(J) > mx.cbegin2(J));
                NSFX_TEST_EXPECT(mx.end2(J) >= mx.cbegin2(J));
                // Operator +=, -=.
                it = mx.begin2(J);
                it += 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I+2,J));
                it -= 2;
                NSFX_TEST_EXPECT_EQ(*it, mx(I,J));
                // Operator [].
                it = mx.begin2(J);
                NSFX_TEST_EXPECT_EQ(it[0], mx(I,J));
                NSFX_TEST_EXPECT_EQ(it[1], mx(I+1,J));
                NSFX_TEST_EXPECT_EQ(it[2], mx(I+2,J));
            }
        }/*}}}*/

        NSFX_TEST_SUITE(Class)/*{{{*/
        {
            struct A {
                A(void) : m_(1) {};
                A(int m) : m_(m) {};
                bool operator==(const A& rhs) const { return m_ == rhs.m_; }
                bool operator==(int rhs) const { return m_ == rhs; }
                int m_;
            };
            enum { I = 1, J = 2 };
            typedef nsfx::BiMatrix<A, I, J, nsfx::bi_matrix_column_major>
                    MatrixType;

            NSFX_TEST_CASE(Ctor0)
            {
                MatrixType mx;
                NSFX_TEST_EXPECT_EQ(mx.size(), 0);
                NSFX_TEST_EXPECT_EQ(mx.size1(), 0);
                NSFX_TEST_EXPECT_EQ(mx.size2(), 0);
                NSFX_TEST_EXPECT(mx.empty());
            }

            NSFX_TEST_CASE(Ctor1)
            {
                MatrixType mx(4, 3);
                NSFX_TEST_EXPECT_EQ(mx.size(), 12);
                NSFX_TEST_EXPECT_EQ(mx.size1(), 4);
                NSFX_TEST_EXPECT_EQ(mx.size2(), 3);
                NSFX_TEST_EXPECT(!mx.empty());
            }

            NSFX_TEST_CASE(InitialValue)
            {
                MatrixType mx(4, 3);
                for (size_t i = I; i < I + mx.size1(); ++i)
                for (size_t j = J; j < J + mx.size2(); ++j)
                {
                    NSFX_TEST_EXPECT(mx.at(i,j) == 1);
                    NSFX_TEST_EXPECT(mx(i,j) == 1);
                }
                for (size_t i = 0; i < mx.size(); ++i)
                {
                    NSFX_TEST_EXPECT(mx(i) == 1);
                    NSFX_TEST_EXPECT(mx[i] == 1);
                }
                const MatrixType& cmx = mx;
                for (size_t i = I; i < I + mx.size1(); ++i)
                for (size_t j = J; j < J + mx.size2(); ++j)
                {
                    NSFX_TEST_EXPECT(cmx.at(i,j) == 1);
                    NSFX_TEST_EXPECT(cmx(i,j) == 1);
                }
                for (size_t i = 0; i < mx.size(); ++i)
                {
                    NSFX_TEST_EXPECT(cmx(i) == 1);
                    NSFX_TEST_EXPECT(cmx[i] == 1);
                }
            }

        }/*}}}*/

    }

}


int main(void)
{
    nsfx::test::runner::GetLogger()->AddStreamSink(std::cerr);
    nsfx::test::runner::Run();

    return 0;
}

