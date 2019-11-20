/**
 * @file
 *
 * @brief Matrix with base indices.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2019-11-17
 *
 * @copyright Copyright (c) 2019.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef BI_MATRIX_H__B32755F1_A11E_47D0_A318_C215FDDFD5A0
#define BI_MATRIX_H__B32755F1_A11E_47D0_A318_C215FDDFD5A0


#include <nsfx/config.h>
#include <boost/core/swap.hpp>
#include <type_traits> // is_pod
#include <cstring> // memset
#include <iterator>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Utility
 * @brief The storage layout of a `bi_matrix`.
 */
enum bi_matrix_layout_t
{
    bi_matrix_row_major,    ///< Each row is stored in contiguous memory.
    bi_matrix_column_major, ///< Each column is stored in contiguous memory.
};


////////////////////////////////////////////////////////////////////////////////
namespace detail /*{{{*/
{

/**
 * @ingroup Utility
 * @brief A matrix with base indices.
 *
 * @tparam T The type of the element.
 * @tparam I The index of the first row.
 *           Defaults to `0`.
 * @tparam J The index of the first column.
 *           Defaults to `I`.
 * @tparam S The storage layout.
 *           Can be `bi_matrix_row_major` or `bi_matrix_column_major`.
 *           Defaults to `bi_matrix_row_major`.
 */
template<class T, size_t I = 0, size_t J = I,
         bi_matrix_layout_t S = bi_matrix_row_major>
struct bi_matrix
{
    size_t size1_; ///< The number of rows.
    size_t size2_; ///< The number of columns.
    T*     data_;
};

////////////////////////////////////////
// Row iterators.
template<class T, size_t I, size_t J, bi_matrix_layout_t S>
struct bi_matrix_row_iterator;

template<class T, size_t I, size_t J>
struct bi_matrix_row_iterator<T, I, J, bi_matrix_row_major>
{
    T* item_;
};

template<class T, size_t I, size_t J>
struct bi_matrix_row_iterator<T, I, J, bi_matrix_column_major>
{
    T* item_;
    size_t size1_;
};

////////////////////
// row major
template<class T, size_t I, size_t J>
inline bi_matrix_row_iterator<T, I, J, bi_matrix_row_major>
bi_matrix_row_iterator_inc(
    bi_matrix_row_iterator<T, I, J, bi_matrix_row_major> it)
{
    bi_matrix_row_iterator<T, I, J, bi_matrix_row_major> nx =
        { it.item_ + 1 };
    return nx;
}

template<class T, size_t I, size_t J>
inline bi_matrix_row_iterator<T, I, J, bi_matrix_row_major>
bi_matrix_row_iterator_dec(
    bi_matrix_row_iterator<T, I, J, bi_matrix_row_major> it)
{
    bi_matrix_row_iterator<T, I, J, bi_matrix_row_major> nx =
        { it.item_ - 1 };
    return nx;
}

template<class T, size_t I, size_t J>
inline bi_matrix_row_iterator<T, I, J, bi_matrix_row_major>
bi_matrix_row_iterator_plus(
    bi_matrix_row_iterator<T, I, J, bi_matrix_row_major> it, ptrdiff_t n)
{
    bi_matrix_row_iterator<T, I, J, bi_matrix_row_major> nx =
        { it.item_ + n };
    return nx;
}

template<class T, size_t I, size_t J>
inline bi_matrix_row_iterator<T, I, J, bi_matrix_row_major>
bi_matrix_row_iterator_minus(
    bi_matrix_row_iterator<T, I, J, bi_matrix_row_major> it, ptrdiff_t n)
{
    bi_matrix_row_iterator<T, I, J, bi_matrix_row_major> nx =
        { it.item_ - n };
    return nx;
}

////////////////////
// column major
template<class T, size_t I, size_t J>
inline bi_matrix_row_iterator<T, I, J, bi_matrix_column_major>
bi_matrix_row_iterator_inc(
    bi_matrix_row_iterator<T, I, J, bi_matrix_column_major> it)
{
    bi_matrix_row_iterator<T, I, J, bi_matrix_column_major> nx =
        { it.item_ + it.size1_ };
    return nx;
}

template<class T, size_t I, size_t J>
inline bi_matrix_row_iterator<T, I, J, bi_matrix_column_major>
bi_matrix_row_iterator_dec(
    bi_matrix_row_iterator<T, I, J, bi_matrix_column_major> it)
{
    bi_matrix_row_iterator<T, I, J, bi_matrix_column_major> nx =
        { it.item_ - it.size1_ };
    return nx;
}

template<class T, size_t I, size_t J>
inline bi_matrix_row_iterator<T, I, J, bi_matrix_column_major>
bi_matrix_row_iterator_plus(
    bi_matrix_row_iterator<T, I, J, bi_matrix_column_major> it, ptrdiff_t n)
{
    bi_matrix_row_iterator<T, I, J, bi_matrix_column_major> nx =
        { it.item_ + n * it.size1_ };
    return nx;
}

template<class T, size_t I, size_t J>
inline bi_matrix_row_iterator<T, I, J, bi_matrix_column_major>
bi_matrix_row_iterator_minus(
    bi_matrix_row_iterator<T, I, J, bi_matrix_column_major> it, ptrdiff_t n)
{
    bi_matrix_row_iterator<T, I, J, bi_matrix_column_major> nx =
        { it.item_ - n * it.size1_ };
    return nx;
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline T*
bi_matrix_row_iterator_deref(bi_matrix_row_iterator<T, I, J, S> it)
{
    return it.item_;
}

////////////////////////////////////////
// Column iterators.
template<class T, size_t I, size_t J, bi_matrix_layout_t S>
struct bi_matrix_column_iterator;

template<class T, size_t I, size_t J>
struct bi_matrix_column_iterator<T, I, J, bi_matrix_row_major>
{
    T* item_;
    size_t size2_;
};

template<class T, size_t I, size_t J>
struct bi_matrix_column_iterator<T, I, J, bi_matrix_column_major>
{
    T* item_;
};

////////////////////
// row major
template<class T, size_t I, size_t J>
inline bi_matrix_column_iterator<T, I, J, bi_matrix_row_major>
bi_matrix_column_iterator_inc(
    bi_matrix_column_iterator<T, I, J, bi_matrix_row_major> it)
{
    bi_matrix_column_iterator<T, I, J, bi_matrix_row_major> nx =
        { it.item_ + it->size2_ };
    return nx;
}

template<class T, size_t I, size_t J>
inline bi_matrix_column_iterator<T, I, J, bi_matrix_row_major>
bi_matrix_column_iterator_dec(
    bi_matrix_column_iterator<T, I, J, bi_matrix_row_major> it)
{
    bi_matrix_column_iterator<T, I, J, bi_matrix_row_major> nx =
        { it.item_ - it->size2_ };
    return nx;
}

template<class T, size_t I, size_t J>
inline bi_matrix_column_iterator<T, I, J, bi_matrix_row_major>
bi_matrix_column_iterator_plus(
    bi_matrix_column_iterator<T, I, J, bi_matrix_row_major> it, ptrdiff_t n)
{
    bi_matrix_column_iterator<T, I, J, bi_matrix_row_major> nx =
        { it.item_ + n * it->size2_ };
    return nx;
}

template<class T, size_t I, size_t J>
inline bi_matrix_column_iterator<T, I, J, bi_matrix_row_major>
bi_matrix_column_iterator_minus(
    bi_matrix_column_iterator<T, I, J, bi_matrix_row_major> it, ptrdiff_t n)
{
    bi_matrix_column_iterator<T, I, J, bi_matrix_row_major> nx =
        { it.item_ - n * it->size2_ };
    return nx;
}

////////////////////
// column major
template<class T, size_t I, size_t J>
inline bi_matrix_column_iterator<T, I, J, bi_matrix_column_major>
bi_matrix_column_iterator_inc(
    bi_matrix_column_iterator<T, I, J, bi_matrix_column_major> it)
{
    bi_matrix_column_iterator<T, I, J, bi_matrix_column_major> nx =
        { it.item_ + 1 };
    return nx;
}

template<class T, size_t I, size_t J>
inline bi_matrix_column_iterator<T, I, J, bi_matrix_column_major>
bi_matrix_column_iterator_dec(
    bi_matrix_column_iterator<T, I, J, bi_matrix_column_major> it)
{
    bi_matrix_column_iterator<T, I, J, bi_matrix_column_major> nx =
        { it.item_ - 1 };
    return nx;
}

template<class T, size_t I, size_t J>
inline bi_matrix_column_iterator<T, I, J, bi_matrix_column_major>
bi_matrix_column_iterator_plus(
    bi_matrix_column_iterator<T, I, J, bi_matrix_column_major> it, ptrdiff_t n)
{
    bi_matrix_column_iterator<T, I, J, bi_matrix_column_major> nx =
        { it.item_ + n };
    return nx;
}

template<class T, size_t I, size_t J>
inline bi_matrix_column_iterator<T, I, J, bi_matrix_column_major>
bi_matrix_column_iterator_minus(
    bi_matrix_column_iterator<T, I, J, bi_matrix_column_major> it, ptrdiff_t n)
{
    bi_matrix_column_iterator<T, I, J, bi_matrix_column_major> nx =
        { it.item_ - n };
    return nx;
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline T*
bi_matrix_column_iterator_deref(bi_matrix_column_iterator<T, I, J, S> it)
{
    return it.item_;
}


////////////////////////////////////////
/**
 * @ingroup Utility
 * @brief Create a matrix.
 *
 * @tparam T The type of the element.
 * @tparam I The index of the first row.
 * @tparam J The index of the first column.
 * @tparam S The storage orgnization.
 *           Can be `bi_matrix_row_major` or `bi_matrix_column_major`.
 *           Defaults to `bi_matrix_row_major`.
 *
 * @param[out] mx      The matrix.
 *                     It **must** be an *uninitialized* matrix.
 * @param[in]  rows    The number of rows.
 * @param[in]  columns The number of columns.
 *
 * The elements are default initialized.
 */
template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline void
bi_matrix_init(bi_matrix<T, I, J, S>* mx, size_t rows, size_t columns)
{
    typedef bi_matrix<T, I, J, S>  matrix_type;
    size_t size = rows * columns;
    BOOST_ASSERT(mx);
    mx->size1_ = rows;
    mx->size2_ = columns;
    mx->data_  = 0;
    if (size)
    {
        mx->data_ = (T*)::operator new (size * sizeof (T));
        if (std::is_pod<T>::value)
        {
            std::memset(mx->data_, 0, size * sizeof (T));
        }
        else
        {
            T* p = mx->data_;
            T* last = p + size;
            while (p != last)
            {
                new (p++) T();
            }
        }
    }
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline void
bi_matrix_destroy(bi_matrix<T, I, J, S>* mx)
{
    BOOST_ASSERT(mx);
    ::operator delete (mx->data_);
}

/**
 * @brief Copy a matrix.
 *
 * @tparam T The type of the element.
 * @tparam I The index of the first row.
 * @tparam J The index of the first column.
 * @tparam S The storage orgnization.
 *
 * @param[in]  src The source matrix.
 *                 It **must** be an *initialized* matrix.
 * @param[out] dst The destination matrix.
 *                 It **must** be an *uninitialized* matrix.
 */
template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline void
bi_matrix_copy(const bi_matrix<T, I, J, S>* src, bi_matrix<T, I, J, S>* dst)
{
    typedef bi_matrix<T, I, J, S>  matrix_type;
    size_t size = src->size1_ * src->size2_;
    BOOST_ASSERT(src);
    BOOST_ASSERT(dst);
    dst->size1_ = src->size1_;
    dst->size2_ = src->size2_;
    dst->data_  = src->data_;
    if (size)
    {
        dst->data_ = (T*)::operator new (size * sizeof (T));
        if (std::is_pod<T>::value)
        {
            std::memcpy(dst->data_, src->data_, size * sizeof (T));
        }
        else
        {
            T* d = dst->data_;
            T* s = src->data_;
            T* last = s + size;
            while (s != last)
            {
                new (d++) T(*s++);
            }
        }
    }
}

/** @brief The total number of elements. */
template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline size_t
bi_matrix_size(const bi_matrix<T, I, J, S>* mx) BOOST_NOEXCEPT
{
    return mx->size1_ * mx->size2_;
}

/** @brief The number of rows. */
template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline size_t
bi_matrix_size1(const bi_matrix<T, I, J, S>* mx) BOOST_NOEXCEPT
{
    return mx->size1_;
}

/** @brief The number of columns. */
template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline size_t
bi_matrix_size2(const bi_matrix<T, I, J, S>* mx) BOOST_NOEXCEPT
{
    return mx->size2_;
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline T*
bi_matrix_at(const bi_matrix<T, I, J, S>* mx, size_t n) BOOST_NOEXCEPT
{
    BOOST_ASSERT(0 <= n && n < mx->size1_ * mx->size2_);
    return mx->data_ + n;
}

template<class T, size_t I, size_t J>
inline T*
bi_matrix_at(const bi_matrix<T, I, J, bi_matrix_row_major>* mx, size_t i, size_t j) BOOST_NOEXCEPT
{
    BOOST_ASSERT(I <= i && i < I + mx->size1_);
    BOOST_ASSERT(J <= j && j < J + mx->size2_);
    return mx->data_ + (i - I) * mx->size2_ + (j - J);
}

template<class T, size_t I, size_t J>
inline T*
bi_matrix_at(const bi_matrix<T, I, J, bi_matrix_column_major>* mx, size_t i, size_t j) BOOST_NOEXCEPT
{
    BOOST_ASSERT(I <= i && i < I + mx->size1_);
    BOOST_ASSERT(J <= j && j < J + mx->size2_);
    return mx->data_ + (j - J) * mx->size1_ + (i - I);
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline T*
bi_matrix_front(const bi_matrix<T, I, J, S>* mx) BOOST_NOEXCEPT
{
    BOOST_ASSERT(mx->size1_ && mx->size2_);
    return mx->data_;
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline T*
bi_matrix_back(const bi_matrix<T, I, J, S>* mx) BOOST_NOEXCEPT
{
    BOOST_ASSERT(mx->size1_ && mx->size2_);
    return mx->data_ + (mx->size1_ * mx->size2_ - 1);
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline T*
bi_matrix_data(const bi_matrix<T, I, J, S>* mx) BOOST_NOEXCEPT
{
    return mx->data_;
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline void
bi_matrix_fill(const bi_matrix<T, I, J, S>* mx, const T& v)
{
    T* p = mx->data_;
    T* last = p + mx->size1_ * mx->size2_;
    while (p != last)
    {
        *(p++) = v;
    }
}

/** @brief Fill a row. */
template<class T, size_t I, size_t J>
inline void
bi_matrix_fill1(const bi_matrix<T, I, J, bi_matrix_row_major>* mx,
                size_t i, const T& v)
{
    BOOST_ASSERT(I <= i && i < I + mx->size1_);
    T* p = mx->data_ + (i - I) * mx->size2_;
    T* last = p + mx->size2_;
    while (p != last)
    {
        *(p++) = v;
    }
}

/** @brief Fill a row. */
template<class T, size_t I, size_t J>
inline void
bi_matrix_fill1(const bi_matrix<T, I, J, bi_matrix_column_major>* mx,
                size_t i, const T& v)
{
    BOOST_ASSERT(I <= i && i < I + mx->size1_);
    T* p = mx->data_ + (i - I);
    for (size_t j = 0; j < mx->size2_; ++j)
    {
        *p = v;
        p += mx->size1_;
    }
}

/** @brief Fill a column. */
template<class T, size_t I, size_t J>
inline void
bi_matrix_fill2(const bi_matrix<T, I, J, bi_matrix_row_major>* mx,
                size_t j, const T& v)
{
    BOOST_ASSERT(J <= j && j < J + mx->size1_);
    T* p = mx->data_ + (j - J);
    for (size_t i = 0; i < mx->size1_; ++i)
    {
        *p = v;
        p += mx->size2_;
    }
}

/** @brief Fill a column. */
template<class T, size_t I, size_t J>
inline void
bi_matrix_fill2(const bi_matrix<T, I, J, bi_matrix_column_major>* mx,
                size_t j, const T& v)
{
    BOOST_ASSERT(J <= j && j < J + mx->size1_);
    T* p = mx->data_ + (j - J) * mx->size1_;
    T* last = p + mx->size1_;
    while (p != last)
    {
        *(p++) = v;
    }
}

/** @brief The begin iterator of the matrix. */
template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline T*
bi_matrix_begin(const bi_matrix<T, I, J, S>* mx) BOOST_NOEXCEPT
{
    return mx->data_;
}

/** @brief The end iterator of the matrix. */
template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline T*
bi_matrix_end(const bi_matrix<T, I, J, S>* mx) BOOST_NOEXCEPT
{
    return mx->data_ + mx->size1_ + mx->size2_;
}

} // namespace detail }}}


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Utility
 * @brief An matrix with base indices.
 *
 * @tparam T The type of the element.
 * @tparam I The index of the first row.
 *           Defaults to `0`.
 * @tparam J The index of the first column.
 *           Defaults to `I`.
 * @tparam S The storage orgnization.
 *           Can be `bi_matrix_row_major` or `bi_matrix_column_major`.
 *           Defaults to `bi_matrix_row_major`.
 *
 * @param[out] mx      The matrix.
 * @param[in]  rows    The number of rows.
 * @param[in]  columns The number of columns.
 *
 * The elements are default initialized.
 */
template<class T, size_t I = 0, size_t J = I,
         bi_matrix_layout_t S = bi_matrix_row_major>
class BiMatrix;

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
class BiMatrixIterator;

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
class ConstBiMatrixIterator;

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
class BiMatrixRowIterator;

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
class ConstBiMatrixRowIterator;

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
class BiMatrixColumnIterator;

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
class ConstBiMatrixColumnIterator;

////////////////////////////////////////////////////////////////////////////////
// Iterator./*{{{*/
template<class T, size_t I, size_t J, bi_matrix_layout_t S>
class BiMatrixIterator
{
public:
    typedef ptrdiff_t difference_type;
    typedef T         value_type;
    typedef T*        pointer;
    typedef T&        reference;
    typedef std::random_access_iterator_tag iterator_category;

    BOOST_STATIC_CONSTANT(bi_matrix_layout_t, layout = S);

    friend class BiMatrix<T, I, J, S>;
    friend class ConstBiMatrixIterator<T, I, J, S>;

private:
    explicit BiMatrixIterator(pointer p) BOOST_NOEXCEPT :
        p_(p)
    {
    }

public:
    // All categories: copy-constructible, copy-assignable and destructible.
    // X b(a); b = a;
    BiMatrixIterator(const BiMatrixIterator& rhs) BOOST_NOEXCEPT :
        p_(rhs.p_)
    {
    }

    BiMatrixIterator& operator=(const BiMatrixIterator& rhs) BOOST_NOEXCEPT
    {
        if (this != &rhs)
        {
            p_ = rhs.p_;
        }
        return *this;
    }

    // All categories: can be incremented.
    // ++a; a++;
    BiMatrixIterator& operator++(void) BOOST_NOEXCEPT
    {
        ++p_;
        return *this;
    }

    BiMatrixIterator operator++(int) BOOST_NOEXCEPT
    {
        BiMatrixIterator it(*this);
        ++p_;
        return it;
    }

    // Input: supports equality/inequality comparisons.
    // a == b; a != b;
    bool operator==(const BiMatrixIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ == rhs.p_;
    }

    bool operator!=(const BiMatrixIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ != rhs.p_;
    }

    // Input: can be dereferenced as an rvalue.
    // *a; a->m;
    // Output: can be dereferenced as an lvalue
    // (only for mutable iterator types)
    // *a = t; *a++ = t;
    reference operator*(void) const BOOST_NOEXCEPT
    {
        return *p_;
    }

    pointer operator->(void) const BOOST_NOEXCEPT
    {
        return p_;
    }

    // Forward: input & output.

    // Forward: default-constructible.
    // X a; X();
    BiMatrixIterator(void) BOOST_NOEXCEPT :
        p_(nullptr)
    {
    }

    // Forward: multi-pass: neither dereferencing nor incrementing affects
    //                      dereferenceability.
    // { b = a; *a++; *b; }

    // Bidirectional: Forward.

    // Bidirectional: can be decremented.
    // --a; a--; *a--;
    BiMatrixIterator& operator--(void) BOOST_NOEXCEPT
    {
        --p_;
        return *this;
    }

    BiMatrixIterator operator--(int) BOOST_NOEXCEPT
    {
        BiMatrixIterator it(*this);
        --p_;
        return it;
    }

    // Random access: bidirectional.

    // Random access: supports arithmetic operators + and -.
    // a + n; n + a; a - n; a - b;
    BiMatrixIterator operator+(difference_type n) const BOOST_NOEXCEPT
    {
        return BiMatrixIterator(p_ + n);
    }

    BiMatrixIterator operator-(difference_type n) const BOOST_NOEXCEPT
    {
        return BiMatrixIterator(p_ - n);
    }

    difference_type operator-(const BiMatrixIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ - rhs.p_;
    }

    // Random access: supports inequality comparisons (<, >, <= and >=)
    //                between iterators.
    // a < b; a > b; a <= b; a >= b;
    bool operator<(const BiMatrixIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ < rhs.p_;
    }

    bool operator>(const BiMatrixIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ > rhs.p_;
    }

    bool operator<=(const BiMatrixIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ <= rhs.p_;
    }

    bool operator>=(const BiMatrixIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ >= rhs.p_;
    }

    // Random access: supports compound assignment operations += and -=.
    // a += n; a -= n;
    BiMatrixIterator& operator+=(difference_type n) BOOST_NOEXCEPT
    {
        p_ += n;
        return *this;
    }

    BiMatrixIterator& operator-=(difference_type n) BOOST_NOEXCEPT
    {
        p_ -= n;
        return *this;
    }

    // Random access: supports offset dereference operator.
    // a[n];
    reference operator[](difference_type n) const BOOST_NOEXCEPT
    {
        return p_[n];
    }

private:
    pointer p_;
};

// Random access: supports arithmetic operators + and -.
// a + n; n + a; a - n; a - b;
template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline BiMatrixIterator<T, I, J, S>
operator+(typename BiMatrixIterator<T, I, J, S>::difference_type n,
          const BiMatrixIterator<T, I, J, S>& it) BOOST_NOEXCEPT
{
    return it + n;
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
class ConstBiMatrixIterator
{
public:
    typedef ptrdiff_t difference_type;
    typedef const T   value_type;
    typedef const T*  pointer;
    typedef const T&  reference;
    typedef std::random_access_iterator_tag iterator_category;

    BOOST_STATIC_CONSTANT(bi_matrix_layout_t, layout = S);

    friend class BiMatrix<T, I, J, S>;

private:
    explicit ConstBiMatrixIterator(pointer p) BOOST_NOEXCEPT :
        p_(p)
    {
    }

public:
    // Implicit conversion from BiMatrixIterator to ConstBiMatrixIterator.
    ConstBiMatrixIterator(const BiMatrixIterator<T, I, J, S>& rhs) BOOST_NOEXCEPT :
        p_(rhs.p_)
    {
    }

    ConstBiMatrixIterator& operator=(const BiMatrixIterator<T, I, J, S>& rhs) BOOST_NOEXCEPT
    {
        p_ = rhs.p_;
        return *this;
    }

    // All categories: copy-constructible, copy-assignable and destructible.
    // X b(a); b = a;
    ConstBiMatrixIterator(const ConstBiMatrixIterator& rhs) BOOST_NOEXCEPT :
        p_(rhs.p_)
    {
    }

    ConstBiMatrixIterator& operator=(const ConstBiMatrixIterator& rhs) BOOST_NOEXCEPT
    {
        if (this != &rhs)
        {
            p_ = rhs.p_;
        }
        return *this;
    }

    // All categories: can be incremented.
    // ++a; a++;
    ConstBiMatrixIterator& operator++(void) BOOST_NOEXCEPT
    {
        ++p_;
        return *this;
    }

    ConstBiMatrixIterator operator++(int) BOOST_NOEXCEPT
    {
        ConstBiMatrixIterator it(*this);
        ++p_;
        return it;
    }

    // Input: supports equality/inequality comparisons.
    // a == b; a != b;
    bool operator==(const ConstBiMatrixIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ == rhs.p_;
    }

    bool operator!=(const ConstBiMatrixIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ != rhs.p_;
    }

    // Input: can be dereferenced as an rvalue.
    // *a; a->m;
    // Output: can be dereferenced as an lvalue
    // (only for mutable iterator types)
    // *a = t; *a++ = t;
    reference operator*(void) const BOOST_NOEXCEPT
    {
        return *p_;
    }

    pointer operator->(void) const BOOST_NOEXCEPT
    {
        return p_;
    }

    // Forward: input & output.

    // Forward: default-constructible.
    // X a; X();
    ConstBiMatrixIterator(void) BOOST_NOEXCEPT :
        p_(nullptr)
    {
    }

    // Forward: multi-pass: neither dereferencing nor incrementing affects
    //                      dereferenceability.
    // { b = a; *a++; *b; }

    // Bidirectional: Forward.

    // Bidirectional: can be decremented.
    // --a; a--; *a--;
    ConstBiMatrixIterator& operator--(void) BOOST_NOEXCEPT
    {
        --p_;
        return *this;
    }

    ConstBiMatrixIterator operator--(int) BOOST_NOEXCEPT
    {
        ConstBiMatrixIterator it(*this);
        --p_;
        return it;
    }

    // Random access: bidirectional.

    // Random access: supports arithmetic operators + and -.
    // a + n; n + a; a - n; a - b;
    ConstBiMatrixIterator operator+(difference_type n) const BOOST_NOEXCEPT
    {
        return ConstBiMatrixIterator(p_ + n);
    }

    ConstBiMatrixIterator operator-(difference_type n) const BOOST_NOEXCEPT
    {
        return ConstBiMatrixIterator(p_ - n);
    }

    difference_type operator-(const ConstBiMatrixIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ - rhs.p_;
    }

    // Random access: supports inequality comparisons (<, >, <= and >=)
    //                between iterators.
    // a < b; a > b; a <= b; a >= b;
    bool operator<(const ConstBiMatrixIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ < rhs.p_;
    }

    bool operator>(const ConstBiMatrixIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ > rhs.p_;
    }

    bool operator<=(const ConstBiMatrixIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ <= rhs.p_;
    }

    bool operator>=(const ConstBiMatrixIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ >= rhs.p_;
    }

    // Random access: supports compound assignment operations += and -=.
    // a += n; a -= n;
    ConstBiMatrixIterator& operator+=(difference_type n) BOOST_NOEXCEPT
    {
        p_ += n;
        return *this;
    }

    ConstBiMatrixIterator& operator-=(difference_type n) BOOST_NOEXCEPT
    {
        p_ -= n;
        return *this;
    }

    // Random access: supports offset dereference operator.
    // a[n];
    reference operator[](difference_type n) const BOOST_NOEXCEPT
    {
        return p_[n];
    }

private:
    pointer p_;
};

// Random access: supports arithmetic operators + and -.
// a + n; n + a; a - n; a - b;
template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline ConstBiMatrixIterator<T, I, J, S>
operator+(typename ConstBiMatrixIterator<T, I, J, S>::difference_type n,
          const ConstBiMatrixIterator<T, I, J, S>& it) BOOST_NOEXCEPT
{
    return it + n;
}

// Support mixed operations between BiMatrixIterator and ConstBiMatrixIterator.
template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline bool
operator==(const BiMatrixIterator<T, I, J, S>& lhs,
           const ConstBiMatrixIterator<T, I, J, S>& rhs) BOOST_NOEXCEPT
{
    return rhs == lhs;
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline bool
operator!=(const BiMatrixIterator<T, I, J, S>& lhs,
           const ConstBiMatrixIterator<T, I, J, S>& rhs) BOOST_NOEXCEPT
{
    return rhs != lhs;
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline bool
operator<(const BiMatrixIterator<T, I, J, S>& lhs,
          const ConstBiMatrixIterator<T, I, J, S>& rhs) BOOST_NOEXCEPT
{
    return rhs > lhs;
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline bool
operator>(const BiMatrixIterator<T, I, J, S>& lhs,
          const ConstBiMatrixIterator<T, I, J, S>& rhs) BOOST_NOEXCEPT
{
    return rhs < lhs;
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline bool
operator<=(const BiMatrixIterator<T, I, J, S>& lhs,
           const ConstBiMatrixIterator<T, I, J, S>& rhs) BOOST_NOEXCEPT
{
    return rhs >= lhs;
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline bool
operator>=(const BiMatrixIterator<T, I, J, S>& lhs,
           const ConstBiMatrixIterator<T, I, J, S>& rhs) BOOST_NOEXCEPT
{
    return rhs <= lhs;
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline typename BiMatrixIterator<T, I, J, S>::difference_type
operator-(const BiMatrixIterator<T, I, J, S>& lhs,
          const ConstBiMatrixIterator<T, I, J, S>& rhs) BOOST_NOEXCEPT
{
    return -(rhs - lhs);
}

/*}}}*/

////////////////////////////////////////////////////////////////////////////////
// Row iterator./*{{{*/
////////////////////
// Row iterator (row major)./*{{{*/
template<class T, size_t I, size_t J>
class BiMatrixRowIterator<T, I, J, bi_matrix_row_major>
{
public:
    typedef ptrdiff_t difference_type;
    typedef T         value_type;
    typedef T*        pointer;
    typedef T&        reference;
    typedef std::random_access_iterator_tag iterator_category;

    BOOST_STATIC_CONSTANT(bi_matrix_layout_t, layout = bi_matrix_row_major);

    friend class BiMatrix<T, I, J, layout>;
    friend class ConstBiMatrixRowIterator<T, I, J, layout>;

private:
    explicit BiMatrixRowIterator(T* p) BOOST_NOEXCEPT :
        p_(p)
    {
    }

public:
    // All categories: copy-constructible, copy-assignable and destructible.
    // X b(a); b = a;
    BiMatrixRowIterator(const BiMatrixRowIterator& rhs) BOOST_NOEXCEPT :
        p_(rhs.p_)
    {
    }

    BiMatrixRowIterator& operator=(const BiMatrixRowIterator& rhs) BOOST_NOEXCEPT
    {
        if (this != &rhs)
        {
            p_ = rhs.p_;
        }
        return *this;
    }

    // All categories: can be incremented.
    // ++a; a++;
    BiMatrixRowIterator& operator++(void) BOOST_NOEXCEPT
    {
        ++p_;
        return *this;
    }

    BiMatrixRowIterator operator++(int) BOOST_NOEXCEPT
    {
        BiMatrixRowIterator it(*this);
        ++p_;
        return it;
    }

    // Input: supports equality/inequality comparisons.
    // a == b; a != b;
    bool operator==(const BiMatrixRowIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ == rhs.p_;
    }

    bool operator!=(const BiMatrixRowIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ != rhs.p_;
    }

    // Input: can be dereferenced as an rvalue.
    // *a; a->m;
    // Output: can be dereferenced as an lvalue
    // (only for mutable iterator types)
    // *a = t; *a++ = t;
    reference operator*(void) const BOOST_NOEXCEPT
    {
        return *p_;
    }

    pointer operator->(void) const BOOST_NOEXCEPT
    {
        return p_;
    }

    // Forward: input & output.

    // Forward: default-constructible.
    // X a; X();
    BiMatrixRowIterator(void) BOOST_NOEXCEPT :
        p_(nullptr)
    {
    }

    // Forward: multi-pass: neither dereferencing nor incrementing affects
    //                      dereferenceability.
    // { b = a; *a++; *b; }

    // Bidirectional: Forward.

    // Bidirectional: can be decremented.
    // --a; a--; *a--;
    BiMatrixRowIterator& operator--(void) BOOST_NOEXCEPT
    {
        --p_;
        return *this;
    }

    BiMatrixRowIterator operator--(int) BOOST_NOEXCEPT
    {
        BiMatrixRowIterator it(*this);
        --p_;
        return it;
    }

    // Random access: bidirectional.

    // Random access: supports arithmetic operators + and -.
    // a + n; n + a; a - n; a - b;
    BiMatrixRowIterator operator+(difference_type n) const BOOST_NOEXCEPT
    {
        return BiMatrixRowIterator(p_ + n);
    }

    BiMatrixRowIterator operator-(difference_type n) const BOOST_NOEXCEPT
    {
        return BiMatrixRowIterator(p_ - n);
    }

    difference_type operator-(const BiMatrixRowIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ - rhs.p_;
    }

    // Random access: supports inequality comparisons (<, >, <= and >=)
    //                between iterators.
    // a < b; a > b; a <= b; a >= b;
    bool operator<(const BiMatrixRowIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ < rhs.p_;
    }

    bool operator>(const BiMatrixRowIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ > rhs.p_;
    }

    bool operator<=(const BiMatrixRowIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ <= rhs.p_;
    }

    bool operator>=(const BiMatrixRowIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ >= rhs.p_;
    }

    // Random access: supports compound assignment operations += and -=.
    // a += n; a -= n;
    BiMatrixRowIterator& operator+=(difference_type n) BOOST_NOEXCEPT
    {
        p_ += n;
        return *this;
    }

    BiMatrixRowIterator& operator-=(difference_type n) BOOST_NOEXCEPT
    {
        p_ -= n;
        return *this;
    }

    // Random access: supports offset dereference operator.
    // a[n];
    reference operator[](difference_type n) const BOOST_NOEXCEPT
    {
        return p_[n];
    }

private:
    pointer p_;
};

/*}}}*/

////////////////////
// Const row iterator (row major)./*{{{*/
template<class T, size_t I, size_t J>
class ConstBiMatrixRowIterator<T, I, J, bi_matrix_row_major>
{
public:
    typedef ptrdiff_t difference_type;
    typedef const T   value_type;
    typedef const T*  pointer;
    typedef const T&  reference;
    typedef std::random_access_iterator_tag iterator_category;

    BOOST_STATIC_CONSTANT(bi_matrix_layout_t, layout = bi_matrix_row_major);

    friend class BiMatrix<T, I, J, layout>;

private:
    explicit ConstBiMatrixRowIterator(const T* p) BOOST_NOEXCEPT :
        p_(p)
    {
    }

public:
    // Implicit conversion from BiMatrixRowIterator to ConstBiMatrixRowIterator.
    ConstBiMatrixRowIterator(const BiMatrixRowIterator<T, I, J, layout>& rhs) BOOST_NOEXCEPT :
        p_(rhs.p_)
    {
    }

    ConstBiMatrixRowIterator& operator=(const BiMatrixRowIterator<T, I, J, layout>& rhs) BOOST_NOEXCEPT
    {
        p_ = rhs.p_;
        return *this;
    }

    // All categories: copy-constructible, copy-assignable and destructible.
    // X b(a); b = a;
    ConstBiMatrixRowIterator(const ConstBiMatrixRowIterator& rhs) BOOST_NOEXCEPT :
        p_(rhs.p_)
    {
    }

    ConstBiMatrixRowIterator& operator=(const ConstBiMatrixRowIterator& rhs) BOOST_NOEXCEPT
    {
        if (this != &rhs)
        {
            p_ = rhs.p_;
        }
        return *this;
    }

    // All categories: can be incremented.
    // ++a; a++;
    ConstBiMatrixRowIterator& operator++(void) BOOST_NOEXCEPT
    {
        ++p_;
        return *this;
    }

    ConstBiMatrixRowIterator operator++(int) BOOST_NOEXCEPT
    {
        ConstBiMatrixRowIterator it(*this);
        ++p_;
        return it;
    }

    // Input: supports equality/inequality comparisons.
    // a == b; a != b;
    bool operator==(const ConstBiMatrixRowIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ == rhs.p_;
    }

    bool operator!=(const ConstBiMatrixRowIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ != rhs.p_;
    }

    // Input: can be dereferenced as an rvalue.
    // *a; a->m;
    // Output: can be dereferenced as an lvalue
    // (only for mutable iterator types)
    // *a = t; *a++ = t;
    reference operator*(void) const BOOST_NOEXCEPT
    {
        return *p_;
    }

    pointer operator->(void) const BOOST_NOEXCEPT
    {
        return p_;
    }

    // Forward: input & output.

    // Forward: default-constructible.
    // X a; X();
    ConstBiMatrixRowIterator(void) BOOST_NOEXCEPT :
        p_(nullptr)
    {
    }

    // Forward: multi-pass: neither dereferencing nor incrementing affects
    //                      dereferenceability.
    // { b = a; *a++; *b; }

    // Bidirectional: Forward.

    // Bidirectional: can be decremented.
    // --a; a--; *a--;
    ConstBiMatrixRowIterator& operator--(void) BOOST_NOEXCEPT
    {
        --p_;
        return *this;
    }

    ConstBiMatrixRowIterator operator--(int) BOOST_NOEXCEPT
    {
        ConstBiMatrixRowIterator it(*this);
        --p_;
        return it;
    }

    // Random access: bidirectional.

    // Random access: supports arithmetic operators + and -.
    // a + n; n + a; a - n; a - b;
    ConstBiMatrixRowIterator operator+(difference_type n) const BOOST_NOEXCEPT
    {
        return ConstBiMatrixRowIterator(p_ + n);
    }

    ConstBiMatrixRowIterator operator-(difference_type n) const BOOST_NOEXCEPT
    {
        return ConstBiMatrixRowIterator(p_ - n);
    }

    difference_type operator-(const ConstBiMatrixRowIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ - rhs.p_;
    }

    // Random access: supports inequality comparisons (<, >, <= and >=)
    //                between iterators.
    // a < b; a > b; a <= b; a >= b;
    bool operator<(const ConstBiMatrixRowIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ < rhs.p_;
    }

    bool operator>(const ConstBiMatrixRowIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ > rhs.p_;
    }

    bool operator<=(const ConstBiMatrixRowIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ <= rhs.p_;
    }

    bool operator>=(const ConstBiMatrixRowIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ >= rhs.p_;
    }

    // Random access: supports compound assignment operations += and -=.
    // a += n; a -= n;
    ConstBiMatrixRowIterator& operator+=(difference_type n) BOOST_NOEXCEPT
    {
        p_ += n;
        return *this;
    }

    ConstBiMatrixRowIterator& operator-=(difference_type n) BOOST_NOEXCEPT
    {
        p_ -= n;
        return *this;
    }

    // Random access: supports offset dereference operator.
    // a[n];
    reference operator[](difference_type n) const BOOST_NOEXCEPT
    {
        return p_[n];
    }

private:
    pointer p_;
};

/*}}}*/

////////////////////
// Row iterator (column major)./*{{{*/
template<class T, size_t I, size_t J>
class BiMatrixRowIterator<T, I, J, bi_matrix_column_major>
{
public:
    typedef ptrdiff_t difference_type;
    typedef T         value_type;
    typedef T*        pointer;
    typedef T&        reference;
    typedef std::random_access_iterator_tag iterator_category;

    BOOST_STATIC_CONSTANT(bi_matrix_layout_t, layout = bi_matrix_column_major);

    friend class BiMatrix<T, I, J, layout>;
    friend class ConstBiMatrixRowIterator<T, I, J, layout>;

private:
    BiMatrixRowIterator(T* p, size_t size1) BOOST_NOEXCEPT :
        p_(p),
        size1_(size1)
    {
    }

public:
    // All categories: copy-constructible, copy-assignable and destructible.
    // X b(a); b = a;
    BiMatrixRowIterator(const BiMatrixRowIterator& rhs) BOOST_NOEXCEPT :
        p_(rhs.p_),
        size1_(rhs.size1_)
    {
    }

    BiMatrixRowIterator& operator=(const BiMatrixRowIterator& rhs) BOOST_NOEXCEPT
    {
        if (this != &rhs)
        {
            p_ = rhs.p_;
            size1_ = rhs.size1_;
        }
        return *this;
    }

    // All categories: can be incremented.
    // ++a; a++;
    BiMatrixRowIterator& operator++(void) BOOST_NOEXCEPT
    {
        p_ += size1_;
        return *this;
    }

    BiMatrixRowIterator operator++(int) BOOST_NOEXCEPT
    {
        BiMatrixRowIterator it(*this);
        p_ += size1_;
        return it;
    }

    // Input: supports equality/inequality comparisons.
    // a == b; a != b;
    bool operator==(const BiMatrixRowIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ == rhs.p_;
    }

    bool operator!=(const BiMatrixRowIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ != rhs.p_;
    }

    // Input: can be dereferenced as an rvalue.
    // *a; a->m;
    // Output: can be dereferenced as an lvalue
    // (only for mutable iterator types)
    // *a = t; *a++ = t;
    reference operator*(void) const BOOST_NOEXCEPT
    {
        return *p_;
    }

    pointer operator->(void) const BOOST_NOEXCEPT
    {
        return p_;
    }

    // Forward: input & output.

    // Forward: default-constructible.
    // X a; X();
    BiMatrixRowIterator(void) BOOST_NOEXCEPT :
        p_(nullptr),
        size1_(0)
    {
    }

    // Forward: multi-pass: neither dereferencing nor incrementing affects
    //                      dereferenceability.
    // { b = a; *a++; *b; }

    // Bidirectional: Forward.

    // Bidirectional: can be decremented.
    // --a; a--; *a--;
    BiMatrixRowIterator& operator--(void) BOOST_NOEXCEPT
    {
        p_ -= size1_;
        return *this;
    }

    BiMatrixRowIterator operator--(int) BOOST_NOEXCEPT
    {
        BiMatrixRowIterator it(*this);
        p_ -= size1_;
        return it;
    }

    // Random access: bidirectional.

    // Random access: supports arithmetic operators + and -.
    // a + n; n + a; a - n; a - b;
    BiMatrixRowIterator operator+(difference_type n) const BOOST_NOEXCEPT
    {
        return BiMatrixRowIterator(p_ + n * size1_, size1_);
    }

    BiMatrixRowIterator operator-(difference_type n) const BOOST_NOEXCEPT
    {
        return BiMatrixRowIterator(p_ - n * size1_, size1_);
    }

    difference_type operator-(const BiMatrixRowIterator& rhs) const BOOST_NOEXCEPT
    {
        return (p_ - rhs.p_) / (difference_type)size1_;
    }

    // Random access: supports inequality comparisons (<, >, <= and >=)
    //                between iterators.
    // a < b; a > b; a <= b; a >= b;
    bool operator<(const BiMatrixRowIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ < rhs.p_;
    }

    bool operator>(const BiMatrixRowIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ > rhs.p_;
    }

    bool operator<=(const BiMatrixRowIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ <= rhs.p_;
    }

    bool operator>=(const BiMatrixRowIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ >= rhs.p_;
    }

    // Random access: supports compound assignment operations += and -=.
    // a += n; a -= n;
    BiMatrixRowIterator& operator+=(difference_type n) BOOST_NOEXCEPT
    {
        p_ += n * size1_;
        return *this;
    }

    BiMatrixRowIterator& operator-=(difference_type n) BOOST_NOEXCEPT
    {
        p_ -= n * size1_;
        return *this;
    }

    // Random access: supports offset dereference operator.
    // a[n];
    reference operator[](difference_type n) const BOOST_NOEXCEPT
    {
        return p_[n * size1_];
    }

private:
    pointer p_;
    size_t size1_;
};

/*}}}*/

////////////////////
// Const row iterator (column major)./*{{{*/
template<class T, size_t I, size_t J>
class ConstBiMatrixRowIterator<T, I, J, bi_matrix_column_major>
{
public:
    typedef ptrdiff_t difference_type;
    typedef const T   value_type;
    typedef const T*  pointer;
    typedef const T&  reference;
    typedef std::random_access_iterator_tag iterator_category;

    BOOST_STATIC_CONSTANT(bi_matrix_layout_t, layout = bi_matrix_column_major);

    friend class BiMatrix<T, I, J, layout>;

private:
    ConstBiMatrixRowIterator(const T* p, size_t size1) BOOST_NOEXCEPT :
        p_(p),
        size1_(size1)
    {
    }

public:
    // Implicit conversion from BiMatrixRowIterator to ConstBiMatrixRowIterator.
    ConstBiMatrixRowIterator(const BiMatrixRowIterator<T, I, J, layout>& rhs) BOOST_NOEXCEPT :
        p_(rhs.p_),
        size1_(rhs.size1_)
    {
    }

    ConstBiMatrixRowIterator& operator=(const BiMatrixRowIterator<T, I, J, layout>& rhs) BOOST_NOEXCEPT
    {
        p_ = rhs.p_;
        size1_ = rhs.size1_;
        return *this;
    }

    // All categories: copy-constructible, copy-assignable and destructible.
    // X b(a); b = a;
    ConstBiMatrixRowIterator(const ConstBiMatrixRowIterator& rhs) BOOST_NOEXCEPT :
        p_(rhs.p_),
        size1_(rhs.size1_)
    {
    }

    ConstBiMatrixRowIterator& operator=(const ConstBiMatrixRowIterator& rhs) BOOST_NOEXCEPT
    {
        if (this != &rhs)
        {
            p_ = rhs.p_;
            size1_ = rhs.size1_;
        }
        return *this;
    }

    // All categories: can be incremented.
    // ++a; a++;
    ConstBiMatrixRowIterator& operator++(void) BOOST_NOEXCEPT
    {
        p_ += size1_;
        return *this;
    }

    ConstBiMatrixRowIterator operator++(int) BOOST_NOEXCEPT
    {
        ConstBiMatrixRowIterator it(*this);
        p_ += size1_;
        return it;
    }

    // Input: supports equality/inequality comparisons.
    // a == b; a != b;
    bool operator==(const ConstBiMatrixRowIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ == rhs.p_;
    }

    bool operator!=(const ConstBiMatrixRowIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ != rhs.p_;
    }

    // Input: can be dereferenced as an rvalue.
    // *a; a->m;
    // Output: can be dereferenced as an lvalue
    // (only for mutable iterator types)
    // *a = t; *a++ = t;
    reference operator*(void) const BOOST_NOEXCEPT
    {
        return *p_;
    }

    pointer operator->(void) const BOOST_NOEXCEPT
    {
        return p_;
    }

    // Forward: input & output.

    // Forward: default-constructible.
    // X a; X();
    ConstBiMatrixRowIterator(void) BOOST_NOEXCEPT :
        p_(nullptr),
        size1_(0)
    {
    }

    // Forward: multi-pass: neither dereferencing nor incrementing affects
    //                      dereferenceability.
    // { b = a; *a++; *b; }

    // Bidirectional: Forward.

    // Bidirectional: can be decremented.
    // --a; a--; *a--;
    ConstBiMatrixRowIterator& operator--(void) BOOST_NOEXCEPT
    {
        p_ -= size1_;
        return *this;
    }

    ConstBiMatrixRowIterator operator--(int) BOOST_NOEXCEPT
    {
        ConstBiMatrixRowIterator it(*this);
        p_ -= size1_;
        return it;
    }

    // Random access: bidirectional.

    // Random access: supports arithmetic operators + and -.
    // a + n; n + a; a - n; a - b;
    ConstBiMatrixRowIterator operator+(difference_type n) const BOOST_NOEXCEPT
    {
        return ConstBiMatrixRowIterator(p_ + n * size1_, size1_);
    }

    ConstBiMatrixRowIterator operator-(difference_type n) const BOOST_NOEXCEPT
    {
        return ConstBiMatrixRowIterator(p_ - n * size1_, size1_);
    }

    difference_type operator-(const ConstBiMatrixRowIterator& rhs) const BOOST_NOEXCEPT
    {
        return (p_ - rhs.p_) / (difference_type)size1_;
    }

    // Random access: supports inequality comparisons (<, >, <= and >=)
    //                between iterators.
    // a < b; a > b; a <= b; a >= b;
    bool operator<(const ConstBiMatrixRowIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ < rhs.p_;
    }

    bool operator>(const ConstBiMatrixRowIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ > rhs.p_;
    }

    bool operator<=(const ConstBiMatrixRowIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ <= rhs.p_;
    }

    bool operator>=(const ConstBiMatrixRowIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ >= rhs.p_;
    }

    // Random access: supports compound assignment operations += and -=.
    // a += n; a -= n;
    ConstBiMatrixRowIterator& operator+=(difference_type n) BOOST_NOEXCEPT
    {
        p_ += n * size1_;
        return *this;
    }

    ConstBiMatrixRowIterator& operator-=(difference_type n) BOOST_NOEXCEPT
    {
        p_ -= n * size1_;
        return *this;
    }

    // Random access: supports offset dereference operator.
    // a[n];
    reference operator[](difference_type n) const BOOST_NOEXCEPT
    {
        return p_[n * size1_];
    }

private:
    pointer p_;
    size_t size1_;
};

/*}}}*/

////////////////////
// Random access: supports arithmetic operators + and -.
// a + n; n + a; a - n; a - b;
template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline BiMatrixRowIterator<T, I, J, S>
operator+(typename BiMatrixRowIterator<T, I, J, S>::difference_type n,
          const BiMatrixRowIterator<T, I, J, S>& it) BOOST_NOEXCEPT
{
    return it + n;
}

// Random access: supports arithmetic operators + and -.
// a + n; n + a; a - n; a - b;
template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline ConstBiMatrixRowIterator<T, I, J, S>
operator+(typename ConstBiMatrixRowIterator<T, I, J, S>::difference_type n,
          const ConstBiMatrixRowIterator<T, I, J, S>& it) BOOST_NOEXCEPT
{
    return it + n;
}

// Support mixed operations between BiMatrixIterator and ConstBiMatrixIterator.
template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline bool
operator==(const BiMatrixRowIterator<T, I, J, S>& lhs,
           const ConstBiMatrixRowIterator<T, I, J, S>& rhs) BOOST_NOEXCEPT
{
    return rhs == lhs;
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline bool
operator!=(const BiMatrixRowIterator<T, I, J, S>& lhs,
           const ConstBiMatrixRowIterator<T, I, J, S>& rhs) BOOST_NOEXCEPT
{
    return rhs != lhs;
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline bool
operator<(const BiMatrixRowIterator<T, I, J, S>& lhs,
          const ConstBiMatrixRowIterator<T, I, J, S>& rhs) BOOST_NOEXCEPT
{
    return rhs > lhs;
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline bool
operator>(const BiMatrixRowIterator<T, I, J, S>& lhs,
          const ConstBiMatrixRowIterator<T, I, J, S>& rhs) BOOST_NOEXCEPT
{
    return rhs < lhs;
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline bool
operator<=(const BiMatrixRowIterator<T, I, J, S>& lhs,
           const ConstBiMatrixRowIterator<T, I, J, S>& rhs) BOOST_NOEXCEPT
{
    return rhs >= lhs;
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline bool
operator>=(const BiMatrixRowIterator<T, I, J, S>& lhs,
           const ConstBiMatrixRowIterator<T, I, J, S>& rhs) BOOST_NOEXCEPT
{
    return rhs <= lhs;
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline typename ConstBiMatrixRowIterator<T, I, J, S>::difference_type
operator-(const BiMatrixRowIterator<T, I, J, S>& lhs,
          const ConstBiMatrixRowIterator<T, I, J, S>& rhs) BOOST_NOEXCEPT
{
    return -(rhs - lhs);
}

/*}}}*/

////////////////////////////////////////////////////////////////////////////////
// Column iterator./*{{{*/
////////////////////
// Column iterator (row major)./*{{{*/
template<class T, size_t I, size_t J>
class BiMatrixColumnIterator<T, I, J, bi_matrix_row_major>
{
public:
    typedef ptrdiff_t difference_type;
    typedef T         value_type;
    typedef T*        pointer;
    typedef T&        reference;
    typedef std::random_access_iterator_tag iterator_category;

    BOOST_STATIC_CONSTANT(bi_matrix_layout_t, layout = bi_matrix_row_major);

    friend class BiMatrix<T, I, J, layout>;
    friend class ConstBiMatrixColumnIterator<T, I, J, layout>;

private:
    BiMatrixColumnIterator(T* p, size_t size2) BOOST_NOEXCEPT :
        p_(p),
        size2_(size2)
    {
    }

public:
    // All categories: copy-constructible, copy-assignable and destructible.
    // X b(a); b = a;
    BiMatrixColumnIterator(const BiMatrixColumnIterator& rhs) BOOST_NOEXCEPT :
        p_(rhs.p_),
        size2_(rhs.size2_)
    {
    }

    BiMatrixColumnIterator& operator=(const BiMatrixColumnIterator& rhs) BOOST_NOEXCEPT
    {
        if (this != &rhs)
        {
            p_ = rhs.p_;
            size2_ = rhs.size2_;
        }
        return *this;
    }

    // All categories: can be incremented.
    // ++a; a++;
    BiMatrixColumnIterator& operator++(void) BOOST_NOEXCEPT
    {
        p_ += size2_;
        return *this;
    }

    BiMatrixColumnIterator operator++(int) BOOST_NOEXCEPT
    {
        BiMatrixColumnIterator it(*this);
        p_ += size2_;
        return it;
    }

    // Input: supports equality/inequality comparisons.
    // a == b; a != b;
    bool operator==(const BiMatrixColumnIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ == rhs.p_;
    }

    bool operator!=(const BiMatrixColumnIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ != rhs.p_;
    }

    // Input: can be dereferenced as an rvalue.
    // *a; a->m;
    // Output: can be dereferenced as an lvalue
    // (only for mutable iterator types)
    // *a = t; *a++ = t;
    reference operator*(void) const BOOST_NOEXCEPT
    {
        return *p_;
    }

    pointer operator->(void) const BOOST_NOEXCEPT
    {
        return p_;
    }

    // Forward: input & output.

    // Forward: default-constructible.
    // X a; X();
    BiMatrixColumnIterator(void) BOOST_NOEXCEPT :
        p_(nullptr),
        size2_(0)
    {
    }

    // Forward: multi-pass: neither dereferencing nor incrementing affects
    //                      dereferenceability.
    // { b = a; *a++; *b; }

    // Bidirectional: Forward.

    // Bidirectional: can be decremented.
    // --a; a--; *a--;
    BiMatrixColumnIterator& operator--(void) BOOST_NOEXCEPT
    {
        p_ -= size2_;
        return *this;
    }

    BiMatrixColumnIterator operator--(int) BOOST_NOEXCEPT
    {
        BiMatrixColumnIterator it(*this);
        p_ -= size2_;
        return it;
    }

    // Random access: bidirectional.

    // Random access: supports arithmetic operators + and -.
    // a + n; n + a; a - n; a - b;
    BiMatrixColumnIterator operator+(difference_type n) const BOOST_NOEXCEPT
    {
        return BiMatrixColumnIterator(p_ + n * size2_, size2_);
    }

    BiMatrixColumnIterator operator-(difference_type n) const BOOST_NOEXCEPT
    {
        return BiMatrixColumnIterator(p_ - n * size2_, size2_);
    }

    difference_type operator-(const BiMatrixColumnIterator& rhs) const BOOST_NOEXCEPT
    {
        return (p_ - rhs.p_) / (difference_type)size2_;
    }

    // Random access: supports inequality comparisons (<, >, <= and >=)
    //                between iterators.
    // a < b; a > b; a <= b; a >= b;
    bool operator<(const BiMatrixColumnIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ < rhs.p_;
    }

    bool operator>(const BiMatrixColumnIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ > rhs.p_;
    }

    bool operator<=(const BiMatrixColumnIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ <= rhs.p_;
    }

    bool operator>=(const BiMatrixColumnIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ >= rhs.p_;
    }

    // Random access: supports compound assignment operations += and -=.
    // a += n; a -= n;
    BiMatrixColumnIterator& operator+=(difference_type n) BOOST_NOEXCEPT
    {
        p_ += n * size2_;
        return *this;
    }

    BiMatrixColumnIterator& operator-=(difference_type n) BOOST_NOEXCEPT
    {
        p_ -= n * size2_;
        return *this;
    }

    // Random access: supports offset dereference operator.
    // a[n];
    reference operator[](difference_type n) const BOOST_NOEXCEPT
    {
        return p_[n * size2_];
    }

private:
    pointer p_;
    size_t size2_;
};

/*}}}*/

////////////////////
// Const column iterator (row major)./*{{{*/
template<class T, size_t I, size_t J>
class ConstBiMatrixColumnIterator<T, I, J, bi_matrix_row_major>
{
public:
    typedef ptrdiff_t difference_type;
    typedef const T   value_type;
    typedef const T*  pointer;
    typedef const T&  reference;
    typedef std::random_access_iterator_tag iterator_category;

    BOOST_STATIC_CONSTANT(bi_matrix_layout_t, layout = bi_matrix_row_major);

    friend class BiMatrix<T, I, J, layout>;

private:
    ConstBiMatrixColumnIterator(const T* p, size_t size2) BOOST_NOEXCEPT :
        p_(p),
        size2_(size2)
    {
    }

public:
    // Implicit conversion from BiMatrixColumnIterator to ConstBiMatrixColumnIterator.
    ConstBiMatrixColumnIterator(const BiMatrixColumnIterator<T, I, J, layout>& rhs) BOOST_NOEXCEPT :
        p_(rhs.p_),
        size2_(rhs.size2_)
    {
    }

    ConstBiMatrixColumnIterator& operator=(const BiMatrixColumnIterator<T, I, J, layout>& rhs) BOOST_NOEXCEPT
    {
        p_ = rhs.p_;
        size2_ = rhs.size2_;
        return *this;
    }

    // All categories: copy-constructible, copy-assignable and destructible.
    // X b(a); b = a;
    ConstBiMatrixColumnIterator(const ConstBiMatrixColumnIterator& rhs) BOOST_NOEXCEPT :
        p_(rhs.p_),
        size2_(rhs.size2_)
    {
    }

    ConstBiMatrixColumnIterator& operator=(const ConstBiMatrixColumnIterator& rhs) BOOST_NOEXCEPT
    {
        if (this != &rhs)
        {
            p_ = rhs.p_;
            size2_ = rhs.size2_;
        }
        return *this;
    }

    // All categories: can be incremented.
    // ++a; a++;
    ConstBiMatrixColumnIterator& operator++(void) BOOST_NOEXCEPT
    {
        p_ += size2_;
        return *this;
    }

    ConstBiMatrixColumnIterator operator++(int) BOOST_NOEXCEPT
    {
        ConstBiMatrixColumnIterator it(*this);
        p_ += size2_;
        return it;
    }

    // Input: supports equality/inequality comparisons.
    // a == b; a != b;
    bool operator==(const ConstBiMatrixColumnIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ == rhs.p_;
    }

    bool operator!=(const ConstBiMatrixColumnIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ != rhs.p_;
    }

    // Input: can be dereferenced as an rvalue.
    // *a; a->m;
    // Output: can be dereferenced as an lvalue
    // (only for mutable iterator types)
    // *a = t; *a++ = t;
    reference operator*(void) const BOOST_NOEXCEPT
    {
        return *p_;
    }

    pointer operator->(void) const BOOST_NOEXCEPT
    {
        return p_;
    }

    // Forward: input & output.

    // Forward: default-constructible.
    // X a; X();
    ConstBiMatrixColumnIterator(void) BOOST_NOEXCEPT :
        p_(nullptr),
        size2_(0)
    {
    }

    // Forward: multi-pass: neither dereferencing nor incrementing affects
    //                      dereferenceability.
    // { b = a; *a++; *b; }

    // Bidirectional: Forward.

    // Bidirectional: can be decremented.
    // --a; a--; *a--;
    ConstBiMatrixColumnIterator& operator--(void) BOOST_NOEXCEPT
    {
        p_ -= size2_;
        return *this;
    }

    ConstBiMatrixColumnIterator operator--(int) BOOST_NOEXCEPT
    {
        ConstBiMatrixColumnIterator it(*this);
        p_ -= size2_;
        return it;
    }

    // Random access: bidirectional.

    // Random access: supports arithmetic operators + and -.
    // a + n; n + a; a - n; a - b;
    ConstBiMatrixColumnIterator operator+(difference_type n) const BOOST_NOEXCEPT
    {
        return ConstBiMatrixColumnIterator(p_ + n * size2_, size2_);
    }

    ConstBiMatrixColumnIterator operator-(difference_type n) const BOOST_NOEXCEPT
    {
        return ConstBiMatrixColumnIterator(p_ - n * size2_, size2_);
    }

    difference_type operator-(const ConstBiMatrixColumnIterator& rhs) const BOOST_NOEXCEPT
    {
        return (p_ - rhs.p_) / (difference_type)size2_;
    }

    // Random access: supports inequality comparisons (<, >, <= and >=)
    //                between iterators.
    // a < b; a > b; a <= b; a >= b;
    bool operator<(const ConstBiMatrixColumnIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ < rhs.p_;
    }

    bool operator>(const ConstBiMatrixColumnIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ > rhs.p_;
    }

    bool operator<=(const ConstBiMatrixColumnIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ <= rhs.p_;
    }

    bool operator>=(const ConstBiMatrixColumnIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ >= rhs.p_;
    }

    // Random access: supports compound assignment operations += and -=.
    // a += n; a -= n;
    ConstBiMatrixColumnIterator& operator+=(difference_type n) BOOST_NOEXCEPT
    {
        p_ += n * size2_;
        return *this;
    }

    ConstBiMatrixColumnIterator& operator-=(difference_type n) BOOST_NOEXCEPT
    {
        p_ -= n * size2_;
        return *this;
    }

    // Random access: supports offset dereference operator.
    // a[n];
    reference operator[](difference_type n) const BOOST_NOEXCEPT
    {
        return p_[n * size2_];
    }

private:
    pointer p_;
    size_t size2_;
};

/*}}}*/

////////////////////
// Column iterator (column major)./*{{{*/
template<class T, size_t I, size_t J>
class BiMatrixColumnIterator<T, I, J, bi_matrix_column_major>
{
public:
    typedef ptrdiff_t difference_type;
    typedef T         value_type;
    typedef T*        pointer;
    typedef T&        reference;
    typedef std::random_access_iterator_tag iterator_category;

    BOOST_STATIC_CONSTANT(bi_matrix_layout_t, layout = bi_matrix_column_major);

    friend class BiMatrix<T, I, J, layout>;
    friend class ConstBiMatrixColumnIterator<T, I, J, layout>;

private:
    BiMatrixColumnIterator(T* p) BOOST_NOEXCEPT :
        p_(p)
    {
    }

public:
    // All categories: copy-constructible, copy-assignable and destructible.
    // X b(a); b = a;
    BiMatrixColumnIterator(const BiMatrixColumnIterator& rhs) BOOST_NOEXCEPT :
        p_(rhs.p_)
    {
    }

    BiMatrixColumnIterator& operator=(const BiMatrixColumnIterator& rhs) BOOST_NOEXCEPT
    {
        if (this != &rhs)
        {
            p_ = rhs.p_;
        }
        return *this;
    }

    // All categories: can be incremented.
    // ++a; a++;
    BiMatrixColumnIterator& operator++(void) BOOST_NOEXCEPT
    {
        ++p_;
        return *this;
    }

    BiMatrixColumnIterator operator++(int) BOOST_NOEXCEPT
    {
        BiMatrixColumnIterator it(*this);
        ++p_;
        return it;
    }

    // Input: supports equality/inequality comparisons.
    // a == b; a != b;
    bool operator==(const BiMatrixColumnIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ == rhs.p_;
    }

    bool operator!=(const BiMatrixColumnIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ != rhs.p_;
    }

    // Input: can be dereferenced as an rvalue.
    // *a; a->m;
    // Output: can be dereferenced as an lvalue
    // (only for mutable iterator types)
    // *a = t; *a++ = t;
    reference operator*(void) const BOOST_NOEXCEPT
    {
        return *p_;
    }

    pointer operator->(void) const BOOST_NOEXCEPT
    {
        return p_;
    }

    // Forward: input & output.

    // Forward: default-constructible.
    // X a; X();
    BiMatrixColumnIterator(void) BOOST_NOEXCEPT :
        p_(nullptr)
    {
    }

    // Forward: multi-pass: neither dereferencing nor incrementing affects
    //                      dereferenceability.
    // { b = a; *a++; *b; }

    // Bidirectional: Forward.

    // Bidirectional: can be decremented.
    // --a; a--; *a--;
    BiMatrixColumnIterator& operator--(void) BOOST_NOEXCEPT
    {
        --p_;
        return *this;
    }

    BiMatrixColumnIterator operator--(int) BOOST_NOEXCEPT
    {
        BiMatrixColumnIterator it(*this);
        --p_;
        return it;
    }

    // Random access: bidirectional.

    // Random access: supports arithmetic operators + and -.
    // a + n; n + a; a - n; a - b;
    BiMatrixColumnIterator operator+(difference_type n) const BOOST_NOEXCEPT
    {
        return BiMatrixColumnIterator(p_ + n);
    }

    BiMatrixColumnIterator operator-(difference_type n) const BOOST_NOEXCEPT
    {
        return BiMatrixColumnIterator(p_ - n);
    }

    difference_type operator-(const BiMatrixColumnIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ - rhs.p_;
    }

    // Random access: supports inequality comparisons (<, >, <= and >=)
    //                between iterators.
    // a < b; a > b; a <= b; a >= b;
    bool operator<(const BiMatrixColumnIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ < rhs.p_;
    }

    bool operator>(const BiMatrixColumnIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ > rhs.p_;
    }

    bool operator<=(const BiMatrixColumnIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ <= rhs.p_;
    }

    bool operator>=(const BiMatrixColumnIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ >= rhs.p_;
    }

    // Random access: supports compound assignment operations += and -=.
    // a += n; a -= n;
    BiMatrixColumnIterator& operator+=(difference_type n) BOOST_NOEXCEPT
    {
        p_ += n;
        return *this;
    }

    BiMatrixColumnIterator& operator-=(difference_type n) BOOST_NOEXCEPT
    {
        p_ -= n;
        return *this;
    }

    // Random access: supports offset dereference operator.
    // a[n];
    reference operator[](difference_type n) const BOOST_NOEXCEPT
    {
        return p_[n];
    }

private:
    pointer p_;
};

/*}}}*/

////////////////////
// Const column iterator (column major)./*{{{*/
template<class T, size_t I, size_t J>
class ConstBiMatrixColumnIterator<T, I, J, bi_matrix_column_major>
{
public:
    typedef ptrdiff_t difference_type;
    typedef const T   value_type;
    typedef const T*  pointer;
    typedef const T&  reference;
    typedef std::random_access_iterator_tag iterator_category;

    BOOST_STATIC_CONSTANT(bi_matrix_layout_t, layout = bi_matrix_column_major);

    friend class BiMatrix<T, I, J, layout>;

private:
    explicit ConstBiMatrixColumnIterator(const T* p) BOOST_NOEXCEPT :
        p_(p)
    {
    }

public:
    // Implicit conversion from BiMatrixColumnIterator to ConstBiMatrixColumnIterator.
    ConstBiMatrixColumnIterator(const BiMatrixColumnIterator<T, I, J, layout>& rhs) BOOST_NOEXCEPT :
        p_(rhs.p_)
    {
    }

    ConstBiMatrixColumnIterator& operator=(const BiMatrixColumnIterator<T, I, J, layout>& rhs) BOOST_NOEXCEPT
    {
        p_ = rhs.p_;
        return *this;
    }

    // All categories: copy-constructible, copy-assignable and destructible.
    // X b(a); b = a;
    ConstBiMatrixColumnIterator(const ConstBiMatrixColumnIterator& rhs) BOOST_NOEXCEPT :
        p_(rhs.p_)
    {
    }

    ConstBiMatrixColumnIterator& operator=(const ConstBiMatrixColumnIterator& rhs) BOOST_NOEXCEPT
    {
        if (this != &rhs)
        {
            p_ = rhs.p_;
        }
        return *this;
    }

    // All categories: can be incremented.
    // ++a; a++;
    ConstBiMatrixColumnIterator& operator++(void) BOOST_NOEXCEPT
    {
        ++p_;
        return *this;
    }

    ConstBiMatrixColumnIterator operator++(int) BOOST_NOEXCEPT
    {
        ConstBiMatrixColumnIterator it(*this);
        ++p_;
        return it;
    }

    // Input: supports equality/inequality comparisons.
    // a == b; a != b;
    bool operator==(const ConstBiMatrixColumnIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ == rhs.p_;
    }

    bool operator!=(const ConstBiMatrixColumnIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ != rhs.p_;
    }

    // Input: can be dereferenced as an rvalue.
    // *a; a->m;
    // Output: can be dereferenced as an lvalue
    // (only for mutable iterator types)
    // *a = t; *a++ = t;
    reference operator*(void) const BOOST_NOEXCEPT
    {
        return *p_;
    }

    pointer operator->(void) const BOOST_NOEXCEPT
    {
        return p_;
    }

    // Forward: input & output.

    // Forward: default-constructible.
    // X a; X();
    ConstBiMatrixColumnIterator(void) BOOST_NOEXCEPT :
        p_(nullptr)
    {
    }

    // Forward: multi-pass: neither dereferencing nor incrementing affects
    //                      dereferenceability.
    // { b = a; *a++; *b; }

    // Bidirectional: Forward.

    // Bidirectional: can be decremented.
    // --a; a--; *a--;
    ConstBiMatrixColumnIterator& operator--(void) BOOST_NOEXCEPT
    {
        --p_;
        return *this;
    }

    ConstBiMatrixColumnIterator operator--(int) BOOST_NOEXCEPT
    {
        ConstBiMatrixColumnIterator it(*this);
        --p_;
        return it;
    }

    // Random access: bidirectional.

    // Random access: supports arithmetic operators + and -.
    // a + n; n + a; a - n; a - b;
    ConstBiMatrixColumnIterator operator+(difference_type n) const BOOST_NOEXCEPT
    {
        return ConstBiMatrixColumnIterator(p_ + n);
    }

    ConstBiMatrixColumnIterator operator-(difference_type n) const BOOST_NOEXCEPT
    {
        return ConstBiMatrixColumnIterator(p_ - n);
    }

    difference_type operator-(const ConstBiMatrixColumnIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ - rhs.p_;
    }

    // Random access: supports inequality comparisons (<, >, <= and >=)
    //                between iterators.
    // a < b; a > b; a <= b; a >= b;
    bool operator<(const ConstBiMatrixColumnIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ < rhs.p_;
    }

    bool operator>(const ConstBiMatrixColumnIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ > rhs.p_;
    }

    bool operator<=(const ConstBiMatrixColumnIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ <= rhs.p_;
    }

    bool operator>=(const ConstBiMatrixColumnIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ >= rhs.p_;
    }

    // Random access: supports compound assignment operations += and -=.
    // a += n; a -= n;
    ConstBiMatrixColumnIterator& operator+=(difference_type n) BOOST_NOEXCEPT
    {
        p_ += n;
        return *this;
    }

    ConstBiMatrixColumnIterator& operator-=(difference_type n) BOOST_NOEXCEPT
    {
        p_ -= n;
        return *this;
    }

    // Random access: supports offset dereference operator.
    // a[n];
    reference operator[](difference_type n) const BOOST_NOEXCEPT
    {
        return p_[n];
    }

private:
    pointer p_;
};

/*}}}*/

////////////////////
// Random access: supports arithmetic operators + and -.
// a + n; n + a; a - n; a - b;
template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline BiMatrixColumnIterator<T, I, J, S>
operator+(typename BiMatrixColumnIterator<T, I, J, S>::difference_type n,
          const BiMatrixColumnIterator<T, I, J, S>& it) BOOST_NOEXCEPT
{
    return it + n;
}

// Random access: supports arithmetic operators + and -.
// a + n; n + a; a - n; a - b;
template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline ConstBiMatrixColumnIterator<T, I, J, S>
operator+(typename ConstBiMatrixColumnIterator<T, I, J, S>::difference_type n,
          const ConstBiMatrixColumnIterator<T, I, J, S>& it) BOOST_NOEXCEPT
{
    return it + n;
}

// Support mixed operations between BiMatrixIterator and ConstBiMatrixIterator.
template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline bool
operator==(const BiMatrixColumnIterator<T, I, J, S>& lhs,
           const ConstBiMatrixColumnIterator<T, I, J, S>& rhs) BOOST_NOEXCEPT
{
    return rhs == lhs;
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline bool
operator!=(const BiMatrixColumnIterator<T, I, J, S>& lhs,
           const ConstBiMatrixColumnIterator<T, I, J, S>& rhs) BOOST_NOEXCEPT
{
    return rhs != lhs;
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline bool
operator<(const BiMatrixColumnIterator<T, I, J, S>& lhs,
          const ConstBiMatrixColumnIterator<T, I, J, S>& rhs) BOOST_NOEXCEPT
{
    return rhs > lhs;
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline bool
operator>(const BiMatrixColumnIterator<T, I, J, S>& lhs,
          const ConstBiMatrixColumnIterator<T, I, J, S>& rhs) BOOST_NOEXCEPT
{
    return rhs < lhs;
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline bool
operator<=(const BiMatrixColumnIterator<T, I, J, S>& lhs,
           const ConstBiMatrixColumnIterator<T, I, J, S>& rhs) BOOST_NOEXCEPT
{
    return rhs >= lhs;
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline bool
operator>=(const BiMatrixColumnIterator<T, I, J, S>& lhs,
           const ConstBiMatrixColumnIterator<T, I, J, S>& rhs) BOOST_NOEXCEPT
{
    return rhs <= lhs;
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline typename BiMatrixColumnIterator<T, I, J, S>::difference_type
operator-(const BiMatrixColumnIterator<T, I, J, S>& lhs,
          const ConstBiMatrixColumnIterator<T, I, J, S>& rhs) BOOST_NOEXCEPT
{
    return -(rhs - lhs);
}

/*}}}*/

////////////////////////////////////////////////////////////////////////////////
template<class T, size_t I, size_t J, bi_matrix_layout_t S>
class BiMatrix
{
public:
    typedef T         value_type;
    typedef T&        reference;
    typedef const T&  const_reference;
    typedef T*        pointer;
    typedef const T*  const_pointer;
    typedef ptrdiff_t difference_type;
    typedef size_t    size_type;

    typedef detail::bi_matrix<T, I, J, S>           container_type;
    typedef BiMatrixIterator<T, I, J, S>            iterator;
    typedef ConstBiMatrixIterator<T, I, J, S>       const_iterator;
    typedef BiMatrixRowIterator<T, I, J, S>         row_iterator;
    typedef ConstBiMatrixRowIterator<T, I, J, S>    const_row_iterator;
    typedef BiMatrixColumnIterator<T, I, J, S>      column_iterator;
    typedef ConstBiMatrixColumnIterator<T, I, J, S> const_column_iterator;

    BOOST_STATIC_CONSTANT(size_t, base1 = I);
    BOOST_STATIC_CONSTANT(size_t, base2 = J);
    BOOST_STATIC_CONSTANT(bi_matrix_layout_t, layout = S);

public:
    /**
     * @brief Construct an empty matrix.
     */
    BiMatrix(void) BOOST_NOEXCEPT
    {
        bi_matrix_init(&mx_, 0, 0);
    }

    /**
     * @brief Construct an matrix.
     *
     * @param[in] size The size of the matrix.
     */
    BiMatrix(size_type rows, size_type columns)
    {
        BOOST_ASSERT(rows <= max_size1());
        BOOST_ASSERT(columns <= max_size2());
        bi_matrix_init(&mx_, rows, columns);
    }

    ~BiMatrix(void)
    {
        bi_matrix_destroy(&mx_);
    }

    BiMatrix(const BiMatrix& rhs)
    {
        bi_matrix_copy(&rhs.mx_, &mx_);
    }

    BiMatrix& operator=(const BiMatrix& rhs)
    {
        if (this != &rhs)
        {
            bi_matrix_destroy(&mx_);
            bi_matrix_copy(&rhs.mx_, &mx_);
        }
        return *this;
    }

    BiMatrix(BiMatrix&& rhs) BOOST_NOEXCEPT
    {
        bi_matrix_init(&mx_, 0, 0);
        swap(rhs);
    }

    BiMatrix& operator=(BiMatrix&& rhs)
    {
        if (this != &rhs)
        {
            bi_matrix_destroy(&mx_);
            bi_matrix_init(&mx_, 0, 0);
            swap(rhs);
        }
        return *this;
    }

    /**
     * @brief The total number of elements.
     */
    size_type size(void) const BOOST_NOEXCEPT
    {
        return bi_matrix_size(&mx_);
    }

    /**
     * @brief The number of rows.
     */
    size_type size1(void) const BOOST_NOEXCEPT
    {
        return bi_matrix_size1(&mx_);
    }

    /**
     * @brief The number of columns.
     */
    size_type size2(void) const BOOST_NOEXCEPT
    {
        return bi_matrix_size2(&mx_);
    }

    /**
     * @brief The maximum number of rows supported.
     */
    static BOOST_CONSTEXPR size_type max_size1(void) BOOST_NOEXCEPT
    {
        return (size_type)(-1) / 2;
    }

    /**
     * @brief The maximum number of columns supported.
     */
    static BOOST_CONSTEXPR size_type max_size2(void) BOOST_NOEXCEPT
    {
        return (size_type)(-1) / 2;
    }

    bool empty(void) const BOOST_NOEXCEPT
    {
        return !bi_matrix_size1(&mx_) || !bi_matrix_size2(&mx_);
    }

    /**
     * @brief Get the `n`-th element of the matrix.
     *
     * @param[in] n The index of the element.
     *              It **must** be within `[0, size() - 1]`.
     */
    T& operator[](size_type n) BOOST_NOEXCEPT
    {
        return *bi_matrix_at(&mx_, n);
    }

    /**
     * @brief Get the `n`-th element of the matrix.
     *
     * @param[in] n The index of the element.
     *              It **must** be within `[0, size() - 1]`.
     */
    const T& operator[](size_type n) const BOOST_NOEXCEPT
    {
        return *bi_matrix_at(&mx_, n);
    }

    /**
     * @brief Get the `n`-th element of the matrix.
     *
     * @param[in] n The index of the element.
     *              It **must** be within `[0, size() - 1]`.
     */
    T& operator()(size_type n) BOOST_NOEXCEPT
    {
        return *bi_matrix_at(&mx_, n);
    }

    /**
     * @brief Get the `n`-th element of the matrix.
     *
     * @param[in] n The index of the element.
     *              It **must** be within `[0, size() - 1]`.
     */
    const T& operator()(size_type n) const BOOST_NOEXCEPT
    {
        return *bi_matrix_at(&mx_, n);
    }

    /**
     * @brief Get an element.
     *
     * @param[in] i The row index.
     *              It **must** be within `[I, I + size1() - 1]`.
     * @param[in] j The column index.
     *              It **must** be within `[J, J + size2() - 1]`.
     */
    T& operator()(size_type i, size_type j) BOOST_NOEXCEPT
    {
        return *bi_matrix_at(&mx_, i, j);
    }

    /**
     * @brief Get an element.
     *
     * @param[in] i The row index.
     *              It **must** be within `[I, I + size1() - 1]`.
     * @param[in] j The column index.
     *              It **must** be within `[J, J + size2() - 1]`.
     */
    const T& operator()(size_type i, size_type j) const BOOST_NOEXCEPT
    {
        return *bi_matrix_at(&mx_, i, j);
    }

    /**
     * @brief Get an element.
     *
     * @param[in] i The row index.
     *              It **must** be within `[I, I + size1() - 1]`.
     * @param[in] j The column index.
     *              It **must** be within `[J, J + size2() - 1]`.
     */
    T& at(size_type i, size_type j) BOOST_NOEXCEPT
    {
        return *bi_matrix_at(&mx_, i, j);
    }

    /**
     * @brief Get an element.
     *
     * @param[in] i The row index.
     *              It **must** be within `[I, I + size1() - 1]`.
     * @param[in] j The column index.
     *              It **must** be within `[J, J + size2() - 1]`.
     */
    const T& at(size_type i, size_type j) const BOOST_NOEXCEPT
    {
        return *bi_matrix_at(&mx_, i, j);
    }

    /**
     * @brief Get the first element of the matrix.
     */
    T& front(void) BOOST_NOEXCEPT
    {
        return *bi_matrix_front(&mx_);
    }

    /**
     * @brief Get the first element of the matrix.
     */
    const T& front(void) const BOOST_NOEXCEPT
    {
        return *bi_matrix_front(&mx_);
    }

    /**
     * @brief Get the last element of the matrix.
     */
    T& back(void) BOOST_NOEXCEPT
    {
        return *bi_matrix_back(&mx_);
    }

    /**
     * @brief Get the last element of the matrix.
     */
    const T& back(void) const BOOST_NOEXCEPT
    {
        return *bi_matrix_back(&mx_);
    }

    /**
     * @brief Get the storage of the matrix.
     */
    T* data(void) BOOST_NOEXCEPT
    {
        return bi_matrix_data(&mx_);
    }

    /**
     * @brief Get the storage of the matrix.
     */
    const T* data(void) const BOOST_NOEXCEPT
    {
        return bi_matrix_data(&mx_);
    }

    /**
     * @brief Fill the matrix.
     */
    void fill(const T& v)
    {
        bi_matrix_fill(&mx_, v);
    }

    /**
     * @brief Fill a row of the marix.
     *
     * @param[in] i The row.
     *              It **must** be within `[I, I + size1() - 1]`.
     * @param[in] v The value to fill.
     */
    void fill1(size_type i, const T& v)
    {
        bi_matrix_fill1(&mx_, i, v);
    }

    /**
     * @brief Fill a column of the marix.
     *
     * @param[in] j The column.
     *              It **must** be within `[J, J + size2() - 1]`.
     * @param[in] v The value to fill.
     */
    void fill2(size_type j, const T& v)
    {
        bi_matrix_fill2(&mx_, j, v);
    }

    void swap(BiMatrix& rhs) BOOST_NOEXCEPT
    {
        if (this != &rhs)
        {
            boost::swap(mx_.size1_, rhs.mx_.size1_);
            boost::swap(mx_.size2_, rhs.mx_.size2_);
            boost::swap(mx_.data_, rhs.mx_.data_);
        }
    }

    /**
     * @brief Get the begin iterator of the matrix.
     */
    iterator begin(void) BOOST_NOEXCEPT
    {
        return iterator(mx_.data_);
    }

    /**
     * @brief Get the end iterator of the matrix.
     */
    iterator end(void) BOOST_NOEXCEPT
    {
        return iterator(mx_.data_ + size());
    }

    /**
     * @brief Get the begin iterator of the matrix.
     */
    const_iterator begin(void) const BOOST_NOEXCEPT
    {
        return cbegin();
    }

    /**
     * @brief Get the end iterator of the matrix.
     */
    const_iterator end(void) const BOOST_NOEXCEPT
    {
        return cend();
    }

    /**
     * @brief Get the begin iterator of the matrix.
     */
    const_iterator cbegin(void) const BOOST_NOEXCEPT
    {
        return const_iterator(mx_.data_);
    }

    /**
     * @brief Get the end iterator of the matrix.
     */
    const_iterator cend(void) const BOOST_NOEXCEPT
    {
        return const_iterator(mx_.data_ + size());
    }

private:
    template<bi_matrix_layout_t S_> struct storage_t {};

    ////////////////////
    BiMatrixRowIterator<T, I, J, bi_matrix_row_major>
    begin1(size_type i, storage_t<bi_matrix_row_major>) BOOST_NOEXCEPT
    {
        BOOST_ASSERT(I <= i && i < I + mx_.size1_);
        return BiMatrixRowIterator<T, I, J, bi_matrix_row_major>(
                mx_.data_ + (i - I) * mx_.size2_);
    }

    ConstBiMatrixRowIterator<T, I, J, bi_matrix_row_major>
    begin1(size_type i, storage_t<bi_matrix_row_major>) const BOOST_NOEXCEPT
    {
        BOOST_ASSERT(I <= i && i < I + mx_.size1_);
        return ConstBiMatrixRowIterator<T, I, J, bi_matrix_row_major>(
                mx_.data_ + (i - I) * mx_.size2_);
    }

    BiMatrixRowIterator<T, I, J, bi_matrix_column_major>
    begin1(size_type i, storage_t<bi_matrix_column_major>) BOOST_NOEXCEPT
    {
        BOOST_ASSERT(I <= i && i < I + mx_.size1_);
        return BiMatrixRowIterator<T, I, J, bi_matrix_column_major>(
                mx_.data_ + (i - I), mx_.size1_);
    }

    ConstBiMatrixRowIterator<T, I, J, bi_matrix_column_major>
    begin1(size_type i, storage_t<bi_matrix_column_major>) const BOOST_NOEXCEPT
    {
        BOOST_ASSERT(I <= i && i < I + mx_.size1_);
        return ConstBiMatrixRowIterator<T, I, J, bi_matrix_column_major>(
                mx_.data_ + (i - I), mx_.size1_);
    }

    ////////////////////
    BiMatrixColumnIterator<T, I, J, bi_matrix_row_major>
    begin2(size_type j, storage_t<bi_matrix_row_major>) BOOST_NOEXCEPT
    {
        BOOST_ASSERT(J <= j && j < J + mx_.size2_);
        return BiMatrixColumnIterator<T, I, J, bi_matrix_row_major>(
                mx_.data_ + (j - J), mx_.size2_);
    }

    ConstBiMatrixColumnIterator<T, I, J, bi_matrix_row_major>
    begin2(size_type j, storage_t<bi_matrix_row_major>) const BOOST_NOEXCEPT
    {
        BOOST_ASSERT(J <= j && j < J + mx_.size2_);
        return ConstBiMatrixColumnIterator<T, I, J, bi_matrix_row_major>(
                mx_.data_ + (j - J), mx_.size2_);
    }

    BiMatrixColumnIterator<T, I, J, bi_matrix_column_major>
    begin2(size_type j, storage_t<bi_matrix_column_major>) BOOST_NOEXCEPT
    {
        BOOST_ASSERT(J <= j && j < J + mx_.size2_);
        return BiMatrixColumnIterator<T, I, J, bi_matrix_column_major>(
                mx_.data_ + (j - J) * mx_.size1_);
    }

    ConstBiMatrixColumnIterator<T, I, J, bi_matrix_column_major>
    begin2(size_type j, storage_t<bi_matrix_column_major>) const BOOST_NOEXCEPT
    {
        BOOST_ASSERT(J <= j && j < J + mx_.size2_);
        return ConstBiMatrixColumnIterator<T, I, J, bi_matrix_column_major>(
                mx_.data_ + (j - J) * mx_.size1_);
    }

public:
    /**
     * @brief The begin iterator of a row.
     *
     * @param[in] i The row.
     *              It **must** be within `[I, I + size1() - 1]`.
     */
    row_iterator begin1(size_type i) BOOST_NOEXCEPT
    {
        return begin1(i, storage_t<S>());
    }

    /**
     * @brief The end iterator of a row.
     *
     * @param[in] i The row.
     *              It **must** be within `[I, I + size1() - 1]`.
     */
    row_iterator end1(size_type i) BOOST_NOEXCEPT
    {
        return begin1(i) + mx_.size2_;
    }

    /**
     * @brief The begin iterator of a row.
     *
     * @param[in] i The row.
     *              It **must** be within `[I, I + size1() - 1]`.
     */
    const_row_iterator cbegin1(size_type i) const BOOST_NOEXCEPT
    {
        return begin1(i, storage_t<S>());
    }

    /**
     * @brief The end iterator of a row.
     *
     * @param[in] i The row.
     *              It **must** be within `[I, I + size1() - 1]`.
     */
    const_row_iterator cend1(size_type i) const BOOST_NOEXCEPT
    {
        return cbegin1(i) + mx_.size2_;
    }

    /**
     * @brief The begin iterator of a column.
     *
     * @param[in] j The column.
     *              It **must** be within `[J, J + size2() - 1]`.
     */
    column_iterator begin2(size_type j) BOOST_NOEXCEPT
    {
        return begin2(j, storage_t<S>());
    }

    /**
     * @brief The end iterator of a column.
     *
     * @param[in] j The column.
     *              It **must** be within `[J, J + size2() - 1]`.
     */
    column_iterator end2(size_type j) BOOST_NOEXCEPT
    {
        return begin2(j) + mx_.size1_;
    }

    /**
     * @brief The begin iterator of a column.
     *
     * @param[in] j The column.
     *              It **must** be within `[J, J + size2() - 1]`.
     */
    const_column_iterator cbegin2(size_type j) const BOOST_NOEXCEPT
    {
        return begin2(j, storage_t<S>());
    }

    /**
     * @brief The end iterator of a column.
     *
     * @param[in] j The column.
     *              It **must** be within `[J, J + size2() - 1]`.
     */
    const_column_iterator cend2(size_type j) const BOOST_NOEXCEPT
    {
        return cbegin2(j) + mx_.size1_;
    }

private:
    container_type mx_;
};


////////////////////////////////////////
template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline void
swap(BiMatrix<T, I, J, S>& lhs, BiMatrix<T, I, J, S>& rhs) BOOST_NOEXCEPT
{
    lhs.swap(rhs);
}

////////////////////////////////////////
template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline typename BiMatrix<T, I, J, S>::iterator
begin(BiMatrix<T, I, J, S>& mx) BOOST_NOEXCEPT
{
    return mx.begin();
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline typename BiMatrix<T, I, J, S>::iterator
end(BiMatrix<T, I, J, S>& mx) BOOST_NOEXCEPT
{
    return mx.end();
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline typename BiMatrix<T, I, J, S>::const_iterator
begin(const BiMatrix<T, I, J, S>& mx) BOOST_NOEXCEPT
{
    return mx.cbegin();
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline typename BiMatrix<T, I, J, S>::const_iterator
end(const BiMatrix<T, I, J, S>& mx) BOOST_NOEXCEPT
{
    return mx.cend();
}

////////////////////
// Row iterator.
template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline typename BiMatrix<T, I, J, S>::row_iterator
begin1(BiMatrix<T, I, J, S>& mx) BOOST_NOEXCEPT
{
    return mx.begin1();
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline typename BiMatrix<T, I, J, S>::row_iterator
end1(BiMatrix<T, I, J, S>& mx) BOOST_NOEXCEPT
{
    return mx.end1();
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline typename BiMatrix<T, I, J, S>::const_row_iterator
begin1(const BiMatrix<T, I, J, S>& mx) BOOST_NOEXCEPT
{
    return mx.cbegin1();
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline typename BiMatrix<T, I, J, S>::const_row_iterator
end1(const BiMatrix<T, I, J, S>& mx) BOOST_NOEXCEPT
{
    return mx.cend1();
}

////////////////////
// Column iterator.
template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline typename BiMatrix<T, I, J, S>::column_iterator
begin2(BiMatrix<T, I, J, S>& mx) BOOST_NOEXCEPT
{
    return mx.begin2();
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline typename BiMatrix<T, I, J, S>::column_iterator
end2(BiMatrix<T, I, J, S>& mx) BOOST_NOEXCEPT
{
    return mx.end2();
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline typename BiMatrix<T, I, J, S>::const_column_iterator
begin2(const BiMatrix<T, I, J, S>& mx) BOOST_NOEXCEPT
{
    return mx.cbegin2();
}

template<class T, size_t I, size_t J, bi_matrix_layout_t S>
inline typename BiMatrix<T, I, J, S>::const_column_iterator
end2(const BiMatrix<T, I, J, S>& mx) BOOST_NOEXCEPT
{
    return mx.cend2();
}


NSFX_CLOSE_NAMESPACE


#endif // BI_MATRIX_H__B32755F1_A11E_47D0_A318_C215FDDFD5A0
