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
 * # Discussion
 *   In Microsoft COM, a component implements \c IDispatch to expose methods,
 *   not interfaces.
 *   Uses cannot see the interfaces exposed by a component.
 *
 *   The interoperability in NSFX aims to provide an component-based programming
 *   experiene more naturally.
 *   i.e., users manipulate components via their interfaces.
 *
 * ## Single interface scheme
 *   One way is to let a component class exposes a common interface,
 *   say \c IYanDispatch, that provides a virtual method that accepts an
 *   interface UID, the name of a method, and a set of arguments.
 *   For example,
 *   @code
 *   virtual void Invoke(const Uid& iid,
 *                       const char* method,
 *                       Variant ret,
 *                       size_t numArguments, Variant* args) = 0;
 *   @endcode
 *   However, the virtual method queries an interface every time.
 *   Since interface querying is slow, the virtual method is slow.
 *   This may be the reason why Microsoft COM makes the \c iid parameter of
 *   \c IDispatch::Invoke() reserved.
 *
 * ## Multi-interface scheme
 *    Another way is to let a component class exposes a common interface,
 *    say \c IObjectInfo, that provides a virtual method that accepts an
 *    interface UID, and returns a pointer to \c IInterfaceInfo to manipulate
 *    the interface.
 *    For example,
 *    @code
 *    virtual Ptr<IInterfaceInfo> QueryInterface(const Uid& iid) = 0;
 *    @endcode
 *
 *    Since a component cannot expose an interface with different
 *    implementations, the \c IInterfaceInfo for different interfaces has
 *    to be implemented by different components.
 *    i.e., \c IInterfaceInfo is implemented by components that are different
 *    from the component that provides the interface described by
 *    \c IInterfaceInfo.
 *
 *    \c IInterfaceInfo provides a virtual method to invoke the methods exposed
 *    by an interface.
 *    @code
 *    virtual void Invoke(const char* method,
 *                        Variant ret,
 *                        size_t numArguments,
 *                        Variant* args) = 0;
 *    @endcode
 *
 *    Lookup a method via its name is slow, and that is why Microsoft COM uses
 *    an integral index to lookup methods.
 *
 *    \c IInterfaceInfo also provides a virtual method that returns
 *    \c IMethodInfo to allow users invoke a method directly.
 *
 *    Multi-interface scheme is a trade-off between performance and ease-of-use.
 *    Use a human-readable name is much more straight forward than an integer,
 *    but there is no free launch.
 *
 * # Design
 * ## Interfaces
 *   \c IObjectInfo represents a component, which is implemented by the
 *   component.
 *   It holds a pointer to the \c IObjectInfo interface of the component.
 *
 *   \c IInterfaceInfo represents an interface exposed by a component, which
 *   is implemented by a different component.
 *   It holds a pointer to the specific interface of the component.
 *   Operating upon \c IInterfaceInfo is operating upon the component.
 *
 *   \c IMethodInfo represents a method on an interface exposed by a component,
 *   which is implemented by a different component.
 *   It also holds a pointer to the specific interface of the component.
 *   Operating upon \c IMethodInfo is operating upon the component.
 *
 *   @code
 *   component --------- interface ------------ method
 *     ^                   ^                      ^
 *     |                   |                      |
 *     |         creates   |            creates   |
 *   IObjectInfo ------> IInterfaceInfo ------> IMethodInfo
 *     ^                   ^                      ^
 *     |                   |                      |
 *     |                   |            Implemented by a different component
 *     |                   |
 *     |         Implemented by a different component
 *     |
 *   Implemented by the component itself
 *   @endcode
 *
 * ## Requirements on methods
 *    The interoperability library supports a limited set of types of return
 *    value and parameters.
 *
 * ### Return type
 *     The following return types are supported.
 *     Const and const-reference of the these types (if possible) are also
 *     supported.
 *
 * #### \c void
 *      The return type of \c void is supported.
 *      * \c void
 *
 *      If the return type is \c void, or the users do not want to get the
 *      return value, the users shall pass an empty variant to
 *      \c IMethodInfo::Invoke().
 *
 *      e.g.,
 *      @code
 *      // Method prototype.
 *      void Foo(...);
 *
 *      // Invoke via interoperability.
 *      Ptr<IMethodInfo> method = ...;
 *      // Pass an empty variant for void return type, or the return value is
 *      // not required.
 *      method->Invoke(Variant(), ...);
 *      @endcode
 *
 * #### Boolean, integer and floating point
 *      The following return types are supported.
 *      @code
 *      bool
 *      int8_t,  uint8_t
 *      int16_t, uint16_t
 *      int32_t, uint32_t
 *      int64_t, uint64_t
 *      float,   double
 *      @endcode
 *
 *      The return value is carried back by a variant, which has an appropriate
 *      \c vartype with the \c VT_BYREF flag set.
 *
 *      e.g.,
 *      @code
 *      // Method prototype.
 *      double Foo(...);
 *
 *      // Refer to a value directly.
 *      double ret1 = 0.0;
 *      method->Invoke(Variant(&ret1), ...);
 *
 *      // Pass a variant 'ByRef'.
 *      Variant ret2(0.0);
 *      method->Invoke(ret2.MakeRef(), ...);
 *      std::cout << ret2.Get<double>() << std::endl;
 *      @endcode
 *
 * #### String
 *      The following return types are supported.
 *      @code
 *      const char*
 *      std::string
 *      @endcode
 *
 *      The library allocates a buffer to hold the returned string, and buffer
 *      is carried back by a variant with its \c vartype set to
 *      <code>VT_STRING | VT_BYREF</code>.
 *
 *      e.g.,
 *      @code
 *      // Method prototype.
 *      const char* Foo(...);
 *      std::string Bar(...);
 *
 *      // Refer to a C-string directly.
 *      const char* s1 = nullptr;
 *      method->Invoke(Variant(&s1), ...);
 *      // Users are responsible to deallocate the string!
 *      VariantDeallocateString(s1);
 *
 *      // Pass a variant 'ByRef'.
 *      Variant s2("");
 *      method->Invoke(s2.MakeRef(), ...);
 *      std::cout << s2.Get<const char*>() << std::endl;
 *      @endcode
 *
 * #### Interface
 *      The following return types are supported.
 *      @code
 *      Ptr<I>
 *      @endcode
 *
 *      Raw pointer to an interface is <b>not</b> supported.
 *
 *      The component that exposes the interface must also support
 *      \c IObjectInfo.
 *      The library obtains a pointer to \c IObjectInfo by querying the
 *      returned interface.
 *
 *      The pointer to \c IObjectInfo is then carried back to users by a
 *      \c Variant with its type set to <code>VT_OBJECT_INFO | VT_BYREF</code>.
 *
 *      e.g.,
 *      @code
 *      // Refer to a value directly.
 *      Ptr<IObject>  o1;
 *      method->Invoke(Variant(&o1), ...);
 *
 *      // Pass a variant 'ByRef'.
 *      Variant o2(Ptr<IObject>());
 *      method->Invoke(o2.MakeRef(), ...);
 *      @endcode
 *
 *      \c IInterfaceInfo and \c IMethodInfo are not supported.
 *
 * ### Argument type
 *     The following argument types are supported.
 *     Const and const-reference of the these types (if possible) are also
 *     supported.
 *
 * #### Boolean, integer and floating point
 *      The following return types are supported.
 *      @code
 *      bool
 *      int8_t,  uint8_t
 *      int16_t, uint16_t
 *      int32_t, uint32_t
 *      int64_t, uint64_t
 *      float,   double
 *
 *      bool*
 *      int8_t*,  uint8_t*
 *      int16_t*, uint16_t*
 *      int32_t*, uint32_t*
 *      int64_t*, uint64_t*
 *      float*,   double*
 *      @endcode
 *
 *      Please note: const pointer and const-reference to pointer are supported,
 *      but pointer to const value is not supported.
 *      e.g., <code>bool* const</code> and <code>boost* const&</code> are
 *      supported, but <code>const bool*</code> is not supported.
 *
 *      e.g.,
 *      @code
 *      // Refer to a value directly.
 *      double ret1;
 *      method->Invoke(Variant(&ret1), ...);
 *
 *      // Pass a variant 'ByRef'.
 *      Variant ret2(false);
 *      method->Invoke(ret2.MakeRef(), ...);
 *      @endcode
 *
 * #### String
 *      The following return types are supported.
 *      @code
 *      const char*
 *      std::string
 *      @endcode
 *
 *      The library allocates a buffer to hold the returned string, and buffer
 *      is carried back by a variant with its \c vartype set to
 *      <code>VT_STRING | VT_BYREF</code>.
 *
 *      e.g.,
 *      @code
 *      // Refer to a value directly.
 *      const char* s1 = nullptr;
 *      method->Invoke(Variant(&s1), ...);
 *      // Users are responsible to deallocate the string buffer!
 *      VariantDeallocate(s1);
 *
 *      // Pass a variant 'ByRef'.
 *      Variant s2("");
 *      method->Invoke(s2.MakeRef(), ...);
 *      @endcode
 *
 * #### Interface
 *      The following return types are supported.
 *      @code
 *      Ptr<I>
 *      @endcode
 *
 *      Raw pointer to an interface is <b>not</b> supported.
 *
 *      The component that exposes the interface must also support
 *      \c IObjectInfo.
 *      The library obtains a pointer to \c IObjectInfo by querying the
 *      returned interface.
 *
 *      The pointer to \c IObjectInfo is then carried back to users by a
 *      \c Variant with its type set to <code>VT_OBJECT_INFO | VT_BYREF</code>.
 *
 *      e.g.,
 *      @code
 *      // Refer to a value directly.
 *      Ptr<IObject>  o1;
 *      method->Invoke(Variant(&o1), ...);
 *
 *      // Pass a variant 'ByRef'.
 *      Variant o2(Ptr<IObject>());
 *      method->Invoke(o2.MakeRef(), ...);
 *      @endcode
 *
 *      \c IInterfaceInfo and \c IMethodInfo are not supported.
 *
 * ## Argument verification
 *    \c IMethodInfo::Invoke() accepts \c Variant as argument.
 *
 * # Interoperate with C
 *   To interoperate with C, a set of functions with C linkage must be provided.
 *   Several conversions must be provided.
 *   * Smart pointer <code>Ptr<></code> and raw interface pointer.
 *   * \c std::string and buffer-based string.
 *   * Exceptions and error codes.
 *
 */


#endif // CONFIG_H__F32D83F7_C5B4_435C_8E5E_EBF5A39E0068

