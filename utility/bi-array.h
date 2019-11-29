/**
 * @file
 *
 * @brief Array with a base index.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2019-10-20
 *
 * @copyright Copyright (c) 2019.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef BI_ARRAY_H__1BECB3B6_CA0A_4A46_AB9C_7F2E0838EAEC
#define BI_ARRAY_H__1BECB3B6_CA0A_4A46_AB9C_7F2E0838EAEC


#include <nsfx/utility/config.h>
#include <nsfx/utility/array-tool.h>
#include <boost/core/swap.hpp>
#include <iterator>

/**
 * @ingroup Utility
 * @{
 */

NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
namespace detail /*{{{*/
{

/**
 * @brief An array with a base index.
 *
 * @tparam T The type of the element.
 * @tparam I The index of the first element.
 */
template<class T, size_t I>
struct bi_array
{
    size_t size_;
    T*     data_;
};

////////////////////////////////////////
/**
 * @brief Create an array.
 *
 * @tparam T The type of the element.
 * @tparam I The index of the first element.
 *
 * @param[out] ar   The array.
 *                  It **must** be an *uninitialized* array.
 * @param[in]  size The size of the array.
 *
 * The elements are default initialized.
 */
template<class T, size_t I>
inline void
bi_array_init(bi_array<T, I>* ar, size_t size)
{
    typedef bi_array<T, I>  array_type;
    BOOST_ASSERT(ar);
    ar->size_ = size;
    ar->data_ = nullptr;
    if (size)
    {
        ar->data_ = (T*)::operator new (size * sizeof (T));
        array_init(ar->data_, ar->size_);
    }
}

/**
 * @brief Create an array with a default value.
 *
 * @tparam T The type of the element.
 * @tparam I The index of the first element.
 *
 * @param[out] ar   The array.
 *                  It **must** be an *uninitialized* array.
 * @param[in]  size The size of the array.
 * @param[in]  v    The default value.
 */
template<class T, size_t I>
inline void
bi_array_init(bi_array<T, I>* ar, size_t size, const T& v)
{
    typedef bi_array<T, I>  array_type;
    BOOST_ASSERT(ar);
    ar->size_ = size;
    ar->data_ = nullptr;
    if (size)
    {
        ar->data_ = (T*)::operator new (size * sizeof (T));
        array_init(ar->data_, ar->size_, v);
    }
}

/**
 * @brief Destroy an array.
 *
 * @tparam T The type of the element.
 * @tparam I The index of the first element.
 *
 * @param[in,out] ar The array.
 */
template<class T, size_t I>
inline void
bi_array_destroy(bi_array<T, I>* ar)
{
    BOOST_ASSERT(ar);
    array_destroy(ar->data_, ar->size_);
    ::operator delete(ar->data_);
}

/**
 * @brief Copy an array.
 *
 * @tparam T The type of the element.
 * @tparam I The index of the first element.
 *
 * @param[in]  src The source array.
 *                 It **must** be an *initialized* array.
 * @param[out] dst The destination array.
 *                 It **must** be an *uninitialized* array.
 */
template<class T, size_t I>
inline void
bi_array_copy(const bi_array<T, I>* src, bi_array<T, I>* dst)
{
    typedef bi_array<T, I>  array_type;
    BOOST_ASSERT(src);
    BOOST_ASSERT(dst);
    bi_array_init(dst, 0);
    if (src->size_)
    {
        dst->size_ = src->size_;
        dst->data_ = (T*)::operator new (src->size_ * sizeof (T));
        array_copy_init(src->data_, dst->data_, src->size_);
    }
}

template<class T, size_t I>
inline size_t
bi_array_size(const bi_array<T, I>* ar) BOOST_NOEXCEPT
{
    return ar->size_;
}

template<class T, size_t I>
inline T*
bi_array_at(const bi_array<T, I>* ar, size_t i) BOOST_NOEXCEPT
{
    BOOST_ASSERT(I <= i && i < I + ar->size_);
    return ar->data_ + (i - I);
}

template<class T, size_t I>
inline T*
bi_array_front(const bi_array<T, I>* ar) BOOST_NOEXCEPT
{
    BOOST_ASSERT(ar->size_);
    return ar->data_;
}

template<class T, size_t I>
inline T*
bi_array_back(const bi_array<T, I>* ar) BOOST_NOEXCEPT
{
    BOOST_ASSERT(ar->size_);
    return ar->data_ + (ar->size_ - 1);
}

template<class T, size_t I>
inline void
bi_array_fill(const bi_array<T, I>* ar, const T& v)
{
    T* p = ar->data_;
    T* last = p + ar->size_;
    while (p != last)
    {
        *(p++) = v;
    }
}

template<class T, size_t I>
inline T*
bi_array_data(const bi_array<T, I>* ar) BOOST_NOEXCEPT
{
    return ar->data_;
}

template<class T, size_t I>
inline T*
bi_array_begin(const bi_array<T, I>* ar) BOOST_NOEXCEPT
{
    return ar->data_;
}

template<class T, size_t I>
inline T*
bi_array_end(const bi_array<T, I>* ar) BOOST_NOEXCEPT
{
    return ar->data_ + ar->size_;
}

} // namespace detail }}}


