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
#include <boost/type_traits/is_same.hpp>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// ClassRegistry
/**
 * @ingroup Component
 * @brief The class registry.
 *
 * It is a singleton.
 */
class ClassRegistry NSFX_FINAL:/*{{{*/
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
        // Omit QueryInterface() as the implementation is known.
        return &registry;
    }

    /*}}}*/

private:
    ClassRegistry(void) BOOST_NOEXCEPT {}

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


NSFX_DEFINE_CALSS_UUID4(ClassRegistry, 0xC229D24E, 0xC71F, 0x4C23, 0x8615EB2054B4FBB2LL);


////////////////////////////////////////////////////////////////////////////////
// Helper functions./*{{{*/
/**
 * @ingroup Component
 * @brief Register a class.
 *
 * @see \c IClassRegistry::Register().
 */
void RegisterClass(const uuid& cid, Ptr<IFactory> factory)
{
    IClassRegistry* reg = ClassRegistry::GetIClassRegistry();
    NSFX_ASSERT(reg);
    reg->Register(cid, factory);
}

/**
 * @ingroup Component
 * @brief Create an uninitialized object.
 *
 * @tparam Intf  The type of interface to query.<br/>
 *               It must conform to \c IObjectConcept and \c HasUuidConcept.
 *
 * @param[in] cid        The uuid of the class.
 * @param[in] controller The controller.
 *
 * @throw ClassNotRegistered
 * @throw BadAggregation
 * @throw OutOfMemory
 * @throw NoInterface
 *
 * @see \c IClassFactory::CreateObject().
 */
template<class Intf>
Ptr<Intf> CreateObject(const uuid& cid, IObject* controller)
{
    IClassRegistry* registry = ClassRegistry::GetIClassRegistry();
    NSFX_ASSERT(registry);
    Ptr<IFactory> factory = registry->GetClassFactory(cid);
    Intf* p = static_cast<Intf>(factory->CreateObject(controller));
    return Ptr<Intf>(p, true);
}

/*}}}*/


NSFX_CLOSE_NAMESPACE


#endif // CLASS_REGISTRY_H__F5D0D382_C0AE_4771_95D9_8DB2A81CCB15

