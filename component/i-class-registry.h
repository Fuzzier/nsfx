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
     * @param[in] cid The UID of an object class.
     *                The class must conform to `EnvelopableConcept`.
     * @param[in] factory The class factory.
     *
     * @throw OutOfMemory
     * @throw ClassAlreadyRegistered The class is already registered.
     * @throw InvalidPointer The factory is `nullptr`.
     */
    virtual void Register(const Uid& cid, Ptr<IClassFactory> factory) = 0;

    /**
     * @brief Unregister a class factory.
     *
     * @param[in] cid The UID of an object class.
     */
    virtual void Unregister(const Uid& cid) = 0;

    /**
     * @brief Unregister all class factories.
     */
    virtual void UnregisterAll(void) = 0;

    /**
     * @brief Get a class factory.
     *
     * @param[in] cid The UID of an object class.
     *                The class must conform to `ObjectImplConcept`.
     *
     * @throw ClassNotRegistered The class is not registered.
     */
    virtual Ptr<IClassFactory> GetClassFactory(const Uid& cid) = 0;

}; // class IClassRegistry /*}}}*/


NSFX_DEFINE_CLASS_UID(IClassRegistry, "edu.uestc.nsfx.IClassRegistry");


NSFX_CLOSE_NAMESPACE


#endif // I_CLASS_REGISTRY_H__24814113_77CB_46E0_AF88_DDFB672AB6CE

