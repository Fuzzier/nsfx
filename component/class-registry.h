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
 * @brief A helper class template that registers a class with a default factory.
 *
 * @param C   Must conform to \c HasUidConcept and \c ObjectImplConcept.
 *            Must not use qualified name.
 * @param cid The ID of the class.
 *
 * ============
 * Requirements
 * ============
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
 * * Inline
 *
 *   Component writers use a macro to associate a CID with a class.
 *   Usually, the macro is written along with the class in the same source
 *   file.
 *   They do not write the information in a separate location, such as
 *   Windows Registry or configuration files.
 *
 * * Self-driven
 *
 *   The class registration is performed during program startup, since the
 *   information is coded in the program itself.
 *   Component users do not need to use loader programs to register the
 *   clases, or create instances.
 *   This is specially helpful in an environment that forbids the usage of
 *   Windows Registry or loader programs.
 *
 * * Non-intrusive
 *
 *   Similar to IID, the macro to associate a CID with a class is placed
 *   outside of the class definition.
 *
 *   The difference is that, an IID is bound to the type of an interface.
 *   That is, an interface can have one and only one IID.
 *   However, a CID is associated loosely with a class.
 *   That is, a class may have multiple CIDs.
 *
 *  * Header friendly
 *
 *    The macro must be able to be placed in header files, so the library
 *    remains header-only.
 *
 * ========
 * Key idea
 * ========
 *
 * 1. Define a helper class whose constructor performs the registration.
 * 2. Define a static instance of the helper class, so the constructor is
 *    called during program startup.
 *
 * =======
 * Methods
 * =======
 *
 * ## Global static variable
 *
 *    A common method is to defined a static instance of the class directly.
 *    @code
 *    static struct MyClassRegister
 *    {
 *        MyClassRegister(void)
 *        {
 *            ::nsfx::RegisterClassFactory<MyClass>(myCid);
 *        }
 *    } s_MyClassRegister;
 *    @endcode
 *
 *    If the code is placed in a header file, each compilation unit that
 *    includes the header file would have a separate instance of the variable.
 *    Thus, the constructor must use a static flag to prevent multiple
 *    registration.
 *    @code
 *    static struct MyClassRegister
 *    {
 *        MyClassRegister(void)
 *        {
 *            static bool registered = false;
 *            if (!registered)
 *            {
 *                ::nsfx::RegisterClassFactory<MyClass>(myCid);
 *                registered = true;
 *            }
 *        }
 *    } s_MyClassRegister;
 *    @endcode
 *
 * ## Class member static instance.
 *
 *    To prevent the use of an additional static flag, the static instance can
 *    be defined as a class member.
 *    @code
 *    struct MyClassRegister
 *    {
 *        static struct Worker
 *        {
 *            Worker(void)
 *            {
 *                ::nsfx::RegisterClassFactory<MyClass>(myCid);
 *            }
 *        } worker_;
 *    };
 *    MyClassRegister::Worker MyClassRegister::worker_;
 *    @endcode
 *
 *    If the code is placed in a header file, the instance would be defined in
 *    each compilation unit that includes the header file.
 *    This results in multiple definition of the instance.
 *    The work around, a class template can be used in place of the class.
 *    An implicit instantiation of the class template is also declared.
 *    @code
 *    template<class T>
 *    struct MyClassRegister
 *    {
 *        static struct Worker
 *        {
 *            Worker(void)
 *            {
 *                ::nsfx::RegisterClassFactory<MyClass>(myCid);
 *            }
 *        } worker_;
 *    };
 *    template<classs T> MyClassRegister<T>::Worker MyClassRegister<T>::worker_;
 *    template MyClassRegister<MyClass>;
 *    @endcode
 *
 *    However, there is a problem in this method.
 *    That is, the static instance is local to the compilation unit, and will
 *    be <b>removed</b> when the object file is archived in a <i>static library</i>.
 *    To prevent the removal, the static instance must be accessed by some
 *    entity with external linkage in the source file.
 *
 *
 * @see \c RegisterClass().
 */
#define NSFX_REGISTER_CLASS(C, cid)                                       \
    template<class T>                                                     \
    struct C ## Register                                                  \
    {                                                                     \
        static struct Worker                                              \
        {                                                                 \
            Worker(void)                                                  \
            {                                                             \
                try                                                       \
                {                                                         \
                    ::nsfx::RegisterClassFactory<C>(cid);                 \
                }                                                         \
                catch (boost::exception& e)                               \
                {                                                         \
                    std::cerr << diagnostic_information(e) << std::endl;  \
                    throw;                                                \
                }                                                         \
            }                                                             \
        } worker_;                                                        \
    };                                                                    \
    template<class T>                                                     \
    typename C ## Register<T>::Worker C ## Register<T>::worker_;          \
    template C ## Register<C>

    // static struct C ## ClassRegister                                  \
    // {                                                                 \
    //     C ## ClassRegister(void)                                      \
    //     {                                                             \
    //         try                                                       \
    //         {                                                         \
    //             static bool registered = false;                       \
    //             if (!registered)                                      \
    //             {                                                     \
    //                 ::nsfx::RegisterClassFactory<C>(cid);             \
    //                 registered = true;                                \
    //             }                                                     \
    //         }                                                         \
    //         catch (boost::exception& e)                               \
    //         {                                                         \
    //             std::cerr << diagnostic_information(e) << std::endl;  \
    //                 throw;                                            \
    //         }                                                         \
    //     }                                                             \
    // } s_##C##_register;                                               \
    // extern void C##Foo()                                              \
    // {                                                                 \
    //     C ## ClassRegister* s = &s_##C##_register;                    \
    // }


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// ClassRegistry
/**
 * @ingroup Component
 * @brief The class registry.
 *
 * It is a singleton.
 *
 * ## 1. Register a class.
 *    The library provides two tiers of tools for class registration.
 *
 * ### 1.1 Free functions.
 *     \c RegisterClassFactory().
 *
 * ### 1.2 Macro.
 *     \c NSFX_REGISTER_CLASS().
 *
 * ## 2. Create an object.
 *    The library provides two tiers of tools for object creation.
 *
 * ### 2.1 Make concrete classes.
 *     If the type of the implementation class is known, users can use
 *     \c Object or \c AggObject to make it concrete, and use \c new to
 *     create an object.
 *
 * ### 2.2 Free functions.
 *     If the class has been registered, users can use \c CreateObject()
 *     function template to create an object.
 *     Users just need to know UID of the implementation class, and do not have
 *     to know its type.
 *
 *     Instead of \c CreateObject(), there are other free functions that create
 *     objects, such as \c CreateEventSink().
 */
