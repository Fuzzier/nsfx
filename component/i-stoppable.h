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

#ifndef I_STOPPABLE_H__1EC2A1F9_A646_4976_809D_F31A89BBDC47
#define I_STOPPABLE_H__1EC2A1F9_A646_4976_809D_F31A89BBDC47


#include <nsfx/component/config.h>
#include <nsfx/component/uid.h>
#include <nsfx/component/i-object.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
/**
 * @ingroup Component
 * @brief A stoppable object.
 */
class IStoppable :
    virtual public IObject
{
public:
    virtual ~IStoppable(void) BOOST_NOEXCEPT {};

    /**
     * @brief Put an object into a stopped state.
     *
     * The *stopped* state is different from the *uninitialized* state.
     *
     * An uninitialized object cannot work as the required resources have not
     * been fully acquired.
     * Thus, it **must** throw exceptions when it is acked to perform a task
     * that requires the missing resources.
     *
     * For an initialized object, it **shall** ignore incomming events and stop
     * emitting events when it is stopped.
     * However, it **may** throw exceptions when its commands are invoked,
     * in order to give the caller an warning that the state of the object
     * should have been checked.
     */
    virtual void Stop(void) = 0;
};

NSFX_DEFINE_CLASS_UID(IStoppable, "edu.uestc.nsfx.IStoppable");


NSFX_CLOSE_NAMESPACE


#endif // I_STOPPABLE_H__1EC2A1F9_A646_4976_809D_F31A89BBDC47

