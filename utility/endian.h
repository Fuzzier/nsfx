/**
 * @file
 *
 * @brief Utility for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2015-04-14
 *
 * @copyright Copyright (c) 2015.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef ENDIAN_H__75B0F1A3_5D65_4951_BB96_E92E73A2CFF4
#define ENDIAN_H__75B0F1A3_5D65_4951_BB96_E92E73A2CFF4


#include <nsfx/utility/config.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Utility
 * @brief The big endain tag used for function overloading.
 */
enum big_endian_t
{
    big_endian
};

/**
 * @ingroup Utility
 * @brief The little endain tag used for function overloading.
 */
enum little_endian_t
{
    little_endian
};

/**
 * @ingroup Utility
 * @brief The native endain tag used for function overloading.
 */
enum native_endian_t
{
    native_endian
};


////////////////////////////////////////
// Traits.
/**
 * @ingroup Utility
 * @brief The endain traits.
 *
 * @tparam endian_t One of the `big_endian_t`, `little_endian_t` and
 *                  `native_endian_t`.
 *
 * The class provides the following traits:
 * * `is_native`: `bool`, is native endian.
 * * `is_big`: `bool`, is big endian.
 * * `is_little`: `bool`, is little endian.
 * * `endian_t`: type, one of `native_endian_t`, `big_endian_t` and `little_endian_t`.
 */
template<class endian_t>
struct endian_traits;

template<>
struct endian_traits<big_endian_t>
{
#if BOOST_ENDIAN_BIG_BYTE
    BOOST_STATIC_CONSTANT(bool, is_native = true);
#elif BOOST_ENDIAN_LITTLE_BYTE
    BOOST_STATIC_CONSTANT(bool, is_native = false);
#endif // BOOST_ENDIAN_*_BYTE
    BOOST_STATIC_CONSTANT(bool, is_big_endian = true);
    BOOST_STATIC_CONSTANT(bool, is_little_endian = false);
    typedef big_endian_t  endian_t;
};

template<>
struct endian_traits<little_endian_t>
{
#if BOOST_ENDIAN_BIG_BYTE
    BOOST_STATIC_CONSTANT(bool, is_native = false);
#elif BOOST_ENDIAN_LITTLE_BYTE
    BOOST_STATIC_CONSTANT(bool, is_native = true);
#endif // BOOST_ENDIAN_*_BYTE
    BOOST_STATIC_CONSTANT(bool, is_big_endian = false);
    BOOST_STATIC_CONSTANT(bool, is_little_endian = true);
    typedef little_endian_t  endian_t;
};

template<>
struct endian_traits<native_endian_t>
{
    BOOST_STATIC_CONSTANT(bool, is_native = true);
#if BOOST_ENDIAN_BIG_BYTE
    BOOST_STATIC_CONSTANT(bool, is_big_endian = true);
    BOOST_STATIC_CONSTANT(bool, is_little_endian = false);
    typedef big_endian_t  endian_t;
#elif BOOST_ENDIAN_LITTLE_BYTE
    BOOST_STATIC_CONSTANT(bool, is_big_endian = false);
    BOOST_STATIC_CONSTANT(bool, is_little_endian = true);
    typedef little_endian_t  endian_t;
#endif // BOOST_ENDIAN_*_BYTE
};


////////////////////////////////////////
// Traits.
/**
 * @ingroup Utility
 * @brief The first meaningful byte of an integral when only several bits are used.
 *
 * @tparam T    An interal type.
 * @tparam bits The bits used.
 *
 * Denote the byte size of `T` by `s = sizeof (T)`, and denote the number of
 * meaningful bytes by `u = (bits + 7) / 8`.
 * For little endian system, the first meaningful byte is the `0`-th byte.
 * For big endian system, the first meaningful byte is the `(s-u)`-th byte.
 *
 * @verbatim
 * little endian
 * |<- bits ->|
 * |<- size  ->|
 * +---+---+---+------------+
 * | x | x | x |            |
 * +---+---+---+------------+
 *   ^
 *   offset = 0
 * @endverbatim
 *
 * @verbatim
 * big endian
 *               |<- bits ->|
 *              |<- size  ->|
 * +------------+---+---+---+
 * |            | x | x | x |
 * +------------+---+---+---+
 *                ^
 *                offset = sizeof (T) - size
 * @endverbatim
 *
 * The class provides the following traits:
 * * `size`: `size_t`, the number of bytes used to hold the `bits`.
 * * `offset`: `size_t`, the offset of the first meaningful byte.
 */
template<class T, size_t bits>
struct bits_endian_traits;

template<bool is_integral, size_t s, size_t u, class endian_t>
struct bits_endian_traits_impl
{
    BOOST_STATIC_CONSTANT(size_t, size = u);
    BOOST_STATIC_CONSTANT(size_t, offset = 0);
};

template<size_t s, size_t u>
struct bits_endian_traits_impl</*is_integral*/true, s, u, /*endian_t*/big_endian_t>
{
    BOOST_STATIC_CONSTANT(size_t, size = u);
    BOOST_STATIC_CONSTANT(size_t, offset = s - u);
};

template<class T, size_t bits>
struct bits_endian_traits : bits_endian_traits_impl<
    /* is_integral */std::is_integral<T>::value,
    /* s */sizeof (T),
    /* u */(bits+7)/8,
    /* endian_t */typename endian_traits<native_endian_t>::endian_t>
{};


NSFX_CLOSE_NAMESPACE


#endif // ENDIAN_H__75B0F1A3_5D65_4951_BB96_E92E73A2CFF4

