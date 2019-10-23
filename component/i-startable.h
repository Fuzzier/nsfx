/**
 * @file
 *
 * @brief Component support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-10-25
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef I_STARTABLE_H__0E290E97_A4DA_4B48_AA53_EE15B1B8D675
#define I_STARTABLE_H__0E290E97_A4DA_4B48_AA53_EE15B1B8D675


#include <nsfx/component/config.h>
#include <nsfx/component/uid.h>
#include <nsfx/component/i-object.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Component
 * @brief A startable object.
 */
class IStartable :
    virtual public IObject
{
public:
    virtual ~IStartable(void) BOOST_NOEXCEPT {};

    /**
     * @brief Put an object into running state.
     *
     * The *started* state is different from the *initialized* state.
     *
     * When an object exposes `IStartable` interface, it **shall** be in
     * stopped state initially.
     *
     * To be able to put an object into running state, the object **must**
     * have been initialized already.
     * Therefore, it is recommended to expose `IInitializable` to perform
     * *one-time* initialization, such as *wiring*; while expose `IStartable`
     * and `IStoppable` to allow runtime configuration/re-configuration,
     * such as setting parameters and connecting events.
     */
    virtual void Start(void) = 0;
};

NSFX_DEFINE_CLASS_UID(IStartable, "edu.uestc.nsfx.IStartable");


NSFX_CLOSE_NAMESPACE


#endif // I_STARTABLE_H__0E290E97_A4DA_4B48_AA53_EE15B1B8D675

