/**
 * @file
 *
 * @brief Packet for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-05-22
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef EXCEPTION_H__2106F3B1_5B26_4D4D_B25C_0FFBDE0B7052
#define EXCEPTION_H__2106F3B1_5B26_4D4D_B25C_0FFBDE0B7052


#include <nsfx/network/config.h>
#include <nsfx/exception/exception.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Exception
 * @brief The requested type mismatches the value type of the tag.
 */
class TagTypeMismatch : public Exception {};

/**
 * @ingroup Exception
 * @brief The tag is not found.
 */
class TagNotFound : public Exception {};


NSFX_CLOSE_NAMESPACE


#endif // EXCEPTION_H__2106F3B1_5B26_4D4D_B25C_0FFBDE0B7052

