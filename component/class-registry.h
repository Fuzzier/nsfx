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
 * @param C Must conform to \c HasUuidConcept and \c EnvelopableConcept.<br/>
 *          Must not use qualified name.
 *
 * @see \c RegisterClass().
 */
#define NSFX_REGISTER_CLASS(C)                           \
    static struct C ## ClassRegister                     \
    {                                                    \
        C ## ClassRegister(void)                         \
        {                                                \
            static bool registered = false;              \
            try                                          \
            {                                            \
                if (!registered)                         \
                {                                        \
                    ::nsfx::RegisterClass<C>();          \
                    registered = true;                   \
                }                                        \
            }                                            \
            catch (boost::exception& e)                  \
            {                                            \
                std::cerr << diagnostic_information(e);  \
                throw;                                   \
            }                                            \
        }                                                \
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
        typedef Object<ClassRegistry, false>  ClassRegistryType;
        static ClassRegistryType registry;
        // Omit AddRef() as there is no need to do so.
        // Omit QueryInterface() as the implementation is known.
        return &registry;
    }

    /*}}}*/

public:
    virtual ~ClassRegistry(void) BOOST_NOEXCEPT {}

public:
    // IClassRegistry /*{{{*/
    virtual void Register(const uuid& cid, Ptr<IClassFactory> factory) NSFX_FINAL NSFX_OVERRIDE
    {
        if (!factory)
        {
            BOOST_THROW_EXCEPTION(InvalidPointer());
        }
        auto result = map_.emplace(cid, std::move(factory));
        if (!result.second)
        {
            BOOST_THROW_EXCEPTION(ClassIsRegistered());
        }
    }

    virtual void Unregister(const uuid& cid) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
    {
        map_.erase(cid);
    }

    virtual void UnregisterAll(void) BOOST_NOEXCEPT NSFX_FINAL NSFX_OVERRIDE
    {
        map_.clear();
    }

    virtual Ptr<IClassFactory> GetClassFactory(const uuid& cid) NSFX_FINAL NSFX_OVERRIDE
    {
        try
        {
            return map_.at(cid);
        }
        catch (std::out_of_range& )
        {
            BOOST_THROW_EXCEPTION(ClassNotRegistered());
        }
    }

    /*}}}*/

    NSFX_INTERFACE_MAP_BEGIN(ClassRegistry)
        NSFX_INTERFACE_ENTRY(IClassRegistry)
    NSFX_INTERFACE_MAP_END()

private:
    unordered_map<uuid, Ptr<IClassFactory> >  map_;

}; // class ClassRegistry /*}}}*/


NSFX_DEFINE_CLASS_UUID4(ClassRegistry, 0xC229D24E, 0xC71F, 0x4C23, 0x8615EB2054B4FBB2LL);


////////////////////////////////////////////////////////////////////////////////
// Helper functions./*{{{*/
/**
 * @ingroup Component
 * @brief Register a class with the default class factory.
 *
 * @param C The class to register.<br/>
 *          It must conform to \c HasUuidConcept and \c EnvelopableConcept.
 *
 * @see \c IClassRegistry::Register().
 */
template<class C>
inline void RegisterClass(typename ::boost::mpl::identity<C>::type* = nullptr)
{
    BOOST_CONCEPT_ASSERT((HasUuidConcept<C>));
    BOOST_CONCEPT_ASSERT((EnvelopableConcept<C>));
    typedef Object<ClassFactory<C> >  ClassFactoryType;
    Ptr<IClassFactory> factory(new ClassFactoryType);
    IClassRegistry* registry = ClassRegistry::GetIClassRegistry();
    NSFX_ASSERT(registry);
    registry->Register(uuid_of<C>(), std::move(factory));
}

/**
 * @ingroup Component
 * @brief Register a class.
 *
 * @see \c IClassRegistry::Register().
 */
inline void RegisterClass(const uuid& cid, Ptr<IClassFactory> factory)
{
    IClassRegistry* registry = ClassRegistry::GetIClassRegistry();
    NSFX_ASSERT(registry);
    registry->Register(cid, std::move(factory));
}

/**
 * @ingroup Component
 * @brief Unregister a class.
 *
 * @param C The class to unregister.<br/>
 *          It must conform to \c HasUuidConcept.
 *
 * @see \c IClassRegistry::Unregister().
 */
template<class C>
inline void UnregisterClass(typename ::boost::mpl::identity<C>::type* = nullptr) BOOST_NOEXCEPT
{
    BOOST_CONCEPT_ASSERT((HasUuidConcept<C>));
    IClassRegistry* registry = ClassRegistry::GetIClassRegistry();
    NSFX_ASSERT(registry);
    registry->Unregister(uuid_of<C>());
}

/**
 * @ingroup Component
 * @brief Unregister a class.
 *
 * @see \c IClassRegistry::Unregister().
 */
inline void UnregisterClass(const uuid& cid) BOOST_NOEXCEPT
{
    IClassRegistry* registry = ClassRegistry::GetIClassRegistry();
    NSFX_ASSERT(registry);
    registry->Unregister(cid);
}

/**
 * @ingroup Component
 * @brief Unregister all classes.
 *
 * @see \c IClassRegistry::UnregisterAll().
 */
inline void UnregisterAllClasses(void) BOOST_NOEXCEPT
{
    IClassRegistry* registry = ClassRegistry::GetIClassRegistry();
    NSFX_ASSERT(registry);
    registry->UnregisterAll();
}

/**
 * @ingroup Component
 * @brief Create an uninitialized object.
 *
 * @tparam I  The type of interface to query.<br/>
 *            It must conform to \c IObjectConcept and \c HasUuidConcept.
 *
 * @param[in] cid        The uuid of the class.
 * @param[in] controller The controller.
 *
 * @throw ClassNotRegistered
 * @throw BadAggregation
 * @throw NoInterface
 *
 * @see \c IClassFactory::CreateObject().
 */
template<class I>
inline Ptr<I> CreateObject(const uuid& cid, IObject* controller = nullptr)
{
    BOOST_CONCEPT_ASSERT((IObjectConcept<I>));
    BOOST_CONCEPT_ASSERT((HasUuidConcept<I>));
    IClassRegistry* registry = ClassRegistry::GetIClassRegistry();
    NSFX_ASSERT(registry);
    Ptr<IClassFactory> factory = registry->GetClassFactory(cid);
    I* p = static_cast<I*>(factory->CreateObject(uuid_of<I>(), controller));
    return Ptr<I>(p, true);
}

/*}}}*/


NSFX_CLOSE_NAMESPACE


#endif // CLASS_REGISTRY_H__F5D0D382_C0AE_4771_95D9_8DB2A81CCB15

