/**
 * @file
 *
 * @brief Exception support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-01-05
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef EXCEPTION_H__BDF60F2C_15F3_4458_ACEE_B1883105F7A4
#define EXCEPTION_H__BDF60F2C_15F3_4458_ACEE_B1883105F7A4


#include <boost/exception/all.hpp>
#include <exception>
#include <string>


NSFX_OPEN_NAMESPACE


/**
 * @defgroup Exception
 * @brief Exceptions.
 */


////////////////////////////////////////////////////////////////////////////////
// Base.
/**
 * @ingroup Exception
 * @brief The base class for all exceptions in NSFX.
 *
 * Multiple inheritance should be avoided for derived exceptions.
 */
struct Exception : boost::exception, std::exception {};

typedef boost::error_info<struct tag_Message, std::string>  ErrorMessage;


////////////////////////////////////////////////////////////////////////////////
// General exceptions.
/**
 * @ingroup Exception
 * @brief Fatal error.
 */
struct Unexpected : Exception {};

// #<{(|*
//  * @ingroup Exception
//  * @brief Out of memory.
//  |)}>#
// struct OutOfMemory: Exception {};

/**
 * @ingroup Exception
 * @brief An index is out of bound.
 */
struct OutOfBounds : Exception {};

/**
 * @ingroup Exception
 * @brief An argument (not a pointer) is invalid.
 */
struct InvalidArgument : Exception {};

/**
 * @ingroup Exception
 * @brief An argument (a pointer) is \c nullptr.
 */
struct InvalidPointer : Exception {};

/**
 * @ingroup Exception
 * @brief The precondition of calling a method is not satisfied.
 */
struct IllegalMethodCall : Exception {};

/**
 * @ingroup Exception
 * @brief A method is not implemented.
 */
struct NotImplemented : Exception {};


NSFX_CLOSE_NAMESPACE


#endif // EXCEPTION_H__BDF60F2C_15F3_4458_ACEE_B1883105F7A4

