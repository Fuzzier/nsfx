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
#include <nsfx/event/i-event.h>
#include <nsfx/component/i-object.h>
#include <nsfx/component/ptr.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Types.
class ITimer;
class ITimerHandle;
class ITimerUser;

#define NSFX_IID_ITimer        NSFX_UUID_OF(::nsfx::ITimer)
#define NSFX_IID_ITimerHandle  NSFX_UUID_OF(::nsfx::ITimerHandle)
#define NSFX_IID_ITimerSink    NSFX_UUID_OF(::nsfx::ITimerSink)


////////////////////////////////////////////////////////////////////////////////
// ITimerHandle.
class ITimerHandle :
    virtual public IObject
{
public:
    virtual ~ITimerHandle(void) BOOST_NOEXCEPT {}

    virtual void Stop(void) = 0;

}; // class ITimerHandle


NSFX_DEFINE_CLASS_UUID(ITimerHandle, 0xEC57866B, 0x28D1, 0x4C73, 0xBE85F4FE32732235LL);


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

    virtual Ptr<ITimerHandle> StartNow(const Duration& period,
                                       Ptr<IEventSink<> > sink) = 0;

    virtual Ptr<ITimerHandle> StartAt(const TimePoint& t0,
                                      const Duration& period,
                                      Ptr<IEventSink<> > sink) = 0;

}; // class ITimer


NSFX_DEFINE_CLASS_UUID(ITimer, 0x91A857DC, 0x2AE6, 0x496E, 0xA397FA5C80A66228LL);


////////////////////////////////////////////////////////////////////////////////
// ITimerUser.
class ITimerUser :
    virtual public IObject
{
public:
    virtual ~ITimerUser(void) BOOST_NOEXCEPT {}

    virtual void Use(Ptr<ITimer> timer) = 0;

}; // class ITimerUser


NSFX_DEFINE_CLASS_UUID(ITimerUser, 0x64A3D699, 0xCEC6, 0x44AD, 0xB1B4B06505A087A2LL);


NSFX_CLOSE_NAMESPACE


#endif // I_TIMER_H__55A9E030_C267_476F_873E_9B289CD21D38

