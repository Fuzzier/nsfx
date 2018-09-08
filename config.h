/**
 * @file
 *
 * @brief Configuration.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-06
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */


#ifndef CONFIG_H__544DA909_13DC_4D4F_AD4F_BE62ECF454E2
#define CONFIG_H__544DA909_13DC_4D4F_AD4F_BE62ECF454E2


////////////////////////////////////////////////////////////////////////////////
#define NSFX_VERSION_MAJOR  2
#define NSFX_VERSION_MINOR  0
#define NSFX_VERSION        (NSFX_VERSION_MAJOR * 1000 + NSFX_VERSION_MINOR)


////////////////////////////////////////////////////////////////////////////////
#ifndef __cplusplus
# error A C++ compiler is required.
#endif // !(__cplusplus)


////////////////////////////////////////////////////////////////////////////////
#define NSFX_OPEN_NAMESPACE  namespace nsfx {
#define NSFX_CLOSE_NAMESPACE } /* namespace nsfx */


////////////////////////////////////////////////////////////////////////////////
#include <boost/version.hpp>
// BOOST 1.68.0
#if (BOOST_VERSION != 106800)
# error BOOST version 1.68.0 is supported. However, an other version is used.
#endif // (BOOST_VERSION != 106800)


////////////////////////////////////////////////////////////////////////////////
#include <boost/config.hpp>

// NSFX_MSVC
// NSFX_X86, NSFX_X64
#if defined(BOOST_MSVC)
# define NSFX_MSVC  BOOST_MSVC
# if NSFX_MSVC < 1600 // Lower than Microsoft Visual C++ 2010
#  error Microsoft Visual C++ 2010 or above version is required.
# endif // NSFX_MSVC < 1600

# if defined(_M_IX86)
#  define NSFX_X86 1
# endif // defined(_M_IX86)

# if defined(_M_X64)
#  define NSFX_X64 1
# endif // defined(_M_IX64)
#endif // defined(BOOST_MSVC)


////////////////////////////////////////////////////////////////////////////////
// NSFX_GCC
// NSFX_X86, NSFX_X64
#if defined(BOOST_GCC)
# define NSFX_GCC  BOOST_GCC
# if NSFX_GCC < 40600 // Lower than GCC 4.6.0
#  error GCC version 4.6.0 or above version is required.
# endif // NSFX_GCC < 40700

# if defined(__LP64__)
#  define NSFX_X64 1
# else // if !defined(__LP64__)
#  define NSFX_X86 1
# endif // defined(__LP64__)
#endif // defined(BOOST_GCC)


////////////////////////////////////////////////////////////////////////////////
// nullptr_t
#if defined(NSFX_GCC)
# if NSFX_GCC < 40800
typedef decltype(nullptr) nullptr_t;
# endif // GCC_VERSION < 40800
#endif // defined(NSFX_GCC)


////////////////////////////////////////////////////////////////////////////////
// NSFX_OVERRIDE
#if defined(NSFX_MSVC)
# define NSFX_OVERRIDE override
#endif // defined(NSFX_MSVC)


#if defined(NSFX_GCC)
# if NSFX_GCC >= 40700
#  define NSFX_OVERRIDE override
# else // if !(NSFX_GCC >= 40700)
#  define NSFX_OVERRIDE
# endif // NSFX_GCC >= 40700
#endif // defined(NSFX_GCC)


////////////////////////////////////////////////////////////////////////////////
// NSFX_FINAL
#if defined(NSFX_MSVC)
# if NSFX_MSVC >= 1700 // Microsoft Visual C++ 2012+
#  define NSFX_FINAL final
# else // if !(NSFX_MSVC >= 1700)
#  define NSFX_FINAL sealed
# endif // NSFX_MSVC >= 1700
#endif // defined(NSFX_MSVC)


#if defined(NSFX_GCC)
# define NSFX_FINAL final
#endif // defined(NSFX_GCC)


////////////////////////////////////////////////////////////////////////////////
// NSFX_EXPLICIT
#if defined(BOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS)
# define NSFX_EXPLICIT
#else // if !defined(BOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS)
# define NSFX_EXPLICIT  explicit
#endif // defined(BOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS)


