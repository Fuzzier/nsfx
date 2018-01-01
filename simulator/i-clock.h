/**
 * @file
 *
 * @brief Simulator for Network Simulation Frameworks.
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


#include <nsfx/simulator/config.h>
#include <nsfx/component/i-object.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// IClock.
/**
 * @ingroup Simulator
 * @brief The clock interface.
 */
class IClock :/*{{{*/
    virtual public IObject
{
public:
    virtual ~IClock(void) BOOST_NOEXCEPT {}

    /**
     * @brief Get the current time point.
     */
    virtual TimePoint Now(void) = 0;

}; // class IClock /*}}}*/


NSFX_DEFINE_CLASS_UUID4(IClock, 0xF5B4D834, 0xC959, 0x446C, 0xB16346F2232C9787LL);


////////////////////////////////////////////////////////////////////////////////
// IClockUser.
/**
 * @ingroup Simulator
 * @brief The clock user interface.
 */
class IClockUser :/*{{{*/
    virtual public IObject
{
public:
    virtual ~IClockUser(void) BOOST_NOEXCEPT {}

    /**
     * @brief Provide a clock to the user.
     *
     * @throw InvalidPointer
     * @throw CannotReinitialize
     */
    virtual void UseClock(Ptr<IClock> clock) = 0;

}; // class IClockUser /*}}}*/


NSFX_DEFINE_CLASS_UUID4(IClockUser, 0x493628F2, 0x34A8, 0x4162, 0xA299C3C49BF92EB5LL);


NSFX_CLOSE_NAMESPACE


#define NSFX_IID_IClock      NSFX_UUID_OF(::nsfx::IClock)
#define NSFX_IID_IClockUser  NSFX_UUID_OF(::nsfx::IClockUser)


#endif // I_CLOCK_H__49523D2A_BE6A_45FA_84EA_88307B44F4D2

