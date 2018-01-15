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
 * A disposed object is at an <b>unsable state</b>, thus <b>cannot</b> be used
 * after disposed.
 *
 * In order to break cyclic referencing, an object shall expose this interface.<br/>
 * Disposing an object forces the it to release all reference counts it holds.<br/>
 *
 * This includes
 * * Objects that are used by the disposable.
 * * Event sinks from itself, if the disposable is an event sink.
 * * Event sinks from its users, if the disposable is an event source.
 *
 * ## The disposing process.
 *    To correctly dispose an object, one has to release the reference counts
 *    of it that are held by its <b>users</b>.<br/>
 *    i.e., one has to make all its <b>users</b> to release the reference counts
 *    of it.<br/>
 *    i.e., one has to dispose all its <b>users</b>.<br/>
 *    Therefore, the <b>boundary and order of disposing</b> is very important.<br/>
 *
 * ### The <b>boundary of disposing</b>.
 *     All direct and indirect users of the disposed object, as long as the
 *     users are unable to work without the disposed object.
 *
 * ### The <b>order of disposing</b>.
 *     The order does not matter, as long as all objects within the boundary
 *     are not used during the disposing process.
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

