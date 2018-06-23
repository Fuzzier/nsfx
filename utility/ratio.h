/**
 * @file
 *
 * @brief Utility for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-06-23
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef RATIO_H__402C4FAB_1BA6_4FD0_9E7C_2AA181DD9814
#define RATIO_H__402C4FAB_1BA6_4FD0_9E7C_2AA181DD9814


#include <nsfx/utility/config.h>
#include <boost/ratio.hpp>


NSFX_OPEN_NAMESPACE


using boost::ratio;

using boost::ratio_add;
using boost::ratio_subtract;
using boost::ratio_multiply;
using boost::ratio_divide;

using boost::ratio_equal;
using boost::ratio_not_equal;
using boost::ratio_less;
using boost::ratio_less_equal;
using boost::ratio_greater;
using boost::ratio_greater_equal;

// using boost::yocto;
// using boost::zepto;
using boost::atto;
using boost::femto;
using boost::pico;
using boost::nano;
using boost::micro;
using boost::milli;
using boost::centi;
using boost::deci;

using boost::deca;
using boost::hecto;
using boost::kilo;
using boost::mega;
using boost::giga;
using boost::tera;
using boost::peta;
using boost::exa;
// using boost::zetta;
// using boost::yotta;


NSFX_CLOSE_NAMESPACE


#endif // RATIO_H__402C4FAB_1BA6_4FD0_9E7C_2AA181DD9814

