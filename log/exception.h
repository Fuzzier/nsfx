/**
 * @file
 *
 * @brief Exception support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-05-07
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef EXCEPTION_H__DD13BC34_4C11_44BB_A7AF_200B9E081B6C
#define EXCEPTION_H__DD13BC34_4C11_44BB_A7AF_200B9E081B6C


#include <nsfx/log/config.h>


NSFX_LOG_OPEN_NAMESPACE


/**
 * @ingroup Exception
 * @brief The attribute value cannot be found.
 */
struct AttributeValueNotFound : Exception {};

/**
 * @ingroup Exception
 * @brief The attribute cannot be found.
 */
struct AttributeNotFound : Exception {};


NSFX_LOG_CLOSE_NAMESPACE


#endif // EXCEPTION_H__DD13BC34_4C11_44BB_A7AF_200B9E081B6C

