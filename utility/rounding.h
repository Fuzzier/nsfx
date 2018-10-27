/**
 * @file
 *
 * @brief Utility for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-10-25
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef ROUNDING_H__A5762225_E058_4B1A_9B5E_AE25E6840949
#define ROUNDING_H__A5762225_E058_4B1A_9B5E_AE25E6840949


#include <nsfx/utility/config.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
enum round_to_zero_t
{
    round_to_zero     ///< Toward zero.
};

enum round_to_nearest_t
{
    round_to_nearest  ///< Toward nearest.
};

enum round_upward_t
{
    round_upward      ///< Toward infinity.
};

enum round_downward_t
{
    round_downward    ///< Toward negative infinity.
};


NSFX_CLOSE_NAMESPACE


#endif // ROUNDING_H__A5762225_E058_4B1A_9B5E_AE25E6840949

