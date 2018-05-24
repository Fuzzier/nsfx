/**
 * @file
 *
 * @brief Component support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-05-22
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef EXCEPTION_H__F9A5033F_DC55_45C8_B9C2_74C8450F93FF
#define EXCEPTION_H__F9A5033F_DC55_45C8_B9C2_74C8450F93FF


#include <nsfx/network/config.h>
#include <nsfx/component/exception.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Buffer.
/**
 * @ingroup Exception
 * @brief The buffer is lock.
 */
struct BufferLocked : Exception {};

/**
 * @ingroup Exception
 * @brief The buffer does not have enough space.
 */
struct BufferTooSmall : Exception {};


NSFX_CLOSE_NAMESPACE


#endif // EXCEPTION_H__F9A5033F_DC55_45C8_B9C2_74C8450F93FF

