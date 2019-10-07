/**
 * @file
 *
 * @brief Component support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-26
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef CLASS_REGISTRY_H__F5D0D382_C0AE_4771_95D9_8DB2A81CCB15
#define CLASS_REGISTRY_H__F5D0D382_C0AE_4771_95D9_8DB2A81CCB15


#include <nsfx/component/config.h>
#include <nsfx/component/i-class-registry.h>
#include <nsfx/component/class-factory.h>
#include <nsfx/component/exception.h>
#include <boost/mpl/identity.hpp>
#include <iostream>


////////////////////////////////////////////////////////////////////////////////
// Macros.
/**
 * @brief A macro that registers a class with a default factory.
 *
 * @param T   It **must** conform to `ObjectImplConcept`.
 *            It **must not** be a qualified name.
 * @param CID The UID of the class.
 *
 * # Requirements
 *
 * The process is that:
 * 1. Component writers associate a CID with a class.
 * 2. The CID is registered during the program startup.
 * 3. Component users use the CID to create instances of the class, and use
 *    interfaces to manipulate the instances.
 *
 * The library provides an inline, self-driven and non-intrusive way for class
 * registration and instance creation.
 *
 * ## Inline
 *
 * Component writers use a macro to associate a CID with a class.
 * Usually, the macro is written along with the class in the same source
 * file.
 * They do not write the information in a separate location, such as
 * Windows Registry or configuration files.
 *
 * ## Self-driven
 *
 * The class registration is performed during program startup, since the
 * information is coded in the program itself.
 * Component users do not need to use loader programs to register the
 * clases, or create instances.
 * This is specially helpful in an environment that forbids the usage of
 * Windows Registry or loader programs.
 *
 * ## Non-intrusive
 *
 * Similar to IID, the macro to associate a CID with a class is placed
 * outside of the class definition.
 *
 * The difference is that, an IID is bound to the type of an interface.
 * That is, an interface can have one and only one IID.
 * However, a CID is associated loosely with a class.
 * That is, a class may have multiple CIDs.
 *
 * ## Header friendly
 *
 * The macro must be able to be placed in header files, so the library
 * remains header-only.
 *
 * # Key idea
 *
 * 1. Define a helper class whose constructor performs the registration.
 * 2. Define a static instance of the helper class, so the constructor is
 *    called during program startup.
 *
 * # Methods
 *
 * ## Global static variable
 *
 * A common method is to defined a static instance of the class directly.
 * @code{.cpp}
 * static struct MyClassRegister
 * {
 *     MyClassRegister(void)
 *     {
 *         ::nsfx::RegisterClassFactory<MyClass>(myCid);
 *     }
 * } s_MyClassRegister;
 * @endcode
 *
 * If the code is placed in a header file, each compilation unit that
 * includes the header file would have a separate instance of the variable.
 * Thus, the constructor must use a static flag to prevent multiple
 * registration.
 * @code{.cpp}
 * static struct MyClassRegister
 * {
 *     MyClassRegister(void)
 *     {
 *         static bool registered = false;
 *         if (!registered)
 *         {
 *             ::nsfx::RegisterClassFactory<MyClass>(myCid);
 *             registered = true;
 *         }
 *     }
 * } s_MyClassRegister;
 * @endcode
 *
 * ## Class member static instance.
 *
 * To prevent the use of an additional static flag, the static instance can
 * be defined as a class member.
 * @code{.cpp}
 * class MyClassRegister
 * {
 *     static struct Helper
 *     {
 *         Helper(void)
 *         {
 *             ::nsfx::RegisterClassFactory<MyClass>(myCid);
 *         }
 *     } helper_;
 * };
 * MyClassRegister::Worker  MyClassRegister::helper_;
 * @endcode
 *
 * If the code is placed in a header file, the instance would be defined in
 * each compilation unit that includes the header file.
 * This results in multiple definition of the instance.
 * The work around, a class template can be used in place of the class.
 * An explicit instantiation of the class template is also defined.
 * @code{.cpp}
 * template<class T>
 * class MyClassRegister
 * {
 *     static struct Helper
 *     {
 *         Helper(void)
 *         {
 *             ::nsfx::RegisterClassFactory<MyClass>(myCid);
 *         }
 *     } helper_;
 * };
 * // Declare the static member variable.
 * template<classs T> typename MyClassRegister<T>::Helper
 *                             MyClassRegister<T>::helper_;
 * // Define an explicit instantiation of the class template.
 * template class MyClassRegister<MyClass>;
 * @endcode
 *
 * # Remarks
 *
 * If a name is local to a compilation unit, it will be stored in a *static
 * library*.
 * However, the local names will be *removed* during linking, since they
 * are neither exported, nor refered by other code.
 *
 * To prevent the removal, the local names must be exported with external
 * linkage, or be refered by some code.
 * Usually, external tools have to be provided to perform such task.
 *
 * Therefore, for code that is built upon this library, it is not recommended
 * to archive the code into a static library.
 * Instead, dynamic link library, shared library or executable are perfered.
 *
 * @see `RegisterClass()`.
 */
