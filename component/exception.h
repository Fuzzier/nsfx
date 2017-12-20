/**
 * @file
 *
 * @brief Component support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-14
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef EXCEPTION_H__F58D565F_FEC4_4D7A_BE86_F688E62F28BD
#define EXCEPTION_H__F58D565F_FEC4_4D7A_BE86_F688E62F28BD


#include <nsfx/component/config.h>
#include <boost/exception/all.hpp>
#include <exception>


NSFX_OPEN_NAMESPACE


struct ComponentException : virtual boost::exception, virtual std::exception {};
/**
 * @brief Fatal error.
 */
struct Unexpected : virtual ComponentException {};
struct NotImplemented : virtual ComponentException {};
struct OutOfMemory: virtual ComponentException {};
struct InvalidArgument : virtual ComponentException {};
/**
 * @brief The queried interface is not supported.
 */
struct NoInterface : virtual ComponentException {};
struct InvalidPointer : virtual ComponentException {};
struct OutOfBounds : virtual ComponentException {};
/**
 * @brief The precondition of calling a method is not satisfied.
 */
struct IllegalMethodCall : virtual ComponentException {};
struct AdviseNotSupported : virtual ComponentException {};
struct Uninitialized : virtual ComponentException {};
/**
 * @brief Practice aggregation in a wrong way.
 */
struct BadAggregation : virtual ComponentException {};
/**
 * @brief The object factory does not produce the requested class.
 */
struct ClassNotAvaiable : virtual ComponentException {};
struct ClassNotRegistered : virtual ComponentException {};
struct InterfaceNotRegistered : virtual ComponentException {};
struct BadThreadingModel : virtual ComponentException {};
/**
 * @brief The event provider cannot accept additional connections any more.
 */
struct ConnectionLimit : virtual ComponentException {};
/**
 * @brief The sink does not support the required interface.
 */
struct CannotConnect : virtual ComponentException {};
/**
 * @brief No sink has been connected.
 */
struct NotConnected : virtual ComponentException {};


NSFX_CLOSE_NAMESPACE


#endif // EXCEPTION_H__F58D565F_FEC4_4D7A_BE86_F688E62F28BD

