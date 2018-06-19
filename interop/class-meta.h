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

#ifndef CLASS_META_H__D77C068A_E047_4900_A567_B167FCB62514
#define CLASS_META_H__D77C068A_E047_4900_A567_B167FCB62514


#include <nsfx/config.h>
#include <nsfx/exception/exception.h>


////////////////////////////////////////////////////////////////////////////////
class IMethodInfo
{
public:
    virtual ~IMethodInfo(void) BOOST_NOEXCEPT {}

    virtual const char* GetName(void) = 0;
    virtual size_t GetNumArguments(void) = 0;
    virtual const boost::typeindex::type_info& GetReturnType(void) = 0;
    virtual const boost::typeindex::type_info& GetArgumentType(size_t index) = 0;
};


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Interop
 * @brief Interface metadata.
 *
 * # Concept
 *   Use a CID to create a component.
 *   Use the \c IComponentInfo to obtain the IIDs of the exposed interfaces,
 *   and obtain the \c IInterfaceInfo of a specific interface.
 *
 *   Use \c IInterfaceInfo to obtain the prototypes of the exposed methods,
 *   and invoke a specific method on the interface.
 *
 *   The library provides tools to implement \c IComponentInfo and
 *   \c IInterfaceInfo.
 *
 *   @code
 *   cid -> component -> iid -> interface -> method
 *
 *   cid -> component (IObject)
 *   iid -> component (IObject) that exposes IInterfaceInfo
 *   @endcode
 *
 * # How to map an IID to an \c IInterfaceInfo?
 *   \c IInterfaceInfo have to be non-intrusive.
 *   i.e., it cannot be a common virtual method exposed by every interface.
 *   The reason is that, an external user does not know the C++ type of an
 *   interface.
 *   \c IObject::QueryInterface() returns a \c void*, which also losses type.
 *
 */
class IInterfaceInfo
{
public:
    virtual ~IInterfaceInfo(void) BOOST_NOEXCEPT {}

    virtual size_t GetIndex(void) = 0;
    virtual const char* GetUid(void) = 0;
    virtual void VisitMethods(Ptr<IMethodInfoVisitor> visitor) = 0;
    virtual void Invoke(const char* method,
                        Variant& returnValue = empty,
                        Variant& arg1 = empty,
                        Variant& arg2 = empty,
                        Variant& arg3 = empty,
                        Variant& arg4 = empty,
                        Variant& arg5 = empty,
                        Variant& arg6 = empty) = 0;
};


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Interop
 */
class IInterfaceInfoVisitor
{
public:
    virtual ~IInterfaceVisitor(void) BOOST_NOEXCEPT {}

    virtual void Accept(Ptr<IInterfaceInfo> interface) = 0;
};


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Interop
 */
class IObjectInfo
{
public:
    virtual ~IMetaObject(void) BOOST_NOEXCEPT {}

    virtual const char* GetUid(void) = 0;
    virtual void VisitInterfaces(Ptr<IInterfaceInfoVisitor> visitor) = 0;
    virtual Ptr<IInterfaceInfo> GetInterface(const char* iid) = 0;
};


#endif // CLASS_META_H__D77C068A_E047_4900_A567_B167FCB62514

