/**
 * @file
 *
 * @brief Fixed-size vector with a base index.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2019-12-05
 *
 * @copyright Copyright (c) 2019.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef BI_VECTOR_H__CD6A3F12_904A_4A9D_8542_C3ACA2CD0931
#define BI_VECTOR_H__CD6A3F12_904A_4A9D_8542_C3ACA2CD0931


#include <nsfx/utility/config.h>
#include <nsfx/utility/array-tool.h>
#include <type_traits> // enable_if, is_integral
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
 * @brief An vector with a base index.
 *
 * @tparam T The type of the element.
 * @tparam I The index of the first element.
 */
template<class T, size_t I>
struct bi_vector
{
    size_t capacity_;
    size_t size_;
    T*     data_;
};

////////////////////////////////////////
/**
 * @brief Create an vector.
 *
 * @tparam T The type of the element.
 * @tparam I The index of the first element.
 *
 * @param[out] vt The vector.
 *                It **must** be an *uninitialized* vector.
 * @param[in] capacity The capacity of the vector.
 */
template<class T, size_t I>
inline void
bi_vector_init(bi_vector<T, I>* vt, size_t capacity)
{
    typedef bi_vector<T, I>  vector_type;
    BOOST_ASSERT(vt);
    vt->capacity_ = capacity;
    vt->size_     = 0;
    vt->data_     = nullptr;
    if (capacity)
    {
        vt->data_ = (T*)::operator new (capacity * sizeof (T));
    }
}

/**
 * @brief Create an vector with a default value.
 *
 * @tparam T The type of the element.
 * @tparam I The index of the first element.
 *
 * @param[out] vt The vector.
 *                It **must** be an *uninitialized* vector.
 * @param[in] capacity The capacity of the vector.
 * @param[in] size     The initial size of the vector.
 * @param[in] v        The default value.
 */
template<class T, size_t I>
inline void
bi_vector_init(bi_vector<T, I>* vt, size_t capacity, size_t size, const T& v)
{
    bi_vector_init(vt, capacity);
    if (size)
    {
        vt->size_ = size;
        array_init(vt->data_, vt->size_, v);
    }
}

/**
 * @brief Destroy an vector.
 *
 * @tparam T The type of the element.
 * @tparam I The index of the first element.
 *
 * @param[in,out] vt The vector.
 */
template<class T, size_t I>
inline void
bi_vector_destroy(bi_vector<T, I>* vt)
{
    BOOST_ASSERT(vt);
    array_destroy(vt->data_, vt->size_);
    ::operator delete(vt->data_);
}

/**
 * @brief Copy an vector.
 *
 * @tparam T The type of the element.
 * @tparam I The index of the first element.
 *
 * @param[in]  src The source vector.
 *                 It **must** be an *initialized* vector.
 * @param[out] dst The destination vector.
 *                 It **must** be an *uninitialized* vector.
 */
template<class T, size_t I>
inline void
bi_vector_copy(const bi_vector<T, I>* src, bi_vector<T, I>* dst)
{
    typedef bi_vector<T, I>  vector_type;
    BOOST_ASSERT(src);
    BOOST_ASSERT(dst);
    bi_vector_init(dst, src->capacity_);
    if (src->size_)
    {
        dst->size_ = src->size_;
        array_copy_init(src->data_, dst->data_, src->size_);
    }
}

template<class T, size_t I>
inline size_t
bi_vector_capacity(const bi_vector<T, I>* vt) BOOST_NOEXCEPT
{
    return vt->capacity_;
}

template<class T, size_t I>
inline size_t
bi_vector_size(const bi_vector<T, I>* vt) BOOST_NOEXCEPT
{
    return vt->size_;
}

template<class T, size_t I>
inline T*
bi_vector_at(const bi_vector<T, I>* vt, size_t i) BOOST_NOEXCEPT
{
    BOOST_ASSERT(I <= i && i < I + vt->size_);
    return vt->data_ + (i - I);
}

template<class T, size_t I>
inline T*
bi_vector_front(const bi_vector<T, I>* vt) BOOST_NOEXCEPT
{
    BOOST_ASSERT(vt->size_);
    return vt->data_;
}

