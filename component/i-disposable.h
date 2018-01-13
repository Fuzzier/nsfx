/**
 * @file
 *
 * @brief Component support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-01-11
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef I_DISPOSABLE_H__1F095864_053F_4229_8401_71FE616A52C0
#define I_DISPOSABLE_H__1F095864_053F_4229_8401_71FE616A52C0


#include <nsfx/component/config.h>
#include <nsfx/component/i-object.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Types.
class IDisposable;

#define NSFX_IID_IDisposable      NSFX_UUID_OF(::nsfx::IDisposable)


////////////////////////////////////////////////////////////////////////////////
// IDisposable.
/**
 * @ingroup Simulator
 * @brief The disposable interface.
 *
 * In order to break cyclic referencing, an object shall expose this interface.<br/>
 * Disposing an object forces the it to release all reference counts it holds.
 */
class IDisposable :
    virtual public IObject
{
public:
    virtual ~IDisposable(void) BOOST_NOEXCEPT {}

    virtual void Dispose(void) = 0;

}; // class IDisposable


NSFX_DEFINE_CLASS_UUID4(IDisposable, 0x7577307E, 0x3D36, 0x485F, 0xB7E3EFAE7E855CFDLL);


NSFX_CLOSE_NAMESPACE


#endif // I_DISPOSABLE_H__1F095864_053F_4229_8401_71FE616A52C0

