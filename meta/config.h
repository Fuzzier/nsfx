/**
 * @file
 *
 * @brief Interoperability support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-06-16
 *
 * @copyright Copyright (c) 2018.
 *            National Key Laboratory of Science and Technology on Communications,
 *            University of Electronic Science and Technology of China.
 *            All rights reserved.
 */

#ifndef CONFIG_H__F32D83F7_C5B4_435C_8E5E_EBF5A39E0068
#define CONFIG_H__F32D83F7_C5B4_435C_8E5E_EBF5A39E0068


#include <nsfx/config.h>
#include <nsfx/exception/exception.h>


////////////////////////////////////////////////////////////////////////////////
/**
 * @defgroup Interop
 *
 * @brief Interoperability support.
 *
 * # Motivation
 *   Interoperability is to use a component via a mininal standard interface.
 *
 *   For example, when a GUI creates a component, it wants to show the set of
 *   interfaces exposed by the component, and the set of methods on each
 *   interface.
 *   It can also invoke a method on an interface by supplying arguments, and
 *   shows the results.
 *
 *   To support interoperability, components and interfaces must expose their
 *   meta-data.
 *   * The description about the interfaces exposed by a component.
 *     + The UID of the interface.
 *   * The description about the methods supported by an interface.
 *     + The name of the method.
 *     + The name and type of the arguments of the method.
 *     + The type of return value of the method.
 *   * The facility to invoke a method on an interface.
 *
 * # Schemes
 * ## Component-based meta-data
 *    An interface does not provide special means to invoke its methods.
 *    A component expose methods to invoke methods.
 *
 * ## Interface-based meta-data
 */


#endif // CONFIG_H__F32D83F7_C5B4_435C_8E5E_EBF5A39E0068