template<class T, size_t I>
inline T*
bi_vector_back(const bi_vector<T, I>* vt) BOOST_NOEXCEPT
{
    BOOST_ASSERT(vt->size_);
    return vt->data_ + (vt->size_ - 1);
}

template<class T, size_t I>
inline void
bi_vector_assign(bi_vector<T, I>* vt, size_t n, const T& v)
{
    BOOST_ASSERT(n <= vt->capacity_);
    if (vt->size_ < n)
    {
        array_destroy(vt->data_, vt->size_);
        vt->size_ = n;
    }
    else
    {
        array_destroy(vt->data_, n);
    }
    array_init(vt->data_, n, v);
}

template<class T, size_t I, class InputIterator>
inline typename
std::enable_if<!std::is_integral<InputIterator>::value, void>::type
bi_vector_assign(bi_vector<T, I>* vt,
                 InputIterator first, InputIterator last)
{
    BOOST_ASSERT(first <= last);
    size_t n = last - first;
    BOOST_ASSERT(n <= vt->capacity_);
    if (vt->size_ < n)
    {
        array_destroy(vt->data_, vt->size_);
        vt->size_ = n;
    }
    else
    {
        array_destroy(vt->data_, n);
    }
    array_copy_init(first, last, vt->data_);
}

template<class T, size_t I>
inline void
bi_vector_push_back(bi_vector<T, I>* vt, const T& v)
{
    BOOST_ASSERT(vt->size_ < vt->capacity_);
    T* p = vt->data_ + vt->size_;
    array_init(p, 1, v);
    ++vt->size_;
}

template<class T, size_t I>
inline void
bi_vector_push_back(bi_vector<T, I>* vt, T&& v)
{
    BOOST_ASSERT(vt->size_ < vt->capacity_);
    T* p = vt->data_ + vt->size_;
    array_move_init(p, std::move(v));
    ++vt->size_;
}

template<class T, size_t I>
inline void
bi_vector_pop_back(bi_vector<T, I>* vt)
{
    BOOST_ASSERT(vt->size_);
    T* p = vt->data_ + vt->size_ - 1;
    array_destroy(p, 1);
    --vt->size_;
}

template<class T, size_t I>
inline void
bi_vector_clear(bi_vector<T, I>* vt)
{
    array_destroy(vt->data_, vt->size_);
    vt->size_ = 0;
}

template<class T, size_t I>
inline T*
bi_vector_data(const bi_vector<T, I>* vt) BOOST_NOEXCEPT
{
    return vt->data_;
}

template<class T, size_t I>
inline T*
bi_vector_begin(const bi_vector<T, I>* vt) BOOST_NOEXCEPT
{
    return vt->data_;
}

template<class T, size_t I>
inline T*
bi_vector_end(const bi_vector<T, I>* vt) BOOST_NOEXCEPT
{
    return vt->data_ + vt->size_;
}

} // namespace detail }}}


////////////////////////////////////////////////////////////////////////////////
/**
 * @brief An vector with a base index.
 *
 * @tparam T The type of the element.
 * @tparam I The index of the first element.
 *           Defaults to `0`.
 *
 * The class is **not** copy-constructible nor copy-assignable.
 */
template<class T, size_t I = 0>
class BiVector;

/**
 * @brief Iterator of `BiVector`.
 */
template<class T, size_t I>
class BiVectorIterator;

/**
 * @brief Const iterator of `BiVector`.
 */
template<class T, size_t I>
class ConstBiVectorIterator;


////////////////////////////////////////////////////////////////////////////////
// Iterator./*{{{*/
template<class T, size_t I>
class BiVectorIterator/*{{{*/
{
public:
    typedef ptrdiff_t difference_type;
    typedef T         value_type;
    typedef T*        pointer;
    typedef T&        reference;
    typedef std::random_access_iterator_tag iterator_category;

