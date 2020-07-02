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
#include <boost/integer.hpp> // uint_t
#include <type_traits>       // enable_if, is_integral


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
 * * `endian_t`: type, one of `big_endian_t` and `little_endian_t`.
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
// Tags and meta-functions.
struct same_byte_order_t {};
struct reverse_byte_order_t {};

namespace aux
{
    template<bool diff_order = false>
    struct byte_order_f
    {
        typedef same_byte_order_t type;
    };

    template<>
    struct byte_order_f</*diffe_order=*/true>
    {
        typedef reverse_byte_order_t type;
    };
} // namespace aux


/**
 * @brief Generate a tag: `endian_t` is native byte order or not.
 *
 * @tparam endian1_t Can be either one of `little_endian_t`, `big_endian_t`,
 *                   or `native_endian_t`.
 *
 * @tparam endian2_t Can be either one of `little_endian_t`, `big_endian_t`,
 *                   or `native_endian_t`.
 *                   <p>
 *                   Defaults to `native_endian_t`.
 *
 * @return If `endian1_t` and `endian2_t` have the same byte order, then `type` is
 *         `same_byte_order_t`.
 *         <p>
 *         Otherwise, `type` is `reverse_byte_order_t`.
 */
template<class endian1_t, class endian2_t = native_endian_t>
struct byte_order_f : aux::byte_order_f<endian_traits<endian1_t>::is_native ^
                                        endian_traits<endian2_t>::is_native>
{};


////////////////////////////////////////////////////////////////////////////////
// Free functions.
namespace aux {

inline uint8_t ReorderBytes(uint8_t v) BOOST_NOEXCEPT
{
    return v;
}

inline uint16_t ReorderBytes(uint16_t v) BOOST_NOEXCEPT
{
    return (uint16_t)((v << 8) | (v >> 8));
}

inline uint32_t ReorderBytes(uint32_t v) BOOST_NOEXCEPT
{
    return ((v & 0x000000ff) << 24)
         | ((v & 0x0000ff00) <<  8)
         | ((v & 0x00ff0000) >>  8)
         | ((v & 0xff000000) >> 24);
}

inline uint64_t ReorderBytes(uint64_t v) BOOST_NOEXCEPT
{
    return ((v & 0x00000000000000ff) << 56)
         | ((v & 0x000000000000ff00) << 40)
         | ((v & 0x0000000000ff0000) << 24)
         | ((v & 0x00000000ff000000) <<  8)
         | ((v & 0x000000ff00000000) >>  8)
         | ((v & 0x0000ff0000000000) >> 24)
         | ((v & 0x00ff000000000000) >> 40)
         | ((v & 0xff00000000000000) >> 56);
}

} // namespace aux

////////////////////////////////////////
/**
 * @brief Reorder bytes of an integral according to the tag.
 *
 * @tparam T The type of the value.
 * @param[in] v The value.
 *              It **must** be an *integral*.
 * @param[in] same_byte_order_t The tag.
 */
template<class T>
inline typename std::enable_if<std::is_integral<T>::value, T>::type
ReorderBytes(T v, same_byte_order_t) BOOST_NOEXCEPT
{
    return v;
}

/**
 * @brief Reorder bytes of an integral according to the tag.
 *
 * @tparam T The type of the value.
 * @param[in] v The value.
 *              It **must** be an *integral*.
 * @param[in] reverse_byte_order_t The tag.
 */
template<class T>
inline typename std::enable_if<std::is_integral<T>::value, T>::type
ReorderBytes(T v, reverse_byte_order_t) BOOST_NOEXCEPT
{
    typedef typename boost::uint_t<sizeof(T) * 8>::exact  V;
    return static_cast<T>(aux::ReorderBytes(static_cast<V>(v)));
}

////////////////////////////////////////
/**
 * @brief Convert an integral from native endian to little endian.
 *
 * @tparam T The type of the value.
 * @param[in] v The value.
 *              It **must** be an *integral*.
 */
template<class T>
inline typename std::enable_if<std::is_integral<T>::value, T>::type
NativeToLittleEndian(T v) BOOST_NOEXCEPT
{
    typedef byte_order_f<little_endian_t>::type  tag;
    return ReorderBytes(v, tag());
}

template<class T>
inline typename std::enable_if<std::is_integral<T>::value, T>::type
LittleToNativeEndian(T v) BOOST_NOEXCEPT
{
    typedef byte_order_f<little_endian_t>::type  tag;
    return ReorderBytes(v, tag());
}

template<class T>
inline typename std::enable_if<std::is_integral<T>::value, T>::type
NativeToBigEndian(T v) BOOST_NOEXCEPT
{
    typedef byte_order_f<big_endian_t>::type  tag;
    return ReorderBytes(v, tag());
}

template<class T>
inline typename std::enable_if<std::is_integral<T>::value, T>::type
BigToNativeEndian(T v) BOOST_NOEXCEPT
{
    typedef byte_order_f<big_endian_t>::type  tag;
    return ReorderBytes(v, tag());
}

template<class T>
inline typename std::enable_if<std::is_integral<T>::value, T>::type
LittleToBigEndian(T v) BOOST_NOEXCEPT
{
    return ReorderBytes(v, reverse_byte_order_t());
}

template<class T>
inline typename std::enable_if<std::is_integral<T>::value, T>::type
BigToLittleEndian(T v) BOOST_NOEXCEPT
{
    return ReorderBytes(v, reverse_byte_order_t());
}


////////////////////////////////////////////////////////////////////////////////
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

