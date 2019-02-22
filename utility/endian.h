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


namespace endian
{


////////////////////////////////////////////////////////////////////////////////
enum Order
{
    little, big,
#if defined(BOOST_ENDIAN_LITTLE_BYTE)
    native = little
#elif defined(BOOST_ENDIAN_BIG_BYTE)
    native = big
#endif // defined(BOOST_ENDIAN_LITTLE_BYTE)
};


} // namespace endian


NSFX_CLOSE_NAMESPACE


#endif // ENDIAN_H__75B0F1A3_5D65_4951_BB96_E92E73A2CFF4