    friend class BiVector<T, I>;
    friend class ConstBiVectorIterator<T, I>;

private:
    explicit BiVectorIterator(pointer p) BOOST_NOEXCEPT :
        p_(p)
    {
    }

public:
    // All categories: copy-constructible, copy-assignable and destructible.
    // X b(a); b = a;
    BiVectorIterator(const BiVectorIterator& rhs) BOOST_NOEXCEPT :
        p_(rhs.p_)
    {
    }

    BiVectorIterator& operator=(const BiVectorIterator& rhs) BOOST_NOEXCEPT
    {
        if (this != &rhs)
        {
            p_ = rhs.p_;
        }
        return *this;
    }

    // All categories: can be incremented.
    // ++a; a++;
    BiVectorIterator& operator++(void) BOOST_NOEXCEPT
    {
        ++p_;
        return *this;
    }

    BiVectorIterator operator++(int) BOOST_NOEXCEPT
    {
        BiVectorIterator it(*this);
        ++p_;
        return it;
    }

    // Input: supports equality/inequality comparisons.
    // a == b; a != b;
    bool operator==(const BiVectorIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ == rhs.p_;
    }

    bool operator!=(const BiVectorIterator& rhs) const BOOST_NOEXCEPT
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
    BiVectorIterator(void) BOOST_NOEXCEPT :
        p_(nullptr)
    {
    }

    // Forward: multi-pass: neither dereferencing nor incrementing affects
    //                      dereferenceability.
    // { b = a; *a++; *b; }

    // Bidirectional: Forward.

    // Bidirectional: can be decremented.
    // --a; a--; *a--;
    BiVectorIterator& operator--(void) BOOST_NOEXCEPT
    {
        --p_;
        return *this;
    }

    BiVectorIterator operator--(int) BOOST_NOEXCEPT
    {
        BiVectorIterator it(*this);
        --p_;
        return it;
    }

    // Random access: bidirectional.

    // Random access: supports arithmetic operators + and -.
    // a + n; n + a; a - n; a - b;
    BiVectorIterator operator+(difference_type n) const BOOST_NOEXCEPT
    {
        return BiVectorIterator(p_ + n);
    }

    BiVectorIterator operator-(difference_type n) const BOOST_NOEXCEPT
    {
        return BiVectorIterator(p_ - n);
    }

    difference_type operator-(const BiVectorIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ - rhs.p_;
    }

    // Random access: supports inequality comparisons (<, >, <= and >=)
    //                between iterators.
    // a < b; a > b; a <= b; a >= b;
    bool operator<(const BiVectorIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ < rhs.p_;
    }

    bool operator>(const BiVectorIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ > rhs.p_;
    }

    bool operator<=(const BiVectorIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ <= rhs.p_;
    }

    bool operator>=(const BiVectorIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ >= rhs.p_;
    }

    // Random access: supports compound assignment operations += and -=.
    // a += n; a -= n;
    BiVectorIterator& operator+=(difference_type n) BOOST_NOEXCEPT
    {
        p_ += n;
        return *this;
    }

    BiVectorIterator& operator-=(difference_type n) BOOST_NOEXCEPT
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
inline BiVectorIterator<T, I>
operator+(typename BiVectorIterator<T, I>::difference_type n,
          const BiVectorIterator<T, I>& it) BOOST_NOEXCEPT
{
    return it + n;
}

template<class T, size_t I>
class ConstBiVectorIterator/*{{{*/
{
public:
    typedef ptrdiff_t difference_type;
    typedef const T   value_type;
    typedef const T*  pointer;
    typedef const T&  reference;
    typedef std::random_access_iterator_tag iterator_category;

    friend class BiVector<T, I>;

private:
    explicit ConstBiVectorIterator(const T* p) BOOST_NOEXCEPT :
        p_(p)
    {
    }

public:
    // All categories: copy-constructible, copy-assignable and destructible.
    // X b(a); b = a;
    ConstBiVectorIterator(const BiVectorIterator<T, I>& rhs) BOOST_NOEXCEPT :
        p_(rhs.p_)
    {
    }

    ConstBiVectorIterator& operator=(const BiVectorIterator<T, I>& rhs) BOOST_NOEXCEPT
    {
        p_ = rhs.p_;
        return *this;
    }

