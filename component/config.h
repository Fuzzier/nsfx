/**
 * @file
 *
 * @brief Component support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-13
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef CONFIG_H__5B879679_8DC6_42C4_A3F8_5CDF1E3F302B
#define CONFIG_H__5B879679_8DC6_42C4_A3F8_5CDF1E3F302B


#include <nsfx/config.h>


/**
 * @defgroup Component
 * @brief Support for component-based programming.
 *
 * Depends upon Exception module.
 */


/**
 * @page Component Component-based programming
 *
 * @includedoc readme.md
 *
 * 1. Component, object and interface
 * ==================================
 *
 *   An interface always represents the object that implements it.
 *   A component can be an aggregation of objects.
 *   The relationship between a component and an interface can be either
 *   <b>`is-a'</b> or <b>`has-a'</b>.
 *   `is-a' means the component is the object that implements the interface.
 *   `has-a' means the component aggregates an object that implements the
 *   *interface*.
 *
 *   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *   interface
 *      | implemented by
 *      V
 *      object
 *         | aggregated into
 *         V
 *         component
 *            | provides/uses
 *            V
 *            interface
 *   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *   For convenience, a user can <b>always</b> assume that a component
 *   `is' an <tt>IObject</tt>, and `has' other interfaces.
 *   i.e., a component implements `IObject` itself, and all other interfaces
 *   are implemented by internally aggregated objects.
 *
 *   When a unique object implements an interface, the component can expose the
 *   interface directly.
 *   When multiple objects implements the same interface, the component can
 *   expose an enumerator interface for users to obtain the required objects.
 *
 *   @retval
 */


NSFX_OPEN_NAMESPACE


NSFX_CLOSE_NAMESPACE


#endif // CONFIG_H__5B879679_8DC6_42C4_A3F8_5CDF1E3F302B

