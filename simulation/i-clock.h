/**
 * @file
 *
 * @brief Simulation support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-17
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef I_CLOCK_H__49523D2A_BE6A_45FA_84EA_88307B44F4D2
#define I_CLOCK_H__49523D2A_BE6A_45FA_84EA_88307B44F4D2


#include <nsfx/simulation/config.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/i-user.h>
#include <nsfx/component/ptr.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// IClock.
/**
 * @ingroup Simulator
 * @brief A clock.
 */
class IClock :
    virtual public IObject
{
public:
    virtual ~IClock(void) BOOST_NOEXCEPT {}

    /**
     * @brief Get the current time point.
     */
    virtual TimePoint Now(void) = 0;

};


NSFX_DEFINE_CLASS_UID(IClock, "edu.uestc.nsfx.IClock");


////////////////////////////////////////////////////////////////////////////////
// IClockUser.
NSFX_DEFINE_USER_INTERFACE(
    IClockUser, "edu.uestc.nsfx.IClockUser",
    IClock);


NSFX_CLOSE_NAMESPACE


#endif // I_CLOCK_H__49523D2A_BE6A_45FA_84EA_88307B44F4D2