    ConstBiVectorIterator(const ConstBiVectorIterator& rhs) BOOST_NOEXCEPT :
        p_(rhs.p_)
    {
    }

    ConstBiVectorIterator& operator=(const ConstBiVectorIterator& rhs) BOOST_NOEXCEPT
    {
        if (this != &rhs)
        {
            p_ = rhs.p_;
        }
        return *this;
    }

    // All categories: can be incremented.
    // ++a; a++;
    ConstBiVectorIterator& operator++(void) BOOST_NOEXCEPT
    {
        ++p_;
        return *this;
    }

    ConstBiVectorIterator operator++(int) BOOST_NOEXCEPT
    {
        ConstBiVectorIterator it(*this);
        ++p_;
        return it;
    }

    // Input: supports equality/inequality comparisons.
    // a == b; a != b;
    bool operator==(const ConstBiVectorIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ == rhs.p_;
    }

    bool operator!=(const ConstBiVectorIterator& rhs) const BOOST_NOEXCEPT
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
    ConstBiVectorIterator(void) BOOST_NOEXCEPT :
        p_(nullptr)
    {
    }

    // Forward: multi-pass: neither dereferencing nor incrementing affects
    //                      dereferenceability.
    // { b = a; *a++; *b; }

    // Bidirectional: Forward.

    // Bidirectional: can be decremented.
    // --a; a--; *a--;
    ConstBiVectorIterator& operator--(void) BOOST_NOEXCEPT
    {
        --p_;
        return *this;
    }

    ConstBiVectorIterator operator--(int) BOOST_NOEXCEPT
    {
        ConstBiVectorIterator it(*this);
        --p_;
        return it;
    }

    // Random access: bidirectional.

    // Random access: supports arithmetic operators + and -.
    // a + n; n + a; a - n; a - b;
    ConstBiVectorIterator operator+(difference_type n) const BOOST_NOEXCEPT
    {
        return ConstBiVectorIterator(p_ + n);
    }

    ConstBiVectorIterator operator-(difference_type n) const BOOST_NOEXCEPT
    {
        return ConstBiVectorIterator(p_ - n);
    }

    difference_type operator-(const ConstBiVectorIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ - rhs.p_;
    }

    // Random access: supports inequality comparisons (<, >, <= and >=)
    //                between iterators.
    // a < b; a > b; a <= b; a >= b;
    bool operator<(const ConstBiVectorIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ < rhs.p_;
    }

    bool operator>(const ConstBiVectorIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ > rhs.p_;
    }

    bool operator<=(const ConstBiVectorIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ <= rhs.p_;
    }

    bool operator>=(const ConstBiVectorIterator& rhs) const BOOST_NOEXCEPT
    {
        return p_ >= rhs.p_;
    }

    // Random access: supports compound assignment operations += and -=.
    // a += n; a -= n;
    ConstBiVectorIterator& operator+=(difference_type n) BOOST_NOEXCEPT
    {
        p_ += n;
        return *this;
    }