////////////////////////////////////////////////////////////////////////////////
/**
 * @brief An array with a base index.
 *
 * @tparam T The type of the element.
 * @tparam I The index of the first element.
 *           Defaults to `0`.
 *
 * The class is **not** copy-constructible nor copy-assignable.
 */
template<class T, size_t I = 0>
class BiArray;

/**
 * @brief Iterator of `BiArray`.
 */
template<class T, size_t I>
class BiArrayIterator;

/**
 * @brief Const iterator of `BiArray`.
 */
template<class T, size_t I>
class ConstBiArrayIterator;


////////////////////////////////////////////////////////////////////////////////
// Iterator./*{{{*/
template<class T, size_t I>
class BiArrayIterator/*{{{*/
{
public:
    typedef ptrdiff_t difference_type;
    typedef T         value_type;
    typedef T*        pointer;
    typedef T&        reference;
    typedef std::random_access_iterator_tag iterator_category;

    friend class BiArray<T, I>;
    friend class ConstBiArrayIterator<T, I>;

private:
    explicit BiArrayIterator(pointer p) BOOST_NOEXCEPT :
        p_(p)
    {
    }

public:
    // All categories: copy-constructible, copy-assignable and destructible.
    // X b(a); b = a;
    BiArrayIterator(const BiArrayIterator& rhs) BOOST_NOEXCEPT :
        p_(rhs.p_)
    {
    }

    BiArrayIterator& operator=(const BiArrayIterator& rhs) BOOST_NOEXCEPT
    {
        if (this != &rhs)
        {
            p_ = rhs.p_;
        }
        return *this;
    }

    // All categories: can be incremented.
    // ++a; a++;
    BiArrayIterator& operator++(void) BOOST_NOEXCEPT
    {
        ++p_;
        return *this;
    }

    BiArrayIterator operator++(int) BOOST_NOEXCEPT
    {
        BiArrayIterator it(*this);
        ++p_;
        return it;
    }

    // Input: supports equality/inequality comparisons.
    // a == b; a != b;
    bool operator==(const BiArrayIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ == rhs.p_;
    }

    bool operator!=(const BiArrayIterator& rhs) const BOOST_NOEXCEPT
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
    BiArrayIterator(void) BOOST_NOEXCEPT :
        p_(nullptr)
    {
    }

    // Forward: multi-pass: neither dereferencing nor incrementing affects
    //                      dereferenceability.
    // { b = a; *a++; *b; }

    // Bidirectional: Forward.

    // Bidirectional: can be decremented.
    // --a; a--; *a--;
    BiArrayIterator& operator--(void) BOOST_NOEXCEPT
    {
        --p_;
        return *this;
    }

    BiArrayIterator operator--(int) BOOST_NOEXCEPT
    {
        BiArrayIterator it(*this);
        --p_;
        return it;
    }

    // Random access: bidirectional.

    // Random access: supports arithmetic operators + and -.
    // a + n; n + a; a - n; a - b;
    BiArrayIterator operator+(difference_type n) const BOOST_NOEXCEPT
    {
        return BiArrayIterator(p_ + n);
    }

    BiArrayIterator operator-(difference_type n) const BOOST_NOEXCEPT
    {
        return BiArrayIterator(p_ - n);
    }

    difference_type operator-(const BiArrayIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ - rhs.p_;
    }

    // Random access: supports inequality comparisons (<, >, <= and >=)
    //                between iterators.
    // a < b; a > b; a <= b; a >= b;
    bool operator<(const BiArrayIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ < rhs.p_;
    }

