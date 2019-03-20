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
enum big_endian_t
{
    big_endian
};

enum little_endian_t
{
    little_endian
};

enum native_endian_t
{
    native_endian
};


////////////////////////////////////////
// Traits.
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
};

template<>
struct endian_traits<native_endian_t>
{
    BOOST_STATIC_CONSTANT(bool, is_native = true);
#if BOOST_ENDIAN_BIG_BYTE
    BOOST_STATIC_CONSTANT(bool, is_big_endian = true);
    BOOST_STATIC_CONSTANT(bool, is_little_endian = false);
#elif BOOST_ENDIAN_LITTLE_BYTE
    BOOST_STATIC_CONSTANT(bool, is_big_endian = false);
    BOOST_STATIC_CONSTANT(bool, is_little_endian = true);
#endif // BOOST_ENDIAN_*_BYTE
};


NSFX_CLOSE_NAMESPACE


#endif // ENDIAN_H__75B0F1A3_5D65_4951_BB96_E92E73A2CFF4

