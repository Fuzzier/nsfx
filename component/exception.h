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
#include <nsfx/exception/exception.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Base.
/**
 * @ingroup Exception
 * @brief The base class for all exceptions in component module.
 */
struct ComponentException : Exception {};


////////////////////////////////////////////////////////////////////////////////
// Object.
/**
 * @ingroup Exception
 * @brief The queried interface is not supported.
 */
struct NoInterface : ComponentException {};

/**
 * @ingroup Exception
 * @brief Practice aggregation in a wrong way.
 */
struct BadAggregation : ComponentException {};


////////////////////////////////////////////////////////////////////////////////
// Object initialization.
/**
 * @ingroup Exception
 * @brief An object is not initialized.
 */
struct Uninitialized : ComponentException {};

/**
 * @ingroup Exception
 * @brief An object cannot be initialized.
 *
 * The reasons may be insufficient or conflicting data.
 */
struct CannotInitialize : ComponentException {};


////////////////////////////////////////////////////////////////////////////////
// Class registration.
/**
 * @ingroup Exception
 * @brief Cannot create an object since its class factory is not registered.
 */
struct ClassNotRegistered : ComponentException {};

/**
 * @ingroup Exception
 * @brief Cannot register a class factory that is already registered.
 */
struct ClassIsRegistered : ComponentException {};


NSFX_CLOSE_NAMESPACE


#endif // EXCEPTION_H__F58D565F_FEC4_4D7A_BE86_F688E62F28BD