    bool operator>(const BiArrayIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ > rhs.p_;
    }

    bool operator<=(const BiArrayIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ <= rhs.p_;
    }

    bool operator>=(const BiArrayIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ >= rhs.p_;
    }

    // Random access: supports compound assignment operations += and -=.
    // a += n; a -= n;
    BiArrayIterator& operator+=(difference_type n) BOOST_NOEXCEPT
    {
        p_ += n;
        return *this;
    }

    BiArrayIterator& operator-=(difference_type n) BOOST_NOEXCEPT
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
};/*}}}*/

// Random access: supports arithmetic operators + and -.
// a + n; n + a; a - n; a - b;
template<class T, size_t I>
inline BiArrayIterator<T, I>
operator+(typename BiArrayIterator<T, I>::difference_type n,
          const BiArrayIterator<T, I>& it) BOOST_NOEXCEPT
{
    return it + n;
}

template<class T, size_t I>
class ConstBiArrayIterator/*{{{*/
{
public:
    typedef ptrdiff_t difference_type;
    typedef const T   value_type;
    typedef const T*  pointer;
    typedef const T&  reference;
    typedef std::random_access_iterator_tag iterator_category;

    friend class BiArray<T, I>;

private:
    explicit ConstBiArrayIterator(const T* p) BOOST_NOEXCEPT :
        p_(p)
    {
    }

public:
    // All categories: copy-constructible, copy-assignable and destructible.
    // X b(a); b = a;
    ConstBiArrayIterator(const BiArrayIterator<T, I>& rhs) BOOST_NOEXCEPT :
        p_(rhs.p_)
    {
    }

    ConstBiArrayIterator& operator=(const BiArrayIterator<T, I>& rhs) BOOST_NOEXCEPT
    {
        p_ = rhs.p_;
        return *this;
    }

    ConstBiArrayIterator(const ConstBiArrayIterator& rhs) BOOST_NOEXCEPT :
        p_(rhs.p_)
    {
    }

    ConstBiArrayIterator& operator=(const ConstBiArrayIterator& rhs) BOOST_NOEXCEPT
    {
        if (this != &rhs)
        {
            p_ = rhs.p_;
        }
        return *this;
    }

    // All categories: can be incremented.
    // ++a; a++;
    ConstBiArrayIterator& operator++(void) BOOST_NOEXCEPT
    {
        ++p_;
        return *this;
    }

    ConstBiArrayIterator operator++(int) BOOST_NOEXCEPT
    {
        ConstBiArrayIterator it(*this);
        ++p_;
        return it;
    }

    // Input: supports equality/inequality comparisons.
    // a == b; a != b;
    bool operator==(const ConstBiArrayIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ == rhs.p_;
    }

    bool operator!=(const ConstBiArrayIterator& rhs) const BOOST_NOEXCEPT
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
    ConstBiArrayIterator(void) BOOST_NOEXCEPT :
        p_(nullptr)
    {
    }

    // Forward: multi-pass: neither dereferencing nor incrementing affects
    //                      dereferenceability.
    // { b = a; *a++; *b; }

    // Bidirectional: Forward.

    // Bidirectional: can be decremented.
    // --a; a--; *a--;
    ConstBiArrayIterator& operator--(void) BOOST_NOEXCEPT
    {
        --p_;
        return *this;
    }

    ConstBiArrayIterator operator--(int) BOOST_NOEXCEPT
    {
        ConstBiArrayIterator it(*this);
        --p_;
        return it;
    }

    // Random access: bidirectional.

    // Random access: supports arithmetic operators + and -.
    // a + n; n + a; a - n; a - b;
    ConstBiArrayIterator operator+(difference_type n) const BOOST_NOEXCEPT
    {
        return ConstBiArrayIterator(p_ + n);
    }

    ConstBiArrayIterator operator-(difference_type n) const BOOST_NOEXCEPT
    {
        return ConstBiArrayIterator(p_ - n);
    }

    difference_type operator-(const ConstBiArrayIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ - rhs.p_;
    }

    // Random access: supports inequality comparisons (<, >, <= and >=)
    //                between iterators.
    // a < b; a > b; a <= b; a >= b;
    bool operator<(const ConstBiArrayIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ < rhs.p_;
    }

    bool operator>(const ConstBiArrayIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ > rhs.p_;
    }

    bool operator<=(const ConstBiArrayIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ <= rhs.p_;
    }

