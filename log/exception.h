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
#include <nsfx/component/exception.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Exception
 * @brief Access an log value with a wroing value type.
 */
struct LogValueTypeMismatch : Exception {};

/**
 * @ingroup Exception
 * @brief The named log value cannot be found.
 */
struct LogValueNotFound : Exception {};

/**
 * @ingroup Exception
 * @brief The named log value already exists.
 */
struct LogValueAlreadyExists : Exception {};


////////////////////////////////////////////////////////////////////////////////
// Error info.
typedef boost::error_info<struct tag_QueriedLogValueName, std::string>                   QueriedLogValueNameErrorInfo;
typedef boost::error_info<struct tag_QueriedLogValueType, boost::typeindex::type_index>  QueriedLogValueTypeErrorInfo;
typedef boost::error_info<struct tag_LogValueType,        boost::typeindex::type_index>  LogValueTypeErrorInfo;


NSFX_CLOSE_NAMESPACE


#endif // EXCEPTION_H__DD13BC34_4C11_44BB_A7AF_200B9E081B6C