class ClassRegistry :/*{{{*/
    public IClassRegistry
{
    // Static methods. /*{{{*/
public:
    /**
     * @brief Get the \c IClassRegistry interface.
     */
    static IClassRegistry* GetIClassRegistry(void)
    {
        typedef StaticObject<ClassRegistry>  ClassRegistryClass;
        static ClassRegistryClass registry;
        // Omit AddRef() as there is no need to do so.
        // Omit QueryInterface() as the implementation is known.
        return static_cast<IClassRegistry*>(registry.GetImpl());
    }

    /*}}}*/

public:
    virtual ~ClassRegistry(void) BOOST_NOEXCEPT {}

public:
    // IClassRegistry /*{{{*/
    virtual void Register(const Uid& cid, Ptr<IClassFactory> factory) NSFX_FINAL NSFX_OVERRIDE
    {
        if (!factory)
        {
            BOOST_THROW_EXCEPTION(InvalidPointer());
        }
        auto result = map_.emplace(cid, std::move(factory));
        if (!result.second)
        {
            BOOST_THROW_EXCEPTION(ClassAlreadyRegistered() << ClassUidErrorInfo(cid));
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
            BOOST_THROW_EXCEPTION(ClassNotRegistered() << ClassUidErrorInfo(cid));
        }
    }

    /*}}}*/

    NSFX_INTERFACE_MAP_BEGIN(ClassRegistry)
        NSFX_INTERFACE_ENTRY(IClassRegistry)
    NSFX_INTERFACE_MAP_END()

private:
    unordered_map<Uid, Ptr<IClassFactory>>  map_;

}; // class ClassRegistry /*}}}*/


NSFX_DEFINE_CLASS_UID(ClassRegistry, "edu.uestc.nsfx.ClassRegistry");


////////////////////////////////////////////////////////////////////////////////
// Free functions./*{{{*/
/**
 * @ingroup Component
 * @brief Register a class with the default class factory.
 *
 * @param C The class to register.
 *          It must conform to \c HasUidConcept and \c ObjectImplConcept.
 *
 * @see \c IClassRegistry::Register().
 */
template<class C>
inline void RegisterClassFactory(const Uid& cid)
{
    BOOST_CONCEPT_ASSERT((ObjectImplConcept<C>));
    typedef Object<ClassFactory<C>>  ClassFactoryClass;
    Ptr<IClassFactory> factory(new ClassFactoryClass);
    IClassRegistry* registry = ClassRegistry::GetIClassRegistry();
    BOOST_ASSERT(registry);
    registry->Register(cid, std::move(factory));
}

/**
 * @ingroup Component
 * @brief Register a class.
 *
 * @see \c IClassRegistry::Register().
 */
inline void RegisterClassFactory(const Uid& cid, Ptr<IClassFactory> factory)
{
    IClassRegistry* registry = ClassRegistry::GetIClassRegistry();
    BOOST_ASSERT(registry);
    registry->Register(cid, std::move(factory));
}

/**
 * @ingroup Component
 * @brief Register a class.
 *
 * @see \c IClassRegistry::Register().
 */
inline void UnregisterClassFactory(const Uid& cid)
{
    IClassRegistry* registry = ClassRegistry::GetIClassRegistry();
    BOOST_ASSERT(registry);
    registry->Unregister(cid);
}

/**
 * @ingroup Component
 * @brief Create an uninitialized object.
 *
 * @tparam I  The type of interface to query.
 *            It must conform to \c IObjectConcept and \c HasUidConcept.
 *
 * @param[in] cid        The UID of the class.
 * @param[in] controller The controller.
 *
 * @throw ClassNotRegistered
 * @throw BadAggregation
 * @throw NoInterface
 *
 * @see \c IClassFactory::CreateObject().
 */
template<class I>
inline Ptr<I> CreateObject(const Uid& cid, IObject* controller = nullptr)
{
    BOOST_CONCEPT_ASSERT((IObjectConcept<I>));
    BOOST_CONCEPT_ASSERT((HasUidConcept<I>));
    try
    {
        IClassRegistry* registry = ClassRegistry::GetIClassRegistry();
        BOOST_ASSERT(registry);
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

/*}}}*/


NSFX_CLOSE_NAMESPACE


#endif // CLASS_REGISTRY_H__F5D0D382_C0AE_4771_95D9_8DB2A81CCB15