#define NSFX_REGISTER_CLASS(T, CID)                                       \
    template<class T>                                                     \
    class Nsfx ## T ## Register                                           \
    {                                                                     \
        static struct Helper                                              \
        {                                                                 \
            Helper(void)                                                  \
            {                                                             \
                try                                                       \
                {                                                         \
                    ::nsfx::RegisterClassFactory<T>(CID);                 \
                }                                                         \
                catch (boost::exception& e)                               \
                {                                                         \
                    std::cerr << diagnostic_information(e) << std::endl;  \
                    throw;                                                \
                }                                                         \
            }                                                             \
        } helper_;                                                        \
    };                                                                    \
    template<class T> typename Nsfx ## T ## Register<T>::Helper           \
                               Nsfx ## T ## Register<T>::helper_;         \
    template class Nsfx ## T ## Register<T>;


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// ClassRegistry
/**
 * @ingroup Component
 * @brief The class registry.
 *
 * It is a singleton.
 *
 * # 1. Register a class.
 * The library provides two tiers of tools for class registration.
 *
 * ## 1.1 Free functions.
 * `RegisterClassFactory()`.
 *
 * ## 1.2 Macro.
 * `NSFX_REGISTER_CLASS()`.
 *
 * # 2. Create an object.
 * The library provides two tiers of tools for object creation.
 *
 * ## 2.1 Make concrete classes.
 * If the type of the implementation class is known, users can use `Object`
 * or `AggObject` to make it concrete, and use `new` to create an object.
 *
 * ## 2.2 Free functions.
 * If the class has been registered, users can use `CreateObject()`
 * function template to create an object.
 * Users just need to know UID of the implementation class, and do not have
 * to know its type.
 *
 * Instead of `CreateObject()`, there are other free functions that create
 * objects, such as `CreateEventSink()`.
 */
