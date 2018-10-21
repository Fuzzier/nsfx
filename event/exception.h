/**
 * @file
 *
 * @brief Event support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-01-14
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef EXCEPTION_H__1D41F747_18D9_452D_AC41_E6695B445E64
#define EXCEPTION_H__1D41F747_18D9_452D_AC41_E6695B445E64


#include <nsfx/event/config.h>
#include <nsfx/component/exception.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Exception
 * @brief The event source cannot accept additional sinks any more.
 */
struct ConnectionLimit : ComponentException {};

/**
 * @ingroup Exception
 * @brief The cookie does not represent a valid connection.
 */
struct NoConnection : ComponentException {};


NSFX_CLOSE_NAMESPACE


#endif // EXCEPTION_H__1D41F747_18D9_452D_AC41_E6695B445E64

