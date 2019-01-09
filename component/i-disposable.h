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
#include <nsfx/component/uid.h>
#include <nsfx/component/i-object.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// IDisposable.
/**
 * @ingroup Component
 * @brief A disposable object.
 */
class IDisposable :
    virtual public IObject
{
public:
    virtual ~IDisposable(void) BOOST_NOEXCEPT {}

    virtual void Dispose(void) = 0;

};

NSFX_DEFINE_CLASS_UID(IDisposable, "edu.uestc.nsfx.IDisposable");


NSFX_CLOSE_NAMESPACE


#endif // I_DISPOSABLE_H__1F095864_053F_4229_8401_71FE616A52C0

