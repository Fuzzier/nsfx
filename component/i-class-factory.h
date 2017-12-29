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
     * Users pass a valid \c controller to create an aggregable object.<br/>
     * In such condition, the \c iid must be the uuid of \c IObject to obtain
     * the the navigator of the aggregated object.<br/>
     * Otherwise, the function throws \c BadAggregation.<br/>
     *
     * @throw BadAggregation The exception is thrown if \c controller is not
     *                       \c nullptr, and \c iid is not the uuid of
     *                       \c IObject.
     * @throw OutOfMemory
     * @throw NoInterface
     */
    virtual void* CreateObject(const uuid& iid, IObject* controller) = 0;

}; // class IClassFactory /*}}}*/


NSFX_DEFINE_CLASS_UUID4(IClassFactory, 0x3F197227, 0x3015, 0x43CD, 0x977E8C0DEEF84F7ALL);


NSFX_CLOSE_NAMESPACE


#define NSFX_IID_IClassFactory  NSFX_UUID_OF(IClassFactory)


#endif // I_CLASS_FACTORY_H__58E74ECC_A474_4041_9605_4E45C759D3D6

