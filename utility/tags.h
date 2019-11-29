/**
 * @file
 *
 * @brief Type tags.
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

#ifndef TAGS_H__7A78E6FB_7E88_49B2_9462_53011995087D
#define TAGS_H__7A78E6FB_7E88_49B2_9462_53011995087D


#include <nsfx/utility/config.h>
#include <type_traits> // is_pod

/**
 * @ingroup Utility
 * @defgroup Tags Tags
 *
 * Tags are used in function overloading to choose the desired instance of
 * function.
 *
 * @{
 */

NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// is_pod
struct is_pod_t  {};
struct not_pod_t {};

template<class T, bool pod = std::is_pod<T>::value>
struct pod_tag;

template<class T>
struct pod_tag<T, /*pod*/true> : is_pod_t
{
    typedef is_pod_t  type;
    BOOST_STATIC_CONSTANT(bool, value = true);
};

template<class T>
struct pod_tag<T, /*pod*/false> : not_pod_t
{
    typedef not_pod_t  type;
    BOOST_STATIC_CONSTANT(bool, value = false);
};


////////////////////////////////////////////////////////////////////////////////
// arithmetic
struct is_arithmetic_t  {};
struct not_arithmetic_t {};

template<class T, bool arithmetic = std::is_arithmetic<T>::value>
struct arithmetic_tag;

template<class T>
struct arithmetic_tag<T, /*arithmetic*/true> : is_arithmetic_t
{
    typedef is_arithmetic_t  type;
    BOOST_STATIC_CONSTANT(bool, value = true);
};

template<class T>
struct arithmetic_tag<T, /*arithmetic*/false> : not_arithmetic_t
{
    typedef not_arithmetic_t  type;
    BOOST_STATIC_CONSTANT(bool, value = false);
};


////////////////////////////////////////////////////////////////////////////////
// integral
struct is_integral_t  {};
struct not_integral_t {};

template<class T, bool integral = std::is_integral<T>::value>
struct integral_tag;

template<class T>
struct integral_tag<T, /*integral*/true> : is_integral_t
{
    typedef is_integral_t  type;
    BOOST_STATIC_CONSTANT(bool, value = true);
};

template<class T>
struct integral_tag<T, /*integral*/false> : not_integral_t
{
    typedef not_integral_t  type;
    BOOST_STATIC_CONSTANT(bool, value = false);
};


////////////////////////////////////////////////////////////////////////////////
// integral-like
struct is_int_like_t  {};
struct not_int_like_t {};

template<class T, bool int_like = (
    std::is_integral<T>::value       ||
    std::is_pointer<T>::value        ||
    std::is_member_pointer<T>::value ||
    std::is_enum<T>::value           ||
    std::is_same<nullptr_t, typename std::remove_cv<T>::type>::value)>
struct int_like_tag;

template<class T>
struct int_like_tag<T, /*like_integral*/true> : is_int_like_t
{
    typedef is_int_like_t  type;
    BOOST_STATIC_CONSTANT(bool, value = true);
};

template<class T>
struct int_like_tag<T, /*like_integral*/false> : not_int_like_t
{
    typedef not_int_like_t  type;
    BOOST_STATIC_CONSTANT(bool, value = false);
};


////////////////////////////////////////////////////////////////////////////////
// comparison
template<size_t n> struct eq_t {}; ///< Equal to `n`.
template<size_t n> struct ne_t {}; ///< Not equal to `n`.
template<size_t n> struct lt_t {}; ///< Less than `n`.
template<size_t n> struct ge_t {}; ///< Greater than or equal to `n`.
template<size_t n> struct le_t {}; ///< Less than or equal to `n`.
template<size_t n> struct gt_t {}; ///< Greater than `n`.


////////////////////////////////////////
/**
 * @brief Compare `a` and `b`.
 *
 * The member `type` is either one of the following types:
 * * `eq_t<a>`: If `a == b`.
 * * `lt_t<a>`: If `a > b`.
 * * `gt_t<a>`: If `a < b`.
 *
 * The member `inverse_type` is either one of the following types:
 * * `eq_t<b>`: If `a == b`.
 * * `lt_t<b>`: If `a < b`.
 * * `gt_t<b>`: If `a > b`.
 */
template<size_t a, size_t b>
struct compare_tag
{
    typedef typename std::conditional<(a == b),
            eq_t<a>, typename std::conditional<(a < b),
                     gt_t<a>, lt_t<a>
                     >::type
            >::type
    type;

    typedef typename std::conditional<(a == b),
            eq_t<b>, typename std::conditional<(a < b),
                     lt_t<b>, gt_t<b>
                     >::type
            >::type
    inverse_type;
};


NSFX_CLOSE_NAMESPACE

/**
 * @}
 */

#endif // TAGS_H__7A78E6FB_7E88_49B2_9462_53011995087D
