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
// Macros./*{{{*/
/**
 * @brief Register a class with a default class factory.
 *
 * @param C Must conform to \c HasUidConcept and \c ObjectImplConcept.<br/>
 *          Must not use qualified name.
 *
 * @see \c RegisterClass().
 */
#define NSFX_REGISTER_CLASS(C, cid)                                   \
    static struct C ## ClassRegister                                  \
    {                                                                 \
        C ## ClassRegister(void)                                      \
        {                                                             \
            static bool registered = false;                           \
            try                                                       \
            {                                                         \
                if (!registered)                                      \
                {                                                     \
                    ::nsfx::RegisterClassFactory<C>(cid);             \
                    registered = true;                                \
                }                                                     \
            }                                                         \
            catch (boost::exception& e)                               \
            {                                                         \
                std::cerr << diagnostic_information(e) << std::endl;  \
                throw;                                                \
            }                                                         \
        }                                                             \
    } s_ ## C ## ClassRegister

/*}}}*/


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
 * ### 2.1 Envelope classes.
 *     If the type of the concrete class is known, users can use \c Object and
 *     \c AggObject to envelope the class, and use \c new to create an object.
 *
 * ### 2.2 Free functions.
 *     If the class has been registered, users can use \c CreateObject()
 *     function template to create an object.<br/>
 *     Users just need to know UUID of the concrete class, and do not have to
 *     know the type of the concrete class.<br/>.
 *     <p>
 *     Instead of \c CreateObject(), there are other free functions that create
 *     objects, such as \c CreateEventSink().<br/>
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
        return &registry;
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
            BOOST_THROW_EXCEPTION(ClassIsRegistered() << ClassUidErrorInfo(cid));
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
    unordered_map<Uid, Ptr<IClassFactory> >  map_;

}; // class ClassRegistry /*}}}*/


NSFX_DEFINE_CLASS_UID(ClassRegistry, "edu.uestc.nsfx.ClassRegistry");


////////////////////////////////////////////////////////////////////////////////
// Free functions./*{{{*/
/**
 * @ingroup Component
 * @brief Register a class with the default class factory.
 *
 * @param C The class to register.<br/>
 *          It must conform to \c HasUidConcept and \c ObjectImplConcept.
 *
 * @see \c IClassRegistry::Register().
 */
template<class C>
inline void RegisterClassFactory(const Uid& cid)
{
    BOOST_CONCEPT_ASSERT((ObjectImplConcept<C>));
    typedef Object<ClassFactory<C> >  ClassFactoryClass;
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
 * @tparam I  The type of interface to query.<br/>
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

