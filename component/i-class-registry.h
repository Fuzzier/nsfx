/**
 * @file
 *
 * @brief Component support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-25
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef I_CLASS_REGISTRY_H__24814113_77CB_46E0_AF88_DDFB672AB6CE
#define I_CLASS_REGISTRY_H__24814113_77CB_46E0_AF88_DDFB672AB6CE


#include <nsfx/component/config.h>
#include <nsfx/component/i-class-factory.h>
#include <nsfx/component/ptr.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// IClassRegistry
/**
 * @ingroup Component
 * @brief The class registry interface.
 */
class IClassRegistry :/*{{{*/
    virtual public IObject
{
public:
    virtual ~IClassRegistry(void) BOOST_NOEXCEPT {}

public:
    /**
     * @brief Register a class factory.
     *
     * @param[in] cid The uuid of an object class.<br/>
     *                The class must conform to \c EnvelopableConcept.
     * @param[in] factory The class factory.
     *
     * @throw OutOfMemory
     * @throw ClassIsRegistered The class is already registered.
     * @throw InvalidPointer The factory is \c nullptr.
     */
    virtual void Register(const uuid& cid, Ptr<IClassFactory> factory) = 0;

    /**
     * @brief Unregister a class factory.
     *
     * @param[in] cid The uuid of an object class.<br/>
     */
    virtual void Unregister(const uuid& cid) = 0;

    /**
     * @brief Unregister all class factories.
     */
    virtual void UnregisterAll(void) = 0;

    /**
     * @brief Get an class factory.
     *
     * @param[in] cid The uuid of an object class.<br/>
     *                The class must conform to \c EnvelopableConcept.
     *
     * @throw ClassNotRegistered The class is not registered.
     */
    virtual Ptr<IClassFactory> GetClassFactory(const uuid& cid) = 0;

}; // class IClassRegistry /*}}}*/


NSFX_DEFINE_CLASS_UUID4(IClassRegistry, 0x46F5D329, 0xE6EC, 0x4816, 0x959939B6B707FC1CLL);


NSFX_CLOSE_NAMESPACE


#define NSFX_IID_IClassRegistry  NSFX_IID_OF(::nsfx::IClassRegistry)


#endif // I_CLASS_REGISTRY_H__24814113_77CB_46E0_AF88_DDFB672AB6CE