    bool operator>=(const ConstBiArrayIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ >= rhs.p_;
    }

    // Random access: supports compound assignment operations += and -=.
    // a += n; a -= n;
    ConstBiArrayIterator& operator+=(difference_type n) BOOST_NOEXCEPT
    {
        p_ += n;
        return *this;
    }

    ConstBiArrayIterator& operator-=(difference_type n) BOOST_NOEXCEPT
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
};/*}}}*/

// Random access: supports arithmetic operators + and -.
// a + n; n + a; a - n; a - b;
template<class T, size_t I>
inline ConstBiArrayIterator<T, I>
operator+(typename ConstBiArrayIterator<T, I>::difference_type n,
          const ConstBiArrayIterator<T, I>& it) BOOST_NOEXCEPT
{
    return it + n;
}

// Support mixed operations between BiArrayIterator and ConstBiArrayIterator.
template<class T, size_t I>
inline bool
operator==(const BiArrayIterator<T, I>& lhs,
           const ConstBiArrayIterator<T, I>& rhs) BOOST_NOEXCEPT
{
    return rhs == lhs;
}

template<class T, size_t I>
inline bool
operator!=(const BiArrayIterator<T, I>& lhs,
           const ConstBiArrayIterator<T, I>& rhs) BOOST_NOEXCEPT
{
    return rhs != lhs;
}

template<class T, size_t I>
inline bool
operator<(const BiArrayIterator<T, I>& lhs,
          const ConstBiArrayIterator<T, I>& rhs) BOOST_NOEXCEPT
{
    return rhs > lhs;
}

template<class T, size_t I>
inline bool
operator>(const BiArrayIterator<T, I>& lhs,
          const ConstBiArrayIterator<T, I>& rhs) BOOST_NOEXCEPT
{
    return rhs < lhs;
}

template<class T, size_t I>
inline bool
operator<=(const BiArrayIterator<T, I>& lhs,
          const ConstBiArrayIterator<T, I>& rhs) BOOST_NOEXCEPT
{
    return rhs >= lhs;
}

template<class T, size_t I>
inline bool
operator>=(const BiArrayIterator<T, I>& lhs,
          const ConstBiArrayIterator<T, I>& rhs) BOOST_NOEXCEPT
{
    return rhs <= lhs;
}

template<class T, size_t I>
inline typename ConstBiArrayIterator<T, I>::difference_type
operator-(const BiArrayIterator<T, I>& lhs,
          const ConstBiArrayIterator<T, I>& rhs) BOOST_NOEXCEPT
{
    return -(rhs - lhs);
}

/*}}}*/


////////////////////////////////////////////////////////////////////////////////
/**
 * @brief An array with a base index.
 *
 * @tparam T The type of the element.
 * @tparam I The index of the first element.
 */
template<class T, size_t I>
class BiArray
{
public:
    typedef T         value_type;
    typedef T&        reference;
    typedef const T&  const_reference;
    typedef T*        pointer;
    typedef const T*  const_pointer;
    typedef ptrdiff_t difference_type;
    typedef size_t    size_type;

    BOOST_STATIC_CONSTANT(size_t, base = I);

    typedef detail::bi_array<T, I>     container_type;
    typedef BiArrayIterator<T, I>      iterator;
    typedef ConstBiArrayIterator<T, I> const_iterator;

public:
    /**
     * @brief Construct an empty array.
     */
    BiArray(void) BOOST_NOEXCEPT
    {
        bi_array_init(&ar_, 0);
    }

    /**
     * @brief Construct an array.
     *
     * @param[in] size The size of the array.
     *
     * The elements are default initialized.
     */
    explicit BiArray(size_type size)
    {
        bi_array_init(&ar_, size);
    }

    BiArray(size_type size, const T& v)
    {
        bi_array_init(&ar_, size, v);
    }

    ~BiArray(void)
    {
        bi_array_destroy(&ar_);
    }

    BiArray(const BiArray& rhs)
    {
        bi_array_copy(&rhs.ar_, &ar_);
    }

    BiArray& operator=(const BiArray& rhs)
    {
        if (&ar_ != &rhs.ar_)
        {
            bi_array_destroy(&ar_);
            bi_array_copy(&rhs.ar_, &ar_);
        }
        return *this;
    }

    BiArray(BiArray&& rhs) BOOST_NOEXCEPT
    {
        swap(rhs);
        bi_array_init(&rhs.ar_, 0);
    }

