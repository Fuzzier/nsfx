/**
 * @file
 *
 * @brief Random number support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang (gauchyler@uestc.edu.cn)
 * @date    2018-09-26
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef IS_POWER_2_H__F36AE61B_DD5E_465C_8A95_427A9707D29A
#define IS_POWER_2_H__F36AE61B_DD5E_465C_8A95_427A9707D29A


#include <nsfx/random/config.h>
#include <type_traits> // true_type, false_type


NSFX_OPEN_NAMESPACE


namespace random {
namespace aux {

// A valid state size is a power of 2.
// i.e., 2^k, where k >= 1.
template<size_t n, bool valid = (n>=2) && !(n&1)>
struct is_power_2 : is_valid_state_size<(n>>1)> {};

template<size_t n>
struct is_power_2<n, false> : std::false_type {};

template<>
struct is_power_2<2, true> : std::true_type {};


} // namespace aux
} // namespace random


NSFX_CLOSE_NAMESPACE


#endif // IS_POWER_2_H__F36AE61B_DD5E_465C_8A95_427A9707D29A