    ConstBiVectorIterator& operator-=(difference_type n) BOOST_NOEXCEPT
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
inline ConstBiVectorIterator<T, I>
operator+(typename ConstBiVectorIterator<T, I>::difference_type n,
          const ConstBiVectorIterator<T, I>& it) BOOST_NOEXCEPT
{
    return it + n;
}

// Support mixed operations between BiVectorIterator and ConstBiVectorIterator.
template<class T, size_t I>
inline bool
operator==(const BiVectorIterator<T, I>& lhs,
           const ConstBiVectorIterator<T, I>& rhs) BOOST_NOEXCEPT
{
    return rhs == lhs;
}

template<class T, size_t I>
inline bool
operator!=(const BiVectorIterator<T, I>& lhs,
           const ConstBiVectorIterator<T, I>& rhs) BOOST_NOEXCEPT
{
    return rhs != lhs;
}

template<class T, size_t I>
inline bool
operator<(const BiVectorIterator<T, I>& lhs,
          const ConstBiVectorIterator<T, I>& rhs) BOOST_NOEXCEPT
{
    return rhs > lhs;
}

template<class T, size_t I>
inline bool
operator>(const BiVectorIterator<T, I>& lhs,
          const ConstBiVectorIterator<T, I>& rhs) BOOST_NOEXCEPT
{
    return rhs < lhs;
}

template<class T, size_t I>
inline bool
operator<=(const BiVectorIterator<T, I>& lhs,
          const ConstBiVectorIterator<T, I>& rhs) BOOST_NOEXCEPT
{
    return rhs >= lhs;
}

template<class T, size_t I>
inline bool
operator>=(const BiVectorIterator<T, I>& lhs,
          const ConstBiVectorIterator<T, I>& rhs) BOOST_NOEXCEPT
{
    return rhs <= lhs;
}

template<class T, size_t I>
inline typename ConstBiVectorIterator<T, I>::difference_type
operator-(const BiVectorIterator<T, I>& lhs,
          const ConstBiVectorIterator<T, I>& rhs) BOOST_NOEXCEPT
{
    return -(rhs - lhs);
}

/*}}}*/


////////////////////////////////////////////////////////////////////////////////
/**
 * @brief An vector with a base index.
 *
 * @tparam T The type of the element.
 * @tparam I The index of the first element.
 */
template<class T, size_t I>
class BiVector
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

    typedef detail::bi_vector<T, I>     container_type;
    typedef BiVectorIterator<T, I>      iterator;
    typedef ConstBiVectorIterator<T, I> const_iterator;
    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

public:
    /**
     * @brief Construct an empty vector.
     */
    BiVector(void) BOOST_NOEXCEPT
    {
        bi_vector_init(&vt_, 0);
    }

    /**
     * @brief Construct an vector.
     *
     * @param[in] capacity The capacity of the vector.
     */
    explicit BiVector(size_type capacity)
    {
        bi_vector_init(&vt_, capacity);
    }

    /**
     * @brief Construct an vector.
     *
     * @param[in] capacity The capacity of the vector.
     * @param[in] size     The initial size of the vector.
     * @param[in] v        The value of the initial elements of the vector.
     */
    BiVector(size_type capacity, size_t size, const T& v)
    {
        bi_vector_init(&vt_, capacity, size, v);
    }

    ~BiVector(void)
    {
        bi_vector_destroy(&vt_);
    }

    BiVector(const BiVector& rhs)
    {
        bi_vector_copy(&rhs.vt_, &vt_);
    }

    BiVector& operator=(const BiVector& rhs)
    {
        if (&vt_ != &rhs.vt_)
        {
            bi_vector_destroy(&vt_);
            bi_vector_copy(&rhs.vt_, &vt_);
        }
        return *this;
    }

    BiVector(BiVector&& rhs) BOOST_NOEXCEPT
    {
        swap(rhs);
        bi_vector_init(&rhs.vt_, 0);
    }

    BiVector& operator=(BiVector&& rhs)
    {
        if (&vt_ != &rhs.vt_)
        {
            bi_vector_destroy(&vt_);
            bi_vector_init(&vt_, 0);
            swap(rhs);
        }
        return *this;
    }

public:
    /**
     * @brief The capacity of the vector.
     */
    size_type capacity(void) const BOOST_NOEXCEPT
    {
        return bi_vector_capacity(&vt_);
    }

    /**
     * @brief The number of elements of the vector.
     */
    size_type size(void) const BOOST_NOEXCEPT
    {
        return bi_vector_size(&vt_);
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
        return !bi_vector_size(&vt_);
    }

    reference at(size_type i) BOOST_NOEXCEPT
    {
        return *bi_vector_at(&vt_, i);
    }

    const_reference at(size_type i) const BOOST_NOEXCEPT
    {
        return *bi_vector_at(&vt_, i);
    }

    reference operator[](size_type i) BOOST_NOEXCEPT
    {
        return *bi_vector_at(&vt_, i);
    }

    const_reference operator[](size_type i) const BOOST_NOEXCEPT
    {
        return *bi_vector_at(&vt_, i);
    }

    reference operator()(size_type i) BOOST_NOEXCEPT
    {
        return *bi_vector_at(&vt_, i);
    }

