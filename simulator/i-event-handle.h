/**
 * @file
 *
 * @brief Simulator for Network Simulation Frameworks.
 *
 * @version 1.0
 * @author  Wei Tang <gauchyler@uestc.edu.cn>
 * @date    2017-12-18
 *
 * @copyright Copyright (c) 2017.
 *   National Key Laboratory of Science and Technology on Communications,
 *   University of Electronic Science and Technology of China.
 *   All rights reserved.
 */

#ifndef I_EVENT_HANDLE_H__BE783B0F_52E9_4314_A6E8_4A229220AFB2
#define I_EVENT_HANDLE_H__BE783B0F_52E9_4314_A6E8_4A229220AFB2


#include <nsfx/simulator/config.h>
#include <nsfx/simulator/i-event-sink.h>


NSFX_OPEN_NAMESPACE


////////////////////////////////////////////////////////////////////////////////
// Types.
class IEventHandle;

#define NSFX_IID_IEventHandle  NSFX_UUID_OF(::nsfx::IEventHandle)


////////////////////////////////////////////////////////////////////////////////
// IEventHandle.
/**
 * @ingroup Simulator
 * @brief The event handle interface.
 */
class IEventHandle :/*{{{*/
    virtual public IObject
{
public:
    virtual ~IEventHandle(void) BOOST_NOEXCEPT {}

    /**
     * @brief The alarm is waiting to be fired.
     */
    virtual bool IsPending(void) = 0;

    /**
     * @brief The alarm is being fired.
     */
    virtual bool IsRunning(void) = 0;

    /**
     * @brief The alarm is waiting or being fired.
     */
    virtual bool IsValid(void) = 0;

    /**
     * @brief Cancel the alarm.
     */
    virtual void Cancel(void) = 0;

    /**
     * @brief Get the time point when the alarm is about to be fired.
     */
    virtual TimePoint GetTimePoint(void) = 0;

    /**
     * @brief Fire the event.
     *
     * @pre <code>this == IEventScheduler::GetNextEvent()</code>
     *
     * @post <code>!IsPending() && !IsRunning() && !IsValid()</code>
     *
     * @throw IllegalMethodCall The event is not the next event to fire.
     */
    virtual void Fire(void) = 0;

}; // class IEventHandle /*}}}*/


NSFX_DEFINE_CLASS_UUID4(IEventHandle, 0x9563654A, 0x8984, 0x448A, 0xBBFD8C654C7AEC61LL);


NSFX_CLOSE_NAMESPACE


#endif // I_EVENT_HANDLE_H__BE783B0F_52E9_4314_A6E8_4A229220AFB2

