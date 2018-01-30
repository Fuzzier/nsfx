/**
 * @file
 *
 * @brief Simulation support for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2018-01-02
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef I_TIMER_H__55A9E030_C267_476F_873E_9B289CD21D38
#define I_TIMER_H__55A9E030_C267_476F_873E_9B289CD21D38


#include <nsfx/simulation/config.h>
#include <nsfx/simulation/i-clock.h>
#include <nsfx/event/i-event-sink.h>
#include <nsfx/component/i-user.h>
#include <nsfx/component/ptr.h>
NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Types.
class ITimer;
class ITimerUser;

#define NSFX_IID_ITimer        NSFX_UUID_OF(::nsfx::ITimer)
#define NSFX_IID_ITimerSink    NSFX_UUID_OF(::nsfx::ITimerSink)


////////////////////////////////////////////////////////////////////////////////
// ITimer.
/**
 * @ingroup Simulator
 * @brief A periodic timer.
 */
class ITimer :
    virtual public IObject
{
public:
    virtual ~ITimer(void) BOOST_NOEXCEPT {}

    virtual void StartNow(const Duration& period,
                          Ptr<IEventSink<> > sink) = 0;

    virtual void StartAt(const TimePoint& t0,
                         const Duration& period,
                         Ptr<IEventSink<> > sink) = 0;

    virtual void Stop(void) = 0;

}; // class ITimer


NSFX_DEFINE_CLASS_UUID(ITimer, 0x91A857DC, 0x2AE6, 0x496E, 0xA397FA5C80A66228LL);


////////////////////////////////////////////////////////////////////////////////
// ITimerUser.
NSFX_DEFINE_USER_INTERFACE(
    ITimerUser,
    ITimer,  0xE5F38F1D, 0x9A16, 0x4E5E, 0xAE7FD827CD000EE2LL);


NSFX_CLOSE_NAMESPACE


#endif // I_TIMER_H__55A9E030_C267_476F_873E_9B289CD21D38