////////////////////////////////////////////////////////////////////////////////
// Sometimes, BOOST_STATIC_CONSTANT(type, expr) doesn't work, if the second
// parameter is too complex.
// NSFX_BEGIN_TEMPLATE_CONST(type)
// NSFX_NEXT_TEMPLATE_CONST(type)
// NSFX_END_TEMPLATE_CONST()
#if defined(NSFX_MSVC)
# define NSFX_TEMPLATE_CONST_FIRST(type)   BOOST_STATIC_CONSTEXPR type
# define NSFX_TEMPLATE_CONST_NEXT(type)  ; BOOST_STATIC_CONSTEXPR type
# define NSFX_TEMPLATE_CONST_END()       ;
#endif // defined(NSFX_MSVC)


#if defined(NSFX_GCC)
# if !defined(BOOST_NO_CXX11_CONSTEXPR)
#  define NSFX_TEMPLATE_CONST_FIRST(type)   BOOST_STATIC_CONSTEXPR type
#  define NSFX_TEMPLATE_CONST_NEXT(type)  ; BOOST_STATIC_CONSTEXPR type
#  define NSFX_TEMPLATE_CONST_END()       ;
# else // if defined(BOOST_NO_CXX11_CONSTEXPR)
#  define NSFX_TEMPLATE_CONST_FIRST(type) enum {
#  define NSFX_TEMPLATE_CONST_NEXT(type)  ,
#  define NSFX_TEMPLATE_CONST_END()       };
# endif // !defined(BOOST_NO_CXX11_CONSTEXPR)
#endif // defined(NSFX_GCC)


////////////////////////////////////////////////////////////////////////////////
// Import standard integer types.
#include <boost/cstdint.hpp>
NSFX_OPEN_NAMESPACE
using boost::int8_t;
using boost::int16_t;
using boost::int32_t;
using boost::int64_t;
using boost::int_least8_t;
using boost::int_least16_t;
using boost::int_least32_t;
using boost::int_least64_t;
using boost::int_fast8_t;
using boost::int_fast16_t;
using boost::int_fast32_t;
using boost::int_fast64_t;
using boost::intmax_t;
using boost::intptr_t;

using boost::uint8_t;
using boost::uint16_t;
using boost::uint32_t;
using boost::uint64_t;
using boost::uint_least8_t;
using boost::uint_least16_t;
using boost::uint_least32_t;
using boost::uint_least64_t;
using boost::uint_fast8_t;
using boost::uint_fast16_t;
using boost::uint_fast32_t;
using boost::uint_fast64_t;
using boost::uintmax_t;
using boost::uintptr_t;
NSFX_CLOSE_NAMESPACE


#include <cstddef>
NSFX_OPEN_NAMESPACE
using std::size_t;
using std::ptrdiff_t;
typedef ptrdiff_t  refcount_t;
NSFX_CLOSE_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Import standard containers.
#include <boost/functional/hash.hpp>  // use boost::hash<> for extensibility.
#include <boost/container/vector.hpp>
#include <boost/container/deque.hpp>
#include <boost/container/list.hpp>
#include <boost/container/set.hpp>
#include <boost/container/map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>

NSFX_OPEN_NAMESPACE
using boost::container::vector;
using boost::container::deque;
using boost::container::list;
using boost::container::set;
using boost::container::map;
using boost::unordered_set;
using boost::unordered_map;
NSFX_CLOSE_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// assert.
/**
 * @brief The assert statement.
 *
 * ## About \c assert().
 * ### Goodness.
 *     \c assert() costs nothing in a release code that defines \c NDEBUG macro.
 *
 * ### Badness.
 *     \c assert() terminates the program with little information.
 *
 * ## When to use \c assert()?
 * ### 1. Functions with \c noexcept.
 *     If a function is specified as \c noexcept, then use \c assert() as the
 *     last resort.
 *
 * ### 2. Singleton failure.
 *     If a user retrieves a pointer to a singleton, use \c assert() to check
 *     the returned pointer is not \c nullptr.
 *     Usually, the program is beyond redemption in such condition.
 *
 */
#include <boost/assert.hpp>


////////////////////////////////////////////////////////////////////////////////
// Endian-ness.
#include <boost/predef/detail/endian_compat.h>

#if !defined(BOOST_LITTLE_ENDIAN) && !defined(BOOST_BIG_ENDIAN)
# error  Unsupported endianness.
#endif


////////////////////////////////////////////////////////////////////////////////
// The maximum number of function parameters for variadic template emulation.
#if !defined(NSFX_MAX_ARITY)
# define NSFX_MAX_ARITY  6
#endif // !defined(NSFX_MAX_ARITY)


#endif // CONFIG_H__544DA909_13DC_4D4F_AD4F_BE62ECF454E2

