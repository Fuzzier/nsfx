/**
 * @file
 *
 * @brief Tools to manipulate arrays.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2019-11-29
 *
 * @copyright Copyright (c) 2019.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef ARRAY_TOOL_H__1FD14358_ED7C_449B_9B96_86110106FF09
#define ARRAY_TOOL_H__1FD14358_ED7C_449B_9B96_86110106FF09


#include <nsfx/utility/config.h>
#include <nsfx/utility/tags.h> // pod_tag
#include <cstring> // memset, memcpy


/**
 * @ingroup Utility
 * @{
 */

NSFX_OPEN_NAMESPACE


namespace detail {


////////////////////////////////////////////////////////////////////////////////
template<class T>
inline void array_init(T* p, size_t size, is_pod_t)
{
    std::memset(p, 0, size * sizeof (T));
}

template<class T>
inline void array_init(T* p, size_t size, not_pod_t)
{
    T* last = p + size;
    while (p != last)
    {
        new (p) T();
        ++p;
    }
}

template<class T>
inline void array_init(T* p, size_t size, const T& v, is_pod_t)
{
    T* last = p + size;
    while (p != last)
    {
        *p = v;
        ++p;
    }
}

template<class T>
inline void array_init(T* p, size_t size, const T& v, not_pod_t)
{
    T* last = p + size;
    while (p != last)
    {
        new (p) T(v);
        ++p;
    }
}

////////////////////////////////////////
/**
 * @brief Initialize an array.
 *
 * @tparam T The type of the element.
 *           It **must** be default-constructible.
 *
 * @param[in,out] p    The array.
 * @param[in]     size The size of the array.
 */
template<class T>
inline void array_init(T* p, size_t size)
{
    // is_pod_t or not_pod_t.
    typedef typename pod_tag<T>::type  tag;
    array_init(p, size, tag());
}

/**
 * @brief Initialize an array with a default value.
 *
 * @tparam T The type of the element.
 *           It **must** be copy-constructible.
 *
 * @param[in,out] p    The array.
 * @param[in]     size The size of the array.
 * @param[in]     v    The default value.
 */
template<class T>
inline void array_init(T* p, size_t size, const T& v)
{
    // is_pod_t or not_pod_t.
    typedef typename pod_tag<T>::type  tag;
    array_init(p, size, v, tag());
}


////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Initialize an array by copying from another range.
 *
 * @tparam T The type of the element.
 * @param[in] first The first iterator.
 * @param[in]  last The last iterator.
 * @param[out]  dst The destination array.
 *                  It **must** be an *uninitialized* array.
 */
template<class T, class InputIterator>
inline void array_copy_init(InputIterator first, InputIterator last, T* dst)
{
    while (first != last)
    {
        new (dst) T(*first);
        ++dst;
        ++first;
    }
}

////////////////////////////////////////
template<class T>
inline void array_copy_init(T* src, T* dst, size_t size, is_pod_t)
{
    std::memcpy(dst, src, size * sizeof (T));
}

template<class T>
inline void array_copy_init(T* src, T* dst, size_t size, not_pod_t)
{
    array_copy_init(src, src + size, dst);
}

/**
 * @brief Initialize an array by copying from another array.
 *
 * @tparam T The type of the element.
 * @param[in]  src The source array.
 *                 It **must** be an *initialized* array.
 * @param[out] dst The destination array.
 *                 It **must** be an *uninitialized* array.
 * @param[in] size The size of the array.
 *                 Both arrays **must** have the same size.
 */
template<class T>
inline void array_copy_init(T* src, T* dst, size_t size)
{
    // is_pod_t or not_pod_t.
    typedef typename pod_tag<T>::type  tag;
    array_copy_init(src, dst, size, tag());
}


////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Initialize an element by moving a value.
 *
 * @tparam T The type of the element.
 * @param[in] p The element.
 *              It **must** be an *uninitialized* element.
 * @param[in] v The value.
 */
template<class T>
inline void array_move_init(T* p, T&& v)
{
    new (p) T(std::move(v));
}


////////////////////////////////////////////////////////////////////////////////
template<class T>
inline void array_destroy(T* p, size_t size, is_pod_t)
{
}

template<class T>
inline void array_destroy(T* p, size_t size, not_pod_t)
{
    T* last = p + size;
    while (p != last)
    {
        p->~T();
        ++p;
    }
}

/**
 * @brief Destroy an array.
 *
 * @tparam T The type of the element.
 *
 * @param[in,out] p    The array.
 * @param[in]     size The size of the array.
 */
template<class T>
inline void array_destroy(T* p, size_t size)
{
    // is_pod_t or not_pod_t.
    typedef typename pod_tag<T>::type  tag;
    array_destroy(p, size, tag());
}


} // namespace detail


NSFX_CLOSE_NAMESPACE

/**
 * @}
 */

#endif // ARRAY_TOOL_H__1FD14358_ED7C_449B_9B96_86110106FF09
