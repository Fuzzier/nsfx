/**
 * @file
 *
 * @brief Component support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-01-30
 *
 * @copyright Copyright (c) 2018.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef I_USER_H__33CDE8D4_1EA9_4078_AE0A_873524A43C98
#define I_USER_H__33CDE8D4_1EA9_4078_AE0A_873524A43C98


#include <nsfx/component/config.h>
#include <nsfx/component/uuid.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/ptr.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Component
 * @brief Define a user interface.
 *
 * @param IXxxUser The name of the user interface.
 * @param IXxx     The name of the used interface.
 *
 * A <i>'User'</i> interface has a single method \c Use(Ptr<IXxx>).
 *
 * @example For example
 *    @code
 *    NSFX_DEFINE_USER_INTERFACE(
 *        IMyInterfaceUser,
 *        IMyInterface,  0x68EB6D5D, 0x1467, 0x44A1, 0x8ACB71B83E595430LL);
 *    @endcode
 */
#define NSFX_DEFINE_USER_INTERFACE(IXxxUser, IXxx, l, w1, w2, ll)   \
    NSFX_DEFINE_CLASS_UUID(class IXxxUser, l, w1, w2, ll);          \
    class IXxxUser :                                                \
        virtual public ::nsfx::IObject                              \
    {                                                               \
    public:                                                         \
        virtual ~IXxxUser(void) BOOST_NOEXCEPT {}                   \
        virtual void Use(Ptr<IXxx> x) = 0;                          \
    }


NSFX_CLOSE_NAMESPACE


#endif // I_USER_H__33CDE8D4_1EA9_4078_AE0A_873524A43C98

