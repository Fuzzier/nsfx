/**
 * @file
 *
 * @brief Utility for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2015-02-26
 *
 * @copyright Copyright (c) 2015.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef LEAST_INT_H__B9131CE4_7903_4236_8A07_20EC52595D2D
#define LEAST_INT_H__B9131CE4_7903_4236_8A07_20EC52595D2D


#include <nsfx/utility/config.h>
#include <nsfx/exception/exception.h>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/conditional.hpp>
#include <boost/type_traits/make_signed.hpp>
#include <boost/functional/hash.hpp>
#include <boost/core/swap.hpp>
#include <iostream>
#include <sstream>


NSFX_OPEN_NAMESPACE


/////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Utility
 * @brief The smallest integer that accomandates the specified number of bits.
 *
 * @tparam bits The number of bits. Must be within <code>[0, 64]</code>.
 */
template<size_t bits>
struct LeastInt /*{{{*/
{
    typedef typename boost::conditional<
        bits >= 0 && bits <= 8, uint8_t,
        typename boost::conditional<
            bits >= 9 && bits <= 16, uint16_t,
            typename boost::conditional<
                bits >= 17 && bits <= 32, uint32_t,
                typename boost::conditional<
                    bits >= 33 && bits <= 64, uint64_t, void>::type
                >::type
            >::type
        >::type  UintType;

    static_assert(!boost::is_same<UintType, void>::value,
                  "The number of bits is too large.");

    typedef typename boost::make_signed<UintType>::type  IntType;

    typedef typename boost::conditional<
        bits >= 0 && bits <= 32, uint32_t,
        typename boost::conditional<
            bits >= 33 && bits <= 64, uint64_t, void>::type
        >::type  UintOpType;

    typedef typename boost::make_signed<UintOpType>::type  IntOpType;

};/*}}}*/


NSFX_CLOSE_NAMESPACE


#endif // LEAST_INT_H__B9131CE4_7903_4236_8A07_20EC52595D2D