    const_reference operator()(size_type i) const BOOST_NOEXCEPT
    {
        return *bi_vector_at(&vt_, i);
    }

    reference front(void) BOOST_NOEXCEPT
    {
        return *bi_vector_front(&vt_);
    }

    const_reference front(void) const BOOST_NOEXCEPT
    {
        return *bi_vector_front(&vt_);
    }

    reference back(void) BOOST_NOEXCEPT
    {
        return *bi_vector_back(&vt_);
    }

    const_reference back(void) const BOOST_NOEXCEPT
    {
        return *bi_vector_back(&vt_);
    }

    pointer data(void) BOOST_NOEXCEPT
    {
        return bi_vector_data(&vt_);
    }

    const_pointer data(void) const BOOST_NOEXCEPT
    {
        return bi_vector_data(&vt_);
    }

    void assign(size_type n, const_reference v)
    {
        bi_vector_assign(&vt_, n, v);
    }

    template<class InputIterator>
    typename std::enable_if<!std::is_integral<InputIterator>::value, void>::type
    assign(InputIterator first, InputIterator last)
    {
        bi_vector_assign(&vt_, first, last);
    }

    void push_back(const_reference v)
    {
        bi_vector_push_back(&vt_, v);
    }

    void push_back(value_type&& v)
    {
        bi_vector_push_back(&vt_, std::move(v));
    }

    void pop_back(void)
    {
        bi_vector_pop_back(&vt_);
    }

    void clear(void)
    {
        bi_vector_clear(&vt_);
    }

    void swap(BiVector& rhs) BOOST_NOEXCEPT
    {
        container_type tmp = vt_;
        vt_ = rhs.vt_;
        rhs.vt_ = tmp;
    }

    ////////////////////
    // Iterators.
    iterator begin(void) BOOST_NOEXCEPT
    {
        return iterator(bi_vector_begin(&vt_));
    }

    iterator end(void) BOOST_NOEXCEPT
    {
        return iterator(bi_vector_end(&vt_));
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
        return const_iterator(bi_vector_begin(&vt_));
    }

    const_iterator cend(void) const BOOST_NOEXCEPT
    {
        return const_iterator(bi_vector_end(&vt_));
    }

    ////////////////////
    // Reverse iterators.
    reverse_iterator rbegin(void) BOOST_NOEXCEPT
    {
        return reverse_iterator(end());
    }

    reverse_iterator rend(void) BOOST_NOEXCEPT
    {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rbegin(void) const BOOST_NOEXCEPT
    {
        return const_reverse_iterator(cend());
    }

    const_reverse_iterator rend(void) const BOOST_NOEXCEPT
    {
        return const_reverse_iterator(cbegin());
    }

    const_reverse_iterator crbegin(void) const BOOST_NOEXCEPT
    {
        return const_reverse_iterator(cend());
    }

    const_reverse_iterator crend(void) const BOOST_NOEXCEPT
    {
        return const_reverse_iterator(cbegin());
    }

private:
    container_type vt_;
};


////////////////////////////////////////
template<class T, size_t I>
inline void swap(BiVector<T, I>& lhs, BiVector<T, I>& rhs) BOOST_NOEXCEPT
{
    lhs.swap(rhs);
}

template<class T, size_t I>
inline typename BiVector<T, I>::iterator
begin(BiVector<T, I>& vt) BOOST_NOEXCEPT
{
    return vt.begin();
}

template<class T, size_t I>
inline typename BiVector<T, I>::iterator
end(BiVector<T, I>& vt) BOOST_NOEXCEPT
{
    return vt.end();
}

template<class T, size_t I>
inline typename BiVector<T, I>::const_iterator
begin(const BiVector<T, I>& vt) BOOST_NOEXCEPT
{
    return vt.begin();
}

template<class T, size_t I>
inline typename BiVector<T, I>::const_iterator
end(const BiVector<T, I>& vt) BOOST_NOEXCEPT
{
    return vt.end();
}


NSFX_CLOSE_NAMESPACE

/**
 * @}
 */

#endif // BI_VECTOR_H__CD6A3F12_904A_4A9D_8542_C3ACA2CD0931

