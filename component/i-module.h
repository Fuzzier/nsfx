/**
 * @file
 *
 * @brief Component support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2020-07-07
 *
 * @copyright Copyright (c) 2020.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef I_MODULE_H__ABEE33FF_C64C_436E_A801_EF3EC8A73B5E
#define I_MODULE_H__ABEE33FF_C64C_436E_A801_EF3EC8A73B5E


#include <nsfx/component/config.h>
#include <nsfx/component/uid.h>
#include <nsfx/component/i-object.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Component
 * @brief A module provides a standard set of methods.
 *
 * * `Initialize()`: called after all required interfaces have been provided, so
 *                 the object can use the acquired interfaces to create and
 *                 initialize internal objects.
 * * `Dispose()`: called to destroy the object in order to break cyclic reference
 *              counting proactively.
 *
 * To properly dispose an object, the object must do the following operations.
 * * Become uninitialized.
 * * Cancel all scheduled events.
 * * Disconnect all event connections made by itself.
 * * Dispose all objects created by itself.
 * * Release all used interfaces.
 */
class IModule :
    virtual public IObject
{
public:
    virtual ~IModule(void) BOOST_NOEXCEPT {};

    virtual void Initialize(void) = 0;
    virtual void Configure(void) = 0;
    virtual void Dispose(void) = 0;
};

NSFX_DEFINE_CLASS_UID(IModule, "edu.uestc.nsfx.IModule");


NSFX_CLOSE_NAMESPACE


#endif // I_MODULE_H__ABEE33FF_C64C_436E_A801_EF3EC8A73B5E

