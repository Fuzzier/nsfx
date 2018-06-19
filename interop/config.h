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
 * # Concept
 *   Interoperability is to manipulate components via a standard set of
 *   type-neutral interfaces without knowing the actual C++ types of components
 *   or interfaces.
 *
 *   Interoperability makes it ease to integrate with scripting languages.
 *
 *   e.g., when a GUI creates a component, it wants to show the set of
 *   interfaces exposed by the component, and the set of methods on each
 *   interface.
 *   It can also invoke a method on an interface by supplying arguments, and
 *   displays the return value.
 *
 *   Since a component is manipulated via its interfaces, one cannot distinguish
 *   a component from its interfaces.
 *   Therefore, one cannot let an interface expose an other interface to invoke
 *   the methods on the former interface.
 *   The idea is to let a component expose an interface for interoperability.
 *   e.g., Microsoft COM uses \c IDispatch to support interoperability.
 *
 *
 *   A component exposes \c IObjectInfo interface.
 *   It provides information about the component.
 *   * represent the component itself.
 *   * class id.
 *   * create components that exposes \c IInterfaceInfo.
 *
 *   \c IInterfaceInfo
 *   * interface id.
 *   * methods.
 *
 *   The library provides \c ITypeInfo to support interoperability.
 *   The problem is to reuse \c ITypeInfo.
 *   \c ITypeInfo is not the component itself.
 *
 *
 *   Wants to expose a specific interface, and invoke methods on that interface.
 *   \c ITypeInfo must be a different component.
 *   \c ITypeInfo holds a pointer to the interface, and implements \c Invoke.
 *
 *   \c ITypeInfo is responsible to provide information about an interface,
 *   and invoke the methods.
 *
 *   @code
 *   Interface
 *     ^
 *     |
 *   ITypeInfo
 *   @endcode
 *
 *   To support interoperability, components and interfaces must expose their
 *   meta-data.
 *
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
 *    Although the methods may come from different interfaces.
 *
 * ## Interface-based meta-data
 *    Each interface expose a method to invoke its methods.
 */


#endif // CONFIG_H__F32D83F7_C5B4_435C_8E5E_EBF5A39E0068