class ClassRegistry :
    public IClassRegistry
{
    // Static methods.
public:
    /**
     * @brief Get the `IClassRegistry` interface.
     */
    static IClassRegistry* GetIClassRegistry(void)
    {
        static StaticObject<ClassRegistry>  registry;
        // Omit AddRef() as there is no need to do so.
        // Omit QueryInterface() as the implementation is known.
        return registry.GetImpl();
    }

public:
    virtual ~ClassRegistry(void) BOOST_NOEXCEPT {}

    // IClassRegistry
    virtual void Register(const Uid& cid, Ptr<IClassFactory> factory) NSFX_FINAL NSFX_OVERRIDE
    {
        if (!factory)
        {
            BOOST_THROW_EXCEPTION(InvalidPointer());
        }
        auto result = map_.emplace(cid, std::move(factory));
        if (!result.second)
        {
            BOOST_THROW_EXCEPTION(
                ClassAlreadyRegistered() <<
                ClassUidErrorInfo(cid));
        }
    }

    virtual void Unregister(const Uid& cid) NSFX_FINAL NSFX_OVERRIDE
    {
        map_.erase(cid);
    }

    virtual void UnregisterAll(void) NSFX_FINAL NSFX_OVERRIDE
    {
        map_.clear();
    }

    virtual Ptr<IClassFactory> GetClassFactory(const Uid& cid) NSFX_FINAL NSFX_OVERRIDE
    {
        try
        {
            return map_.at(cid);
        }
        catch (std::out_of_range& )
        {
            BOOST_THROW_EXCEPTION(
                ClassNotRegistered() <<
                ClassUidErrorInfo(cid));
        }
    }

private:
    NSFX_INTERFACE_MAP_BEGIN(ClassRegistry)
        NSFX_INTERFACE_ENTRY(IClassRegistry)
    NSFX_INTERFACE_MAP_END()

private:
    unordered_map<Uid, Ptr<IClassFactory>>  map_;

};


NSFX_DEFINE_CLASS_UID(ClassRegistry, "edu.uestc.nsfx.ClassRegistry");


////////////////////////////////////////////////////////////////////////////////
// Free functions.
/**
 * @ingroup Component
 * @brief Register a class with the default class factory.
 *
 * @param[in] C The class to register.
 *              It must conform to `HasUidConcept` and `ObjectImplConcept`.
 *
 * @see `IClassRegistry::Register()`.
 */
template<class C>
inline void RegisterClassFactory(const Uid& cid)
{
    BOOST_CONCEPT_ASSERT((ObjectImplConcept<C>));
    typedef Object<ClassFactory<C>>  ClassFactoryClass;
    Ptr<IClassFactory> factory(new ClassFactoryClass);
    IClassRegistry* registry = ClassRegistry::GetIClassRegistry();
    registry->Register(cid, std::move(factory));
}

/**
 * @ingroup Component
 * @brief Register a class.
 *
 * @see `IClassRegistry::Register()`.
 */
inline void RegisterClassFactory(const Uid& cid, Ptr<IClassFactory> factory)
{
    IClassRegistry* registry = ClassRegistry::GetIClassRegistry();
    registry->Register(cid, std::move(factory));
}

/**
 * @ingroup Component
 * @brief Register a class.
 *
 * @see `IClassRegistry::Register()`.
 */
inline void UnregisterClassFactory(const Uid& cid)
{
    IClassRegistry* registry = ClassRegistry::GetIClassRegistry();
    registry->Unregister(cid);
}

/**
 * @ingroup Component
 * @brief Create an uninitialized object.
 *
 * @tparam I  The type of interface to query.
 *            It must conform to `IObjectConcept` and `HasUidConcept`.
 *
 * @param[in] cid        The UID of the class.
 * @param[in] controller The controller.
 *
 * @throw ClassNotRegistered
 * @throw BadAggregation
 * @throw NoInterface
 *
 * @see `IClassFactory::CreateObject()`.
 */
template<class I>
inline Ptr<I> CreateObject(const Uid& cid, IObject* controller = nullptr)
{
    BOOST_CONCEPT_ASSERT((IObjectConcept<I>));
    BOOST_CONCEPT_ASSERT((HasUidConcept<I>));
    try
    {
        IClassRegistry* registry = ClassRegistry::GetIClassRegistry();
        Ptr<IClassFactory> factory = registry->GetClassFactory(cid);
        I* p = static_cast<I*>(factory->CreateObject(uid_of<I>(), controller));
        return Ptr<I>(p, true);
    }
    catch (NoInterface& e)
    {
        e << ClassUidErrorInfo(cid);
        throw;
    }
}


NSFX_CLOSE_NAMESPACE


#endif // CLASS_REGISTRY_H__F5D0D382_C0AE_4771_95D9_8DB2A81CCB15

