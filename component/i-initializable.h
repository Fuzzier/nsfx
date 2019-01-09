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

#ifndef I_INITIALIZABLE_H__69B98B5C_0DCA_48C3_AB8B_9FAEF6EA6477
#define I_INITIALIZABLE_H__69B98B5C_0DCA_48C3_AB8B_9FAEF6EA6477


#include <nsfx/component/config.h>
#include <nsfx/component/uid.h>
#include <nsfx/component/i-object.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Component
 * @brief An initializable object.
 */
class IInitializable :
    virtual public IObject
{
public:
    virtual ~IInitializable(void) BOOST_NOEXCEPT {};

    virtual void Initialize(void) = 0;
};

NSFX_DEFINE_CLASS_UID(IInitializable, "edu.uestc.nsfx.IInitializable");


NSFX_CLOSE_NAMESPACE


#endif // I_INITIALIZABLE_H__69B98B5C_0DCA_48C3_AB8B_9FAEF6EA6477

