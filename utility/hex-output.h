/**
 * @file
 *
 * @brief Output as hexidecimal.
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

#ifndef HEX_OUTPUT_H__9CC13870_7B4E_4790_90A5_72ED31354E6F
#define HEX_OUTPUT_H__9CC13870_7B4E_4790_90A5_72ED31354E6F


#include <nsfx/utility/config.h>
#include <nsfx/utility/tags.h>
#include <iomanip>

/**
 * @ingroup Utility
 * @{
 */

NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Output integral-like value in hexidecimal.
template<class T, bool valid = int_like_tag<T>::value>
struct hex_wrapper;

template<class T>
struct hex_wrapper<T, /*valid*/true>
{
    hex_wrapper(T value) : value_(value) {}
    T value_;
};

////////////////////////////////////////
/**
 * @brief Output a 64-bit integral-like value in hexidecimal.
 *
 * The hexidecimal is delimited by a backtick:
 * <code>0xyyyyyyyy`zzzzzzzz</code>.
 */
template<class Char, class Traits, class T>
inline std::basic_ostream<Char, Traits>&
hex_output(std::basic_ostream<Char, Traits>& os, hex_wrapper<T> v, eq_t<64>)
{
    return os << "0x"
        << std::setw(sizeof (T)) << std::setfill('0') << std::hex
        << std::nouppercase
        << (uint32_t)(((uint64_t)(v.value_) & 0xffffffff00000000) >> 32)
        << '`'
        << std::setw(sizeof (T)) << std::setfill('0') << std::hex
        << std::nouppercase
        << (uint32_t)(((uint64_t)(v.value_) & 0x00000000ffffffff));
}

/**
 * @brief Output an integral-like value (less than 64-bit) in hexidecimal.
 */
template<class Char, class Traits, class T>
inline std::basic_ostream<Char, Traits>&
hex_output(std::basic_ostream<Char, Traits>& os, hex_wrapper<T> v, lt_t<64>)
{
    typedef typename std::make_unsigned<T>::type  U;
    return os << "0x"
              << std::setw(sizeof (T) * 2) << std::setfill('0') << std::hex
              << std::nouppercase << (uint32_t)((U)(v.value_));
}

////////////////////////////////////////
/**
 * @brief Wrap an integral-like value to output in hexidecimal format.
 *
 * @tparam T The type of the value.
 *           It **must** be one of the follow types:
 *           * integral
 *           * pointer
 *           * pointer to member
 *           * enum
 *           * `nullptr_t`
 *
 * @code{.cpp}
 * char a = 'a';
 * std::cout << as_hex(a);
 *
 * int* p = new int();
 * std::cout << as_hex(p);
 * delete p;
 * @endcode
 */
template<class T>
inline hex_wrapper<T> as_hex(T value)
{
    return hex_wrapper<T>(value);
}

////////////////////////////////////////
/**
 * @brief Output integral-like types in hexidecimal format.
 *
 * @see `as_hex()`
 */
template<class Char, class Traits, class T>
inline std::basic_ostream<Char, Traits>&
operator<<(std::basic_ostream<Char, Traits>& os, hex_wrapper<T> v)
{
    typedef typename compare_tag<64, sizeof (T) * 8>::type  tag;
    return hex_output(os, v, tag());
}


NSFX_CLOSE_NAMESPACE

/**
 * @}
 */

#endif // HEX_OUTPUT_H__9CC13870_7B4E_4790_90A5_72ED31354E6F
