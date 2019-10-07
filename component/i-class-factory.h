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

#ifndef I_CLASS_FACTORY_H__58E74ECC_A474_4041_9605_4E45C759D3D6
#define I_CLASS_FACTORY_H__58E74ECC_A474_4041_9605_4E45C759D3D6


#include <nsfx/component/config.h>
#include <nsfx/component/i-object.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// IClassFactory
/**
 * @ingroup Component
 * @brief The class factory interface.
 */
class IClassFactory :/*{{{*/
    virtual public IObject
{
public:
    virtual ~IClassFactory(void) BOOST_NOEXCEPT {}

public:
    /**
     * @brief Create an uninitialized object.
     *
     * Users pass a valid `controller` to create an aggregable object.
     * In such condition, the `iid` must be the UID of `IObject` to obtain
     * the the navigator of the aggregated object.
     * Otherwise, the function throws `BadAggregation`.
     *
     * @throw BadAggregation The exception is thrown if `controller` is not
     *                       `nullptr`, and `iid` is not the UID of `IObject`.
     * @throw OutOfMemory
     * @throw NoInterface
     */
    virtual void* CreateObject(const Uid& iid, IObject* controller) = 0;

}; // class IClassFactory /*}}}*/


NSFX_DEFINE_CLASS_UID(IClassFactory, "edu.uestc.nsfx.IClassFactory");


NSFX_CLOSE_NAMESPACE


#endif // I_CLASS_FACTORY_H__58E74ECC_A474_4041_9605_4E45C759D3D6