    BiArray& operator=(BiArray&& rhs)
    {
        if (&ar_ != &rhs.ar_)
        {
            bi_array_destroy(&ar_);
            bi_array_init(&ar_, 0);
            swap(rhs);
        }
        return *this;
    }

public:
    /**
     * @brief The number of elements of the array.
     */
    size_type size(void) const BOOST_NOEXCEPT
    {
        return bi_array_size(&ar_);
    }

    /**
     * @brief The maximum number of elements supported.
     */
    static BOOST_CONSTEXPR size_type max_size(void) BOOST_NOEXCEPT
    {
        return (size_type)(-1);
    }

    bool empty(void) const BOOST_NOEXCEPT
    {
        return !bi_array_size(&ar_);
    }

    reference at(size_type i) BOOST_NOEXCEPT
    {
        return *bi_array_at(&ar_, i);
    }

    const_reference at(size_type i) const BOOST_NOEXCEPT
    {
        return *bi_array_at(&ar_, i);
    }

    reference operator[](size_type i) BOOST_NOEXCEPT
    {
        return *bi_array_at(&ar_, i);
    }

    const_reference operator[](size_type i) const BOOST_NOEXCEPT
    {
        return *bi_array_at(&ar_, i);
    }

    reference operator()(size_type i) BOOST_NOEXCEPT
    {
        return *bi_array_at(&ar_, i);
    }

    const_reference operator()(size_type i) const BOOST_NOEXCEPT
    {
        return *bi_array_at(&ar_, i);
    }

    reference front(void) BOOST_NOEXCEPT
    {
        return *bi_array_front(&ar_);
    }

    const_reference front(void) const BOOST_NOEXCEPT
    {
        return *bi_array_front(&ar_);
    }

    reference back(void) BOOST_NOEXCEPT
    {
        return *bi_array_back(&ar_);
    }

    const_reference back(void) const BOOST_NOEXCEPT
    {
        return *bi_array_back(&ar_);
    }

    pointer data(void) BOOST_NOEXCEPT
    {
        return bi_array_data(&ar_);
    }

    const_pointer data(void) const BOOST_NOEXCEPT
    {
        return bi_array_data(&ar_);
    }

    void fill(const_reference v)
    {
        bi_array_fill(&ar_, v);
    }

    void swap(BiArray& rhs) BOOST_NOEXCEPT
    {
        boost::swap(ar_.size_, rhs.ar_.size_);
        boost::swap(ar_.data_, rhs.ar_.data_);
    }

    iterator begin(void) BOOST_NOEXCEPT
    {
        return iterator(bi_array_begin(&ar_));
    }

    iterator end(void) BOOST_NOEXCEPT
    {
        return iterator(bi_array_end(&ar_));
    }

    const_iterator begin(void) const BOOST_NOEXCEPT
    {
        return cbegin();
    }

    const_iterator end(void) const BOOST_NOEXCEPT
    {
        return cend();
    }

    const_iterator cbegin(void) const BOOST_NOEXCEPT
    {
        return const_iterator(bi_array_begin(&ar_));
    }

    const_iterator cend(void) const BOOST_NOEXCEPT
    {
        return const_iterator(bi_array_end(&ar_));
    }

private:
    container_type ar_;
};


////////////////////////////////////////
template<class T, size_t I>
inline void swap(BiArray<T, I>& lhs, BiArray<T, I>& rhs) BOOST_NOEXCEPT
{
    lhs.swap(rhs);
}

template<class T, size_t I>
inline typename BiArray<T, I>::iterator
begin(BiArray<T, I>& ar) BOOST_NOEXCEPT
{
    return ar.begin();
}

template<class T, size_t I>
inline typename BiArray<T, I>::iterator
end(BiArray<T, I>& ar) BOOST_NOEXCEPT
{
    return ar.end();
}

template<class T, size_t I>
inline typename BiArray<T, I>::const_iterator
begin(const BiArray<T, I>& ar) BOOST_NOEXCEPT
{
    return ar.begin();
}

template<class T, size_t I>
inline typename BiArray<T, I>::const_iterator
end(const BiArray<T, I>& ar) BOOST_NOEXCEPT
{
    return ar.end();
}


NSFX_CLOSE_NAMESPACE

/**
 * @}
 */

#endif // BI_ARRAY_H__1BECB3B6_CA0A_4A46_AB9C_7F2E0838EAEC
